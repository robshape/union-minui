#!/bin/bash
set -euo pipefail

DISK_PATH="/dev/disk2"
FIRMWARE_NAME="35XX-64GB230309EN.IMG"
RELEASE_NAME="FinUI-*-0-full.zip"
WORKING_DIRECTORY="./releases/"

echo "[*] PREPARING INSTALLATION"
cd $WORKING_DIRECTORY
echo

echo "[*] INSTALLING FIRMWARE"
diskutil unmountDisk $DISK_PATH
sleep 3

sudo dd bs=1m if=$FIRMWARE_NAME of=$DISK_PATH
sleep 3

sudo diskutil eraseVolume FAT32 ROMS ${DISK_PATH}s4
sleep 3
echo

echo "[*] INSTALLING FinUI"
unzip -o ${RELEASE_NAME[0]}

cp -R dmenu.bin /Volumes/misc/
cp -R MinUI.zip /Volumes/ROMS/

cp -R Bios/ /Volumes/ROMS/Bios
cp -R Emus/ /Volumes/ROMS/Emus
cp -R Roms/ /Volumes/ROMS/Roms
cp -R Saves/ /Volumes/ROMS/Saves
cp -R Tools/ /Volumes/ROMS/Tools
echo

echo "[*] CLEANING FILES"
dot_clean -m /Volumes/misc
rm -fr /Volumes/misc/.fseventsd /Volumes/misc/.Spotlight-V100 /Volumes/misc/.Trashes /Volumes/misc/._.Trashes
mkdir /Volumes/misc/.fseventsd
touch /Volumes/misc/.fseventsd/no_log /Volumes/misc/.metadata_never_index /Volumes/misc/.Trashes

dot_clean -m /Volumes/ROMS
rm -fr /Volumes/ROMS/.fseventsd /Volumes/ROMS/.Spotlight-V100 /Volumes/ROMS/.Trashes /Volumes/ROMS/._.Trashes
mkdir /Volumes/ROMS/.fseventsd
touch /Volumes/ROMS/.fseventsd/no_log /Volumes/ROMS/.metadata_never_index /Volumes/ROMS/.Trashes
echo
