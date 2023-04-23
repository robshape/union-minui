# FinUI

FinUI is a fork of [MinUI](https://github.com/shauninman/union-minui). MinUI is a
focused custom launcher for the Anbernic RG35XX. FinUI adds additional features on
top of MinUI.

![FinUI](./github/minui-menu-gbc.png)

## Features

### MinUI features

- Simple launcher, simple SD card
- No settings or configuration
- No boxart, themes, or distractions
- Automatically hides extension
  and region/version cruft in
  display names
- Consistent in-emulator menu with
  quick access to save states, disc
  changing, and emulator options
- Automatically sleeps after 30 seconds
  or press POWER to sleep (and wake)
- Automatically powers off while asleep
  after two minutes or hold POWER for
  one second
- Automatically resumes right where
  you left off if powered off while
  in-game, manually or while asleep
- Resume from manually created, last
  used save state by pressing X in
  the launcher instead of A
- Streamlined emulator frontend
  (minarch + libretro cores)

### Additional FinUI features

- Add to Favorites
- Clear Recently Played
- Adjusted battery readings for 2100, 2600 and 3500 mAh batteries
- All extras included in default installation
- Improved developer onboarding

### Shortcuts

| Description                  | Shortcut                     |
| ---------------------------- | ---------------------------- |
| Adjust brightness            | MENU + VOLUME_DOWN/VOLUME_UP |
| Add to Favorites (from menu) | SELECT                       |

## Development

### Prerequisites

- [Docker Desktop](https://docker.com/products/docker-desktop)

### Build project

Run `./start-toolchain.sh` and then `make all` in the Docker container shell.

### TODO

- ~~Add macOS CLI installation instructions~~
- ~~Move toolchain to repo~~
- ~~Include extras in release by default~~
- ~~Add Clear Recent setting~~
- ~~Refactor Tools to Settings~~
- ~~Add to Favorites~~
- ~~Adjust battery capacity readings (2100 mAh, 2600 mAh, 3500 mAh)~~
- ~~Update visible MinUI strings (show that it is the FinUI fork)~~
- Release using GitHub Actions
- Adjust overclocking and CPU usage (to maximize battery life)
- Improve battery capacity reading accuracy
- [...things in the old todo?...](./todo.txt)

## Disclaimer

This software is provided without warranty of any kind. In no event shall the authors
or contributors be liable for any claim, damages, or other liability, arising from,
out of, or in connection with this software, the use of this software, or other
dealings with this software.

Use at your own risk.

## Contributors

@robshape @guiburi
