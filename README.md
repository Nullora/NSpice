# NSpice
Auto update spicetify so you don't have to worry about it!

# Details
NSpice checks for spicetify new updates every 2 hours, ensuring you get nonstop music with 0 interruptions!
- Extremely lightweight: Only ~500kb of ram used up, along with a small 800kb executable.
- Silent: You will forget you ever have it running until it notifies you that it updated something!
- Reliable: This has been tested on my personal computer for over a week now. Longest it ever ran was 48 hours, 0 memory leaks and never broke once.


# Windows install
## Note
This installation process does not include installing minGw or gcc, as i literally dont know how i did it myself. im not the best on windows
I know... :(
## To build the binary yourself (complex if you dont have everything preinstalled)
```bash
winget # if you see: Windows Package Manager v1.28.220  at the far top, proceed.

winget install --id Git.Git -e --source winget # if you don't have git installed

git clone https://github.com/Nullora/NSpice # clone repo

# you have to add this manually to path, just google how
winget install GnuWin32.Make # if you dont have make installed

cd NSpice

make # to build the binary file

make r # to run binary

# close with system tray button in your toolbar
```
## To run the prebuilt executable (easiest)
You can either:
- Install the **prebuilt_binaries_win** folder directly and just double click the executable
- Or you can clone the whole repo (overkill it if youre just looking for the executable) and running
```bash
.\prebuilt_binaries_win\spice.exe
#or
.\build\spice.exe 
```
Both of these do literally the same thing. If you run them both you will just have 2 NSpice instances running at the same time, which usually gets the program rate limited
because it's requesting the same copy of the json twice from the same webpage.
## NOTE
This program adds itself to startup once turned on, you can't turned this off easily unless you remove the
```cpp
addToStartup(); //line 68
```
and rebuild the binary. doing this requires you having the compiler installed and make installed.
If you want an option to stop itself from adding to startup, please lmk!
If you are gonna do that, go back to the steps above and follow them alone.

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
install gcc with:
```bash
sudo apt install build-essential
# or
sudo pacman -S gcc gcc-fortran
```
The linux version does not have a prebuilt binary, as I assume anyone using Linux can correctly build and run and add a binary to startup on their own!

# Updates
You can find patch notes in **details/patch_notes.md**. Not much gets changed but when it does change, it's always documented in that file.
