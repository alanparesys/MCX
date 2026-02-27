use anyhow::{anyhow, Result};
use flate2::read::GzDecoder;
use std::fs::{self, File};
use std::io;
use std::path::{Path, PathBuf};
use tar::Archive;
use zip::read::ZipArchive;

pub fn extract_archive(archive_path: &Path, output_dir: &Path) -> Result<()> {
    let name = archive_path
        .file_name()
        .and_then(|n| n.to_str())
        .ok_or_else(|| anyhow!("Invalid archive name"))?;

    fs::create_dir_all(output_dir)?;

    if name.ends_with(".zip") {
        extract_zip(archive_path, output_dir)?;
        return Ok(());
    }

    if name.ends_with(".tar.gz") || name.ends_with(".tgz") {
        extract_targz(archive_path, output_dir)?;
        return Ok(());
    }

    Err(anyhow!("Unsupported archive type: {name}"))
}

fn extract_zip(path: &Path, out: &Path) -> Result<()> {
    let file = File::open(path)?;
    let mut zip = ZipArchive::new(file)?;

    for i in 0..zip.len() {
        let mut entry = zip.by_index(i)?;
        let enclosed = entry
            .enclosed_name()
            .ok_or_else(|| anyhow!("Invalid zip entry path"))?
            .to_path_buf();

        let out_path = out.join(enclosed);
        if entry.is_dir() {
            fs::create_dir_all(&out_path)?;
            continue;
        }

        if let Some(parent) = out_path.parent() {
            fs::create_dir_all(parent)?;
        }

        let mut out_file = File::create(&out_path)?;
        io::copy(&mut entry, &mut out_file)?;
    }

    Ok(())
}

fn extract_targz(path: &Path, out: &Path) -> Result<()> {
    let tar_gz = File::open(path)?;
    let decoder = GzDecoder::new(tar_gz);
    let mut archive = Archive::new(decoder);
    archive.unpack(out)?;
    Ok(())
}

pub fn locate_executable(root: &Path) -> Option<PathBuf> {
    #[cfg(target_os = "windows")]
    let candidates = ["mcx.exe", "release-pro/mcx.exe", "mcx-windows-x64/mcx.exe"];

    #[cfg(not(target_os = "windows"))]
    let candidates = ["mcx", "release-pro/mcx", "mcx-linux-x64/mcx"];

    for c in candidates {
        let p = root.join(c);
        if p.exists() {
            return Some(p);
        }
    }

    None
}
