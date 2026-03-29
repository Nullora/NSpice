# NSpice
Auto update spicetify so you don't have to worry about it!

# Details
NSpice checks for new updates every 2 hours, ensuring you get nonstop music with 0 interruptions!

# Windows install
```bash
winget # if you see: Windows Package Manager v1.28.220  at the far top, proceed.
winget install --id Git.Git -e --source winget # if you don't have git installed
git clone https://github.com/Nullora/NSpice # clone repo
winget install GnuWin32.Make # if you dont have make installed
cd NSpice
make r # to run the already existring binary file
make # to build the binary from scratch
# close with system tray button in your toolbar
```
# Debian/Arch install
```bash
git clone https://github.com/Nullora/NSpice # clone repo
cd NSpice # change dir
make install-deps # install dependencies
make # build files
make r
# close with system tray button in your toolbar
```
