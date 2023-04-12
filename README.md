# FinUI

FinUI is a fork of [MinUI](https://github.com/shauninman/union-minui). MinUI is a
focused custom launcher for the Anbernic RG35XX. FinUI adds additional features on
top of MinUI.

![MinUI](./github/minui-menu-gbc.png)

## MinUI features

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

You can [grab the latest version here](https://github.com/shauninman/union-minui/releases).

## FinUI additional features

-

## Installation guide

The following section explains how to install MinUI using macOS, without having to
download any additional tools.

### Prerequisites

- A new 64GB microSD card from a reputable brand
- The offical Anbernic RG355XX firmware (`35XX-64GB230309EN.IMG`)
- A [MinUI release](https://github.com/shauninman/union-minui/releases)
- An [administrator user account](https://support.apple.com/guide/mac-help/change-users-groups-settings-mtusr001/mac)
- [Full Disk Access](https://support.apple.com/guide/mac-help/change-privacy-security-settings-on-mac-mchl211c911f/13.0/mac/13.0)
for the app `Terminal.app`

### Install MinUI

1. Connect your microSD card to your Mac
2. Start the app `Terminal`
3. Run the command `diskutil list` and identify your microSD by looking at the `SIZE`
column. It is `/dev/disk4` in this example.
4. Run the command `diskutil unmountDisk /dev/disk4`
5. Unzip the RG35XX firmware to your desktop and then run the command `cd ~/Desktop`
in the Terminal
6. Run the command `sudo dd bs=1m if=35XX-64GB230309EN.IMG of=/dev/disk4` to flash
the firmware onto your microSD. Enter your user account password if prompted. Wait.
7. Run the command `diskutil list` again and identify the largest partition on your
microSD by looking at the `SIZE` column. It is `/dev/disk4s4` in this example. Note
that it differs from the previous path!
8. Run `sudo diskutil eraseVolume FAT32 ROMS /dev/disk4s4` to format that partition
as `FAT32` with the name `ROMS`. Enter your user account password if prompted. Wait.
9. You should now have a `misc` and a `ROMS` disk connected to your Mac. If you
don't then try reconnecting your microSD card to your Mac.
10. Unzip the MinUI release to your desktop
11. Copy `dmenu.bin` to the root of the `misc` disk. Copy `MinUI.zip` and the `Bios`,
`Roms`, `Saves` folders to the root of the `ROMS` disk. Optionally, follow the additional
instructions in the MinUI README if you wish.
12. Finally, run `dot_clean -m /Volumes/misc` and `dotclean -m /Volumes/ROMS` to
remove any dot-underscore files created by macOS on your microSD.

Done. Insert your microSD to your RG35XX and boot it up!

## FinUI TODO

- Add "mark as finished" OR "add as favorite" to menu
- Improve battery capacity readings (2100 mAh, 2600 mAh, 3500 mAh)
- Refactor Tools to Settings
- Add Clear Recent setting: <https://github.com/dingmah/MiniUI-Tool-Clear-Recently-Played-List>
- Adjust recently played count
- Adjust overclocking
- Include extras in release by default
- Move toolchain to repo:  <https://github.com/shauninman/union-rg35xx-toolchain>
- Release using GitHub action: <https://github.com/JoeStaff/devilutionX/commit/a0fe502e70767ca8e2921d5580d4d4dec9e15cc1>
- Automate installation guide

## Disclaimer

This software is provided without warranty of any kind. In no event shall the authors
or contributors be liable for any claim, damages, or other liability, arising from,
out of, or in connection with this software, the use of this software, or other
dealings with this software.

Use at your own risk.
