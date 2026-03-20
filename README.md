# Dynamic Virtual Machine Manager
A tray application for quick management of your dynamic virtual machine + looking glass setup.

> [!WARNING]
> This app is basically provided as-is, and you would probably want to modify it to your needs, especially since it assumes NVIDIA GPU. I mainly wrote it for myself.

## Setup
1. Install the dependencies:
```bash
sudo pacman -S gtk3 libayatana-appindicator
```
2. Build the project using CMake
```bash
git clone https://github.com/SocketByte/dvmm.git
cd dvmm
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
6. Create an autostart script.
### For KDE
```bash
# autostart-dvmm.sh
sleep 3
/usr/bin/dvmm
```
```bash
sudo chmod +x ./autostart-dvmm.sh
```
Add it through the KDE autostart settings GUI as a new autostart script.

## Requirements
- Requires `gpu-check` installed globally.
```bash
#!/bin/bash

# GPU PCIe address
GPU="01:00.0"

driver=$(lspci -nnk -s "$GPU" | awk -F': ' '/Kernel driver in use/ {print $2}')

if [[ -z "$driver" ]]; then
    echo "Could not find used driver"
    lspci -nnk -s "$GPU"
elif [[ "$driver" == "nvidia" ]]; then
    echo "Driver in use: nvidia"
elif [[ "$driver" == "vfio-pci" ]]; then
    echo "Driver in use: vfio-pci"
else
    echo "Unknown driver: $driver"
fi
```