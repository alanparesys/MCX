mod github;
mod updater;

use anyhow::Result;
use clap::{Parser, Subcommand};
use github::{is_newer_version, pick_asset_for_platform};

#[derive(Parser, Debug)]
#[command(name = "mcx-launcher")]
#[command(about = "MCX Rust launcher prototype (Sprint 2)")]
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
    }

    Ok(())
}
