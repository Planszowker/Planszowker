# Planszowker
[![CMake Build with SFML](https://github.com/Ishiguro96/Planszowker/actions/workflows/cmake.yml/badge.svg)](https://github.com/Ishiguro96/Planszowker/actions/workflows/cmake.yml)

## Building
### Prerequirements - Linux
Building on Linux depends on few libs. To install them, simply use default package manager.

Libs to install:
```
gcc g++ make cmake curl zip unzip tar pkg-config autoconf libtool libx11-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev libgl1-mesa-dev ninja-build
```
### VCPKG
Planszówker uses `VCPKG` as a package manager. Before building project itself, one must install all libraries that Planszówker depends on.

First, configure `VCPKG` by running command:
```
./vcpkg/bootstrap-vcpkg.sh
```

Then, install all required libs by running
```
./vcpkg/vcpkg install
```

Incorporate changes made by `VCPKG` by running:
```
./vcpkg/vcpkg integrate install
```

Also, you may be interested in setting `VCPKG_ROOT` env variable to tell CMake where `VCPKG`'s toolchain file is located.

### Building with Ninja
#### Configure
```sh
cmake -B ../build -S . --preset=default
```
This will configure cmake to the `build` directory outside of sources.

#### Build
```sh
cmake --build ../build
```
