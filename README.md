# NSpice
Auto update spicetify so you don't have to worry about it!

# Details
NSpice checks for new updates every 30 minutes, ensuring you get nonstop music with 0 interruptions!

# Installation and usage
<ins>**1. Downloading the repository:**</ins>
- Start by cloning the repository with `git clone https://github.com/Nullora/NSpice`.
<ins>**2. Installing the dependencies:**</ins>
- Before using the tool, you must install all dependencies:
- Install for [windows](https://github.com/Nullora/NSpice#windows-install).
- Install for [debian](https://github.com/Nullora/NSpice#debian-install).

# Windows install
```bash
winget # if you see: Windows Package Manager v1.28.220  at the far top, proceed.
winget install --id Git.Git -e --source winget # if you don't have git installed
winget install GnuWin32.Make # if you dont have make installed
make
make r
# close with system tray button at the bottom
```
# Debian install
```bash
make install-d # install dependencies
make # build files
make r # run tool in background (look at your toolbar for the icon to pop up, you can close it from there.) 
```