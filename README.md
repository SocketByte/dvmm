# Dynamic Virtual Machine Manager
A tray application for quick management of your dynamic virtual machine + looking glass setup.

> [!WARNING]
> This app is basically provided as-is, and you would probably want to modify it to your needs. I mainly wrote it for myself.

## Setup
1. Install the dependencies:
```bash
sudo pacman -S gtk3 libayatana-appindicator
```
2. Build the project using CMake
```bash
mkdir -p build
cd build
cmake ..
make
```
3. Create two icons (`vfio.png` and `nvidia.png`) at `~/.config/dvmm`
4. Ensure your virtual machine is named `win11`
5. Install the application to `/usr/bin`:
```bash
sudo cp dvmm /usr/bin/dvmm
```
6. Create a systemd service:
```bash
# /etc/systemd/system/dvmm.service
[Unit]
Description=Dynamic VM Manager Tray App
After=graphical.target

[Service]
Type=simple

User=YOUR_USERNAME
Group=YOUR_USERNAME

Environment=DISPLAY=:0
Environment=XDG_RUNTIME_DIR=/run/user/1000
Environment=DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus

ExecStart=/usr/bin/dvmm
Restart=always
RestartSec=3

[Install]
WantedBy=graphical.target
```
```bash
sudo systemctl enable dvmm
sudo systemctl start dvmm
```

## Requirements
- Requires `gpu-check` installed globally.