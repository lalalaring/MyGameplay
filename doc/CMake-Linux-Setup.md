## Install Linux (64-bit)
- Ubuntu 14.04
- Other distributions have been known to be working.

## Install Linux Dependencies

```bash
sudo apt-get install build-essential gcc cmake libglu1-mesa-dev libogg-dev libopenal-dev libgtk2.0-dev curl libpcrecpp0:i386 lib32z1-dev
```
## Install External Dependencies
Run the following command-line from the top-level directory of your GamePlay repository:

    ./install.sh

## Building
Run the following from the top-level directory of your GamePlay repository:
```
mkdir build
cd build
cmake ..
make
```

## Running a sample (Ex. sample-browser)
Run the following from the top-level directory of your GamePlay repository:
```
cd build/sample/browser
./sample-browser
```