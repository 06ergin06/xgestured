# xGestured

A lightweight, minimal, and lightning-fast touchpad gesture daemon specifically built for **X11** desktop environments (it works on Wayland too).

## ✨ Features

* **Built for X11:** Perfect for Window Managers (i3, bspwm, Awesome) and traditional Desktop Environments (KDE X11, XFCE, Mate) where native gesture support is missing.
* **Pure C & Minimal Overhead:** Written strictly in C with zero bloated dependencies.
* **Zero Zombie Processes:** Gracefully handles child process execution natively.
* **Instant Config Reloading:** Uses `inotify` to automatically detect changes to the configuration file. No need to restart the daemon.

## 📦 Dependencies

`xgestured` requires only the essential hardware input libraries:
* `libinput`
* `libudev`

**For Arch Linux / CachyOS:**
```bash
sudo pacman -S libinput
```

**For Debian / Ubuntu:**
```bash
sudo apt install libinput-dev libudev-dev
```

**For Fedora:**
```bash
sudo dnf install libinput-devel systemd-devel
```
## 🚀 Installation
### Method 1 : AUR
```bash
paru -S xgestured-git
# or 
yay -S xgestured-git
```
```bash
sudo usermod -aG input $USER
```
**CRITICAL STEP:** You **MUST** log out and log back in (or reboot your computer).

### Method 2 : Manual
1. Clone the repository:
```bash
git clone https://github.com/06ergin06/xgestured.git
cd xgestured
```

2. Build and install the daemon (**DO NOT run with sudo**):
```bash
make
make install
```
*(The script will ask for your sudo password once to place the config file in `/etc/` and add you to the `input` group.)*

**CRITICAL STEP:** You **MUST** log out and log back in (or reboot your computer).

## ⚙️ Configuration

The configuration file is located at `/etc/xgestured/config.ini`. You can define shell commands for 3-finger and 4-finger swipes. 

**Default config for KDE KWin**

*Any changes made to this file are applied immediately in real-time.*

## 🗑️ Uninstallation

To completely remove `xgestured` and its configuration from your system:
```bash
make uninstall
```

## 📜 License

This project is licensed under the GPLv3 License - see the [LICENSE](LICENSE) file for details. 

*The INI parser (`ini.c`, `ini.h`) included in this project is developed by Ben Hoyt and is licensed under the BSD 3-Clause License.*