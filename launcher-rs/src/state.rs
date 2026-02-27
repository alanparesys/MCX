use anyhow::Result;
use serde::{Deserialize, Serialize};
use std::fs;
use std::path::Path;

#[derive(Debug, Clone, Serialize, Deserialize, Default)]
pub struct LauncherState {
    pub installed_version: Option<String>,
    pub executable_path: Option<String>,
}

pub fn load_state(path: &Path) -> Result<LauncherState> {
    if !path.exists() {
        return Ok(LauncherState::default());
    }
    let raw = fs::read_to_string(path)?;
    Ok(serde_json::from_str(&raw)?)
}

pub fn save_state(path: &Path, state: &LauncherState) -> Result<()> {
    if let Some(parent) = path.parent() {
        fs::create_dir_all(parent)?;
    }
    let content = serde_json::to_string_pretty(state)?;
    fs::write(path, content)?;
    Ok(())
}
