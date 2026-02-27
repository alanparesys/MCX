use serde::Deserialize;

#[derive(Debug, Deserialize)]
pub struct Release {
    pub tag_name: String,
    pub html_url: String,
    pub assets: Vec<Asset>,
}

#[derive(Debug, Deserialize)]
pub struct Asset {
    pub name: String,
    pub browser_download_url: String,
}

pub fn pick_asset_for_platform(assets: &[Asset]) -> Option<&Asset> {
    #[cfg(target_os = "windows")]
    {
        assets
            .iter()
            .find(|a| a.name.contains("windows") && a.name.ends_with(".zip"))
            .or_else(|| assets.first())
    }

    #[cfg(target_os = "linux")]
    {
        assets
            .iter()
            .find(|a| {
                a.name.contains("linux")
                    && (a.name.ends_with(".tar.gz") || a.name.ends_with(".tgz"))
            })
            .or_else(|| assets.first())
    }

    #[cfg(not(any(target_os = "windows", target_os = "linux")))]
    {
        assets.first()
    }
}

pub fn is_newer_version(latest: &str, current: &str) -> bool {
    fn parse(v: &str) -> Vec<i32> {
        v.trim_start_matches(['v', 'V'])
            .split('.')
            .map(|p| p.parse::<i32>().unwrap_or(0))
            .collect()
    }

    let mut a = parse(latest);
    let mut b = parse(current);
    let n = a.len().max(b.len());
    a.resize(n, 0);
    b.resize(n, 0);

    for i in 0..n {
        if a[i] > b[i] {
            return true;
        }
        if a[i] < b[i] {
            return false;
        }
    }
    false
}
