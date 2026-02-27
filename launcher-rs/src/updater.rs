use anyhow::{anyhow, Result};
use reqwest::Client;
use tokio::fs::File;
use tokio::io::AsyncWriteExt;

use crate::github::Release;

const LATEST_RELEASE_URL: &str = "https://api.github.com/repos/alanparesys/MCX/releases/latest";

pub async fn fetch_latest_release() -> Result<Release> {
    let client = Client::builder().user_agent("mcx-launcher-rs/0.1.0").build()?;

    let release = client
        .get(LATEST_RELEASE_URL)
        .send()
        .await?
        .error_for_status()?
        .json::<Release>()
        .await?;

    Ok(release)
}

pub async fn download_file(url: &str, output_path: &str) -> Result<()> {
    let client = Client::builder().user_agent("mcx-launcher-rs/0.1.0").build()?;

    let mut response = client.get(url).send().await?.error_for_status()?;
    let mut file = File::create(output_path).await?;

    while let Some(chunk) = response.chunk().await? {
        file.write_all(&chunk).await?;
    }

    file.flush().await?;
    Ok(())
}

pub fn require_asset_url(url: Option<&str>) -> Result<&str> {
    url.ok_or_else(|| anyhow!("No suitable release asset found for this platform"))
}
