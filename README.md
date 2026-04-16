# xGestured

A lightweight, minimal, and lightning-fast touchpad gesture daemon for Linux.

`xgestured` reads raw touchpad input events directly from the hardware and executes user-defined shell commands based on multi-finger swipes. It is designed to be extremely resource-efficient, running silently in the background as a native system daemon.

## Features

* **Pure C & Minimal Overhead:** Written strictly in C with zero bloated dependencies.
* **Zero Zombie Processes:** Gracefully handles child process execution natively.
* **Instant Config Reloading:** Uses `inotify` to automatically detect changes to the configuration file. No need to restart the daemon or send `SIGHUP` signals manually.
* **Native Systemd Integration:** Installs and runs as a standard background service without requiring manual `sudo` execution by the user.
* **Desktop Environment Agnostic:** Works perfectly on X11 and can trigger commands on any Window Manager or DE (KDE, GNOME, Window Maker, etc.).

## Dependencies

`xgestured` requires only the essential hardware input libraries:
* `libinput`
* `libudev`

**For Arch Linux / CachyOS:**
```bash
sudo pacman -S libinput systemd
```
**For Debian / Ubuntu:**
```bash
sudo apt install libinput-dev libudev-dev
```

## Installation

1. Clone repository
```bash
git clone [https://github.com/06ergin06/xgestured.git](https://github.com/06ergin06/xgestured.git)
cd xgestured
```
2. Build and install the daemon:
```bash
make
sudo make install
```
*Note: The make install command will automatically copy the binary, create the configuration directory, and enable the systemd service.*

## Configuration

The configuration file is located at `/etc/xgestured/config.ini`. You can define shell commands for 3-finger and 4-finger swipes.
**Default config for Kde.**
*Any changes made to this file are applied immediately in real-time.*

## Uninstallation

To completely remove xgestured and its configuration from your system:
```bash
sudo make uninstall
```

## License

This project is licensed under the GPLv3 License - see the [LICENSE](LICENSE) file for details.

The INI parser (ini.c, ini.h) included in this project is developed by Ben Hoyt and is licensed under the BSD 3-Clause License.