# BL-Bridge-Host-Manifest

The library that reads the manifest of a module, and states the module for a host of Bridge.

## Background

A module that runs on Bridge states its manifest in its file. A host reads it, and gives Bridge the config of the module.
This library holds that work, so each host of Bridge reads a manifest and states a module with the same rules.
Bridge-Host links it, and so can any other host.

## Install

Each release of the part `manifest` holds the archive `manifest.a` and the header `bridge_host_manifest.h` for each target.
The release has one zip, `BL-Bridge-Host-Manifest.zip`, with a folder for each target, for example `x86_64-linux-gnu`. Its digest is in `sha256.txt` of the release.
The header includes `bridge.h`, so the host also gets the release of Bridge that it links.

## Usage

Read the manifest of a file with `manifest_read`. Then give the keys of the module through a `manifest_host_t`, and call `manifest_state`.
`docs/extern/manifest/bridge_host_manifest.h.md` tells each type and each function.
