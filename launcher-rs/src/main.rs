mod github;
mod installer;
mod state;
mod updater;

use anyhow::{anyhow, Result};
use clap::{Parser, Subcommand};
use github::{is_newer_version, pick_asset_for_platform};
use state::{load_state, save_state};
use std::path::{Path, PathBuf};
use std::process::Command as ProcessCommand;

#[derive(Parser, Debug)]
#[command(name = "mcx-launcher")]
#[command(about = "MCX Rust launcher (Sprint 3)")]
struct Cli {
    #[command(subcommand)]
    command: Command,
}

#[derive(Subcommand, Debug)]
enum Command {
    /// Check latest release and compare against a local version
    Check {
        #[arg(long, default_value = "v0.0.0")]
        current: String,
    },
    /// Download latest platform asset
    Download {
        #[arg(long)]
        output: String,
    },
    /// Install latest platform asset into an install directory
    Install {
        #[arg(long, default_value = "./.mcx-runtime")]
        dir: String,
    },
    /// Launch installed MCX binary from state file
    Launch {
        #[arg(long, default_value = "./.mcx-runtime")]
        dir: String,
        #[arg(trailing_var_arg = true)]
        args: Vec<String>,
    },
}

#[tokio::main]
async fn main() -> Result<()> {
    let cli = Cli::parse();

    match cli.command {
        Command::Check { current } => {
            let release = updater::fetch_latest_release().await?;
            let selected = pick_asset_for_platform(&release.assets);

            println!("Current : {current}");
            println!("Latest  : {}", release.tag_name);
            println!("Newer   : {}", is_newer_version(&release.tag_name, &current));
            println!("Release : {}", release.html_url);
            println!(
                "Asset   : {}",
                selected.map(|a| a.name.as_str()).unwrap_or("<none>")
            );
        }
        Command::Download { output } => {
            let release = updater::fetch_latest_release().await?;
            let asset = pick_asset_for_platform(&release.assets);
            let url = updater::require_asset_url(asset.map(|a| a.browser_download_url.as_str()))?;

            updater::download_file(url, &output).await?;
            println!("Downloaded latest asset to: {output}");
        }
        Command::Install { dir } => install_latest(Path::new(&dir)).await?,
        Command::Launch { dir, args } => launch_installed(Path::new(&dir), &args)?,
    }

    Ok(())
}

async fn install_latest(install_dir: &Path) -> Result<()> {
    let release = updater::fetch_latest_release().await?;
    let asset = pick_asset_for_platform(&release.assets)
        .ok_or_else(|| anyhow!("No release assets available"))?;

    let release_dir = install_dir.join("releases").join(&release.tag_name);
    let archive_name = format!("{}.download", asset.name);
    let archive_path = install_dir.join("cache").join(archive_name);

    std::fs::create_dir_all(archive_path.parent().ok_or_else(|| anyhow!("Invalid cache path"))?)?;

    println!("Installing {}", release.tag_name);
    println!("Asset: {}", asset.name);

    updater::download_file(&asset.browser_download_url, archive_path.to_string_lossy().as_ref()).await?;

    if release_dir.exists() {
        std::fs::remove_dir_all(&release_dir)?;
    }
    std::fs::create_dir_all(&release_dir)?;
    installer::extract_archive(&archive_path, &release_dir)?;

    let exe = installer::locate_executable(&release_dir)
        .ok_or_else(|| anyhow!("Installed package does not contain an MCX executable"))?;

    let state_file = install_dir.join("launcher-state.json");
    let mut state = load_state(&state_file)?;
    state.installed_version = Some(release.tag_name.clone());
    state.executable_path = Some(exe.to_string_lossy().to_string());
    save_state(&state_file, &state)?;

    println!("Install complete");
    println!("Version   : {}", release.tag_name);
    println!("Executable: {}", exe.display());

    Ok(())
}

fn launch_installed(install_dir: &Path, args: &[String]) -> Result<()> {
    let state_file = install_dir.join("launcher-state.json");
    let state = load_state(&state_file)?;
    let exe = state
        .executable_path
        .ok_or_else(|| anyhow!("No installed executable found. Run `install` first."))?;

    let mut cmd = ProcessCommand::new(PathBuf::from(&exe));
    cmd.args(args);

    println!("Launching: {}", exe);
    let status = cmd.status()?;

    if status.success() {
        Ok(())
    } else {
        Err(anyhow!("MCX exited with status: {status}"))
    }
}
