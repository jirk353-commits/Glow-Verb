# Glow Verb by ROHIT JI

Algorithmic reverb VST3 for Windows.

- **9 environments:** Ambience, Room, Studio, Chamber, Plate, Hall, Cathedral, Arena, Vintage (80s)
- **Space:** pre-delay, decay (0.1-20 s), size, diffusion, damping, modulation, early reflections, width
- **Reverb EQ:** low cut, high cut, mid bell - with a live curve
- **Colour:** Warm / Tube / Tape saturation and an **Air** exciter
- **Sidechain ducking:** duck the reverb from its own input or from an external sidechain
- 14 presets (Vocal Plate, Bhakti Hall, Ducked Vocal Hall, Drum Room, Sidechain Pump Verb, ...)

## Download

Go to **[Releases](../../releases/latest)** and pick:

| File | For |
|---|---|
| **Glow-Verb-Setup-64bit.exe** | 64-bit FL Studio, Cubase and other modern DAWs (most people) |
| **Glow-Verb-Setup-32bit.exe** | Older 32-bit DAWs only |

Run it and click **Next > Install > Finish**.

## How it is built

Every push to `main` builds the plugin with MSVC (x64 and Win32) on GitHub Actions, packages each
into its own setup wizard (`Installer/setup.iss`, Inno Setup) and publishes them as the latest release.
