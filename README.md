# NSpice
Auto update spicetify so you don't have to worry about it!

# Details
NSpice checks for new updates every 2 hours, ensuring you get nonstop music with 0 interruptions!

# Windows install
Open a terminal and run:
```bash
winget # if you see: Windows Package Manager v1.28.220  at the far top, proceed.

winget install --id Git.Git -e --source winget # if you don't have git installed

git clone https://github.com/Nullora/NSpice # clone repo

winget install GnuWin32.Make # if you dont have make installed

cd NSpice

make # to build the binary file

make r # to run binary

# close with system tray button in your toolbar
```

# Debian/Arch install
Run in terminal:
```bash
git clone https://github.com/Nullora/NSpice # clone repo
cd NSpice # change dir
make install-deps # install dependencies
make # to build the binary file
make r # to run binary
# close with system tray button in your toolbar
```
```bash
make compdb # to fix the squiggly lines if you use vscodium
```
install gcc with:
```bash
sudo apt install build-essential
# or
sudo pacman -S gcc gcc-fortran
```
