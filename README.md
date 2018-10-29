# csgo-webradar-client
A shitty web-based radar client for Counter-Strike: Global Offensive

## Compiling

### Build requirements

Specific distro instructions for build requirements:

*Arch Linux*:
```bash
sudo pacman -S cmake git
```

*Debian/Ubuntu/Mint*:
```bash
sudo apt-get install cmake build-essential git
```

### Clone the repository
```bash
git clone https://github.com/w3bn00b/csgo-webradar-client.git
```

### Build the project
```bash
cd csgo-webradar-client
./build.sh
```

## Running
In order to run this, you MUST use sudo/su due to the memory functions.  Once you've compiled the project, you can run it from the build directory:
```bash
./run.sh
```

### Update the project
```bash
./update.sh
```