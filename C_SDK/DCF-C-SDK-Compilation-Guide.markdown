# DCF C SDK Compilation and Packaging Guide

**Version 5.0.0 | August 20, 2025**  
**Developed by DeMoD LLC**  
**Contact:** info@demodllc.example  
**License:** GNU General Public License v3.0 (GPL-3.0)  

This guide details how to compile executables and create packages for the DeMoD Communications Framework (DCF) C SDK on Linux, macOS, and Windows. It covers prerequisites, building executables (e.g., `p2p`, `master`), and packaging for distribution, ensuring compatibility with the AUTO mode, master node logic, and plugin system. The C SDK is located in the `c_sdk/` submodule of the DCF repository (https://github.com/ALH477/DeMoD-Communication-Framework).

## Table of Contents
- [Overview](#overview)
- [Prerequisites](#prerequisites)
  - [Linux](#linux)
  - [macOS](#macos)
  - [Windows](#windows)
- [Compilation Steps](#compilation-steps)
  - [Linux Compilation](#linux-compilation)
  - [macOS Compilation](#macos-compilation)
  - [Windows Compilation](#windows-compilation)
- [Packaging for Distribution](#packaging-for-distribution)
  - [Linux Packaging](#linux-packaging)
  - [macOS Packaging](#macos-packaging)
  - [Windows Packaging](#windows-packaging)
- [Testing Executables](#testing-executables)
- [Distribution Notes](#distribution-notes)
- [Troubleshooting](#troubleshooting)

## Overview
The DCF C SDK provides a modular, low-latency communication framework with support for client, server, P2P, AUTO, and master modes. It uses Protocol Buffers for serialization, gRPC for communication, and a plugin system for custom transports. The SDK is built using CMake, producing a static library (`libdcf_sdk.a`) and executables (`p2p`, `master`, `dcf`). This guide ensures cross-platform compatibility, leveraging CMake’s flexibility to generate executables and packages for Linux (Debian/Ubuntu, RPM-based), macOS (DMG), and Windows (ZIP, MSI).

Executables:
- `p2p`: Demonstrates P2P communication with RTT-based grouping.
- `master`: Implements master node logic for AUTO mode role assignment and network optimization.
- `dcf`: CLI/TUI interface for configuration, messaging, and master node commands.

Packages include the executables, library, headers, example configs, and plugins, ensuring GPL-3.0 compliance.

## Prerequisites
Ensure the repository is cloned with submodules:
```bash
git clone --recurse-submodules https://github.com/ALH477/DeMoD-Communication-Framework.git
cd DeMoD-Communication-Framework
```

### Linux
Supported distributions: Ubuntu 20.04+, Debian 10+, CentOS 8+, Fedora 34+.
- **Tools**: `gcc` or `clang`, `cmake` (3.16+), `make`, `protoc` (3.12+).
- **Dependencies**:
  ```bash
  sudo apt install -y build-essential cmake libprotobuf-c-dev libuuid-dev libdl-dev libcjson-dev libncurses-dev
  ```
  For RPM-based (CentOS/Fedora):
  ```bash
  sudo dnf install -y gcc make cmake protobuf-c-devel libuuid-devel cjson-devel ncurses-devel
  ```
- **gRPC**: Install `grpc` and `protobuf` for C++ wrapper:
  ```bash
  sudo apt install -y libgrpc-dev libgrpc++-dev protobuf-compiler-grpc
  ```
  RPM-based:
  ```bash
  sudo dnf install -y grpc-devel grpc-cpp protobuf-compiler-grpc
  ```
- **Optional**: `valgrind` for memory testing, `dpkg-dev` (Debian/Ubuntu) or `rpm-build` (CentOS/Fedora) for packaging.

### macOS
Supported: macOS 11 (Big Sur) or later.
- **Tools**: Xcode (Command Line Tools), `cmake` (3.16+), `protoc` (3.12+).
  ```bash
  xcode-select --install
  brew install cmake protobuf
  ```
- **Dependencies**:
  ```bash
  brew install protobuf-c libuuid cjson ncurses
  ```
- **gRPC**:
  ```bash
  brew install grpc
  ```
- **Optional**: `valgrind` (via Homebrew, limited support), `hdiutil` for DMG packaging (included in macOS).

### Windows
Supported: Windows 10/11 (64-bit).
- **Tools**: Visual Studio 2019/2022 (Community Edition), CMake (3.16+), MSYS2 (for Unix-like environment).
  - Install Visual Studio with C++ Desktop Development workload.
  - Install CMake via https://cmake.org/download or MSYS2:
    ```bash
    pacman -S mingw-w64-x86_64-cmake
    ```
- **Dependencies** (via MSYS2):
  ```bash
  pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-protobuf-c mingw-w64-x86_64-libuuid mingw-w64-x86_64-cjson mingw-w64-x86_64-ncurses
  ```
- **gRPC**:
  ```bash
  pacman -S mingw-w64-x86_64-grpc
  ```
- **Optional**: WiX Toolset (3.11+) for MSI packaging, `7zip` for ZIP.

## Compilation Steps
The C SDK uses CMake to generate build files. The process builds `libdcf_sdk.a` and executables (`p2p`, `master`, `dcf`). Navigate to `c_sdk/`:
```bash
cd c_sdk
mkdir build
cd build
```

### Linux Compilation
1. **Generate Build Files**:
   ```bash
   cmake .. -DCMAKE_BUILD_TYPE=Release
   ```
   - Use `-DCMAKE_C_COMPILER=clang` to use Clang instead of GCC.
2. **Compile**:
   ```bash
   make -j$(nproc)
   ```
3. **Output**:
   - Library: `build/libdcf_sdk.a`
   - Executables: `build/p2p`, `build/master`, `build/dcf`
4. **Install (Optional)**:
   ```bash
   sudo make install
   ```
   Installs to `/usr/local/lib` (library), `/usr/local/include/dcf_sdk` (headers), `/usr/local/bin` (executables).

### macOS Compilation
1. **Generate Build Files**:
   ```bash
   cmake .. -DCMAKE_BUILD_TYPE=Release
   ```
   - Ensure Homebrew paths are in `CMAKE_PREFIX_PATH` if needed:
     ```bash
     cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/usr/local/opt
     ```
2. **Compile**:
   ```bash
   make -j$(sysctl -n hw.ncpu)
   ```
3. **Output**:
   - Library: `build/libdcf_sdk.a`
   - Executables: `build/p2p`, `build/master`, `build/dcf`
4. **Install (Optional)**:
   ```bash
   sudo make install
   ```
   Installs to `/usr/local/lib`, `/usr/local/include/dcf_sdk`, `/usr/local/bin`.

### Windows Compilation
1. **Set Up MSYS2**:
   - Open MSYS2 MinGW 64-bit shell (`mingw64.exe`).
2. **Generate Build Files**:
   ```bash
   cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
   ```
   - Alternatively, use Visual Studio:
     ```bash
     cmake .. -G "Visual Studio 16 2019" -A x64
     ```
3. **Compile**:
   - MinGW:
     ```bash
     mingw32-make -j$(nproc)
     ```
   - Visual Studio (open `dcf_sdk.sln` in Visual Studio, set Release/x64, build).
4. **Output**:
   - Library: `build/libdcf_sdk.a` (MinGW) or `build/Release/dcf_sdk.lib` (VS)
   - Executables: `build/p2p.exe`, `build/master.exe`, `build/dcf.exe`
5. **Install (Optional)**:
   ```bash
   mingw32-make install
   ```
   Installs to `C:\Program Files\dcf_sdk` (adjust with `-DCMAKE_INSTALL_PREFIX`).

## Packaging for Distribution
Packages include `libdcf_sdk.a`, headers (`include/dcf_sdk/`), executables, `config.json.example`, and example plugins (`plugins/custom_transport.c`). Ensure GPL-3.0 license is included.

### Linux Packaging
#### Debian/Ubuntu (.deb)
1. **Install Tools**:
   ```bash
   sudo apt install -y dpkg-dev
   ```
2. **Create Package Structure**:
   ```bash
   mkdir -p dcf-sdk_5.0.0-1_amd64/DEBIAN
   mkdir -p dcf-sdk_5.0.0-1_amd64/usr/local/lib
   mkdir -p dcf-sdk_5.0.0-1_amd64/usr/local/include/dcf_sdk
   mkdir -p dcf-sdk_5.0.0-1_amd64/usr/local/bin
   mkdir -p dcf-sdk_5.0.0-1_amd64/usr/local/share/dcf_sdk
   ```
3. **Copy Files**:
   ```bash
   cp build/libdcf_sdk.a dcf-sdk_5.0.0-1_amd64/usr/local/lib/
   cp include/dcf_sdk/*.h dcf-sdk_5.0.0-1_amd64/usr/local/include/dcf_sdk/
   cp build/p2p build/master build/dcf dcf-sdk_5.0.0-1_amd64/usr/local/bin/
   cp config.json.example plugins/custom_transport.c dcf-sdk_5.0.0-1_amd64/usr/local/share/dcf_sdk/
   cp ../LICENSE dcf-sdk_5.0.0-1_amd64/usr/local/share/dcf_sdk/
   ```
4. **Create Control File** (`dcf-sdk_5.0.0-1_amd64/DEBIAN/control`):
   ```text
   Package: dcf-sdk
   Version: 5.0.0-1
   Architecture: amd64
   Maintainer: DeMoD LLC <info@demodllc.example>
   Depends: libprotobuf-c1, libuuid1, libcjson1, libncurses6
   Description: DeMoD Communications Framework C SDK
    A modular, low-latency communication framework with AUTO mode and master node logic.
   License: GPL-3.0
   ```
5. **Build Package**:
   ```bash
   dpkg-deb --build dcf-sdk_5.0.0-1_amd64
   ```
6. **Output**: `dcf-sdk_5.0.0-1_amd64.deb`

#### RPM-based (CentOS/Fedora)
1. **Install Tools**:
   ```bash
   sudo dnf install -y rpm-build
   ```
2. **Create Spec File** (`dcf-sdk.spec`):
   ```spec
   Name: dcf-sdk
   Version: 5.0.0
   Release: 1
   Summary: DeMoD Communications Framework C SDK
   License: GPL-3.0
   Requires: protobuf-c libuuid cjson ncurses-libs
   %description
   A modular, low-latency communication framework with AUTO mode and master node logic.
   %prep
   %setup -q -n dcf-sdk
   %build
   %install
   mkdir -p %{buildroot}/usr/local/lib
   mkdir -p %{buildroot}/usr/local/include/dcf_sdk
   mkdir -p %{buildroot}/usr/local/bin
   mkdir -p %{buildroot}/usr/local/share/dcf_sdk
   cp libdcf_sdk.a %{buildroot}/usr/local/lib/
   cp include/dcf_sdk/*.h %{buildroot}/usr/local/include/dcf_sdk/
   cp p2p master dcf %{buildroot}/usr/local/bin/
   cp config.json.example plugins/custom_transport.c LICENSE %{buildroot}/usr/local/share/dcf_sdk/
   %files
   /usr/local/lib/libdcf_sdk.a
   /usr/local/include/dcf_sdk/*
   /usr/local/bin/p2p
   /usr/local/bin/master
   /usr/local/bin/dcf
   /usr/local/share/dcf_sdk/*
   ```
3. **Copy Files to RPM Build Area**:
   ```bash
   mkdir -p ~/rpmbuild/SOURCES/dcf-sdk
   cp -r build/libdcf_sdk.a include/dcf_sdk build/p2p build/master build/dcf config.json.example plugins/custom_transport.c ../LICENSE ~/rpmbuild/SOURCES/dcf-sdk/
   ```
4. **Build RPM**:
   ```bash
   rpmbuild -ba dcf-sdk.spec
   ```
5. **Output**: `~/rpmbuild/RPMS/x86_64/dcf-sdk-5.0.0-1.x86_64.rpm`

### macOS Packaging
#### DMG Package
1. **Create Directory Structure**:
   ```bash
   mkdir -p dcf-sdk-5.0.0
   mkdir -p dcf-sdk-5.0.0/lib
   mkdir -p dcf-sdk-5.0.0/include/dcf_sdk
   mkdir -p dcf-sdk-5.0.0/bin
   mkdir -p dcf-sdk-5.0.0/share/dcf_sdk
   ```
2. **Copy Files**:
   ```bash
   cp build/libdcf_sdk.a dcf-sdk-5.0.0/lib/
   cp include/dcf_sdk/*.h dcf-sdk-5.0.0/include/dcf_sdk/
   cp build/p2p build/master build/dcf dcf-sdk-5.0.0/bin/
   cp config.json.example plugins/custom_transport.c ../LICENSE dcf-sdk-5.0.0/share/dcf_sdk/
   ```
3. **Create DMG**:
   ```bash
   hdiutil create -srcfolder dcf-sdk-5.0.0 -volname "DCF SDK 5.0.0" dcf-sdk-5.0.0.dmg
   ```
4. **Output**: `dcf-sdk-5.0.0.dmg`

### Windows Packaging
#### ZIP Package
1. **Create Directory Structure**:
   ```bash
   mkdir -p dcf-sdk-5.0.0
   mkdir -p dcf-sdk-5.0.0/lib
   mkdir -p dcf-sdk-5.0.0/include/dcf_sdk
   mkdir -p dcf-sdk-5.0.0/bin
   mkdir -p dcf-sdk-5.0.0/share/dcf_sdk
   ```
2. **Copy Files**:
   ```bash
   cp build/libdcf_sdk.a build/p2p.exe build/master.exe build/dcf.exe dcf-sdk-5.0.0/bin/
   cp include/dcf_sdk/*.h dcf-sdk-5.0.0/include/dcf_sdk/
   cp config.json.example plugins/custom_transport.c ../LICENSE dcf-sdk-5.0.0/share/dcf_sdk/
   ```
3. **Create ZIP**:
   ```bash
   7z a dcf-sdk-5.0.0.zip dcf-sdk-5.0.0
   ```
4. **Output**: `dcf-sdk-5.0.0.zip`

#### MSI Package (Optional)
1. **Install WiX Toolset**: Download from https://wixtoolset.org.
2. **Create WiX Source** (`dcf-sdk.wxs`):
   ```xml
   <?xml version="1.0" encoding="UTF-8"?>
   <Wix xmlns="http://schemas.microsoft.com/wix/2006/wi">
       <Product Id="*" Name="DCF SDK" Language="1033" Version="5.0.0" Manufacturer="DeMoD LLC" UpgradeCode="YOUR-GUID-HERE">
           <Package InstallerVersion="200" Compressed="yes" InstallScope="perMachine"/>
           <Directory Id="TARGETDIR" Name="SourceDir">
               <Directory Id="ProgramFiles64Folder">
                   <Directory Id="INSTALLDIR" Name="DCF SDK">
                       <Directory Id="LIBDIR" Name="lib"/>
                       <Directory Id="INCLUDEDIR" Name="include/dcf_sdk"/>
                       <Directory Id="BINDIR" Name="bin"/>
                       <Directory Id="SHAREDIR" Name="share/dcf_sdk"/>
                   </Directory>
               </Directory>
           </Directory>
           <DirectoryRef Id="LIBDIR">
               <Component Id="LibComponent" Guid="*">
                   <File Id="LibFile" Source="build/libdcf_sdk.a"/>
               </Component>
           </DirectoryRef>
           <DirectoryRef Id="INCLUDEDIR">
               <Component Id="IncludeComponent" Guid="*">
                   <File Id="ClientH" Source="include/dcf_sdk/dcf_client.h"/>
                   <File Id="ErrorH" Source="include/dcf_sdk/dcf_error.h"/>
                   <File Id="MasterH" Source="include/dcf_sdk/dcf_master.h"/>
                   <!-- Add other headers -->
               </Component>
           </DirectoryRef>
           <DirectoryRef Id="BINDIR">
               <Component Id="BinComponent" Guid="*">
                   <File Id="P2PExe" Source="build/p2p.exe"/>
                   <File Id="MasterExe" Source="build/master.exe"/>
                   <File Id="DCFExe" Source="build/dcf.exe"/>
               </Component>
           </DirectoryRef>
           <DirectoryRef Id="SHAREDIR">
               <Component Id="ShareComponent" Guid="*">
                   <File Id="ConfigExample" Source="config.json.example"/>
                   <File Id="PluginExample" Source="plugins/custom_transport.c"/>
                   <File Id="License" Source="../LICENSE"/>
               </Component>
           </DirectoryRef>
           <Feature Id="Complete" Title="DCF SDK" Level="1">
               <ComponentRef Id="LibComponent"/>
               <ComponentRef Id="IncludeComponent"/>
               <ComponentRef Id="BinComponent"/>
               <ComponentRef Id="ShareComponent"/>
           </Feature>
       </Product>
   </Wix>
   ```
3. **Compile MSI**:
   ```bash
   candle dcf-sdk.wxs
   light dcf-sdk.wixobj
   ```
4. **Output**: `dcf-sdk.msi`

## Testing Executables
1. **Run P2P Example**:
   ```bash
   ./build/p2p
   ```
   - Requires `config.json` with `"mode": "p2p"`.
2. **Run Master Example**:
   ```bash
   ./build/master
   ```
   - Requires `master_config.json` with `"mode": "master"`.
3. **Run CLI/TUI**:
   ```bash
   ./build/dcf master-assign-role "peer1" "p2p" --json
   ```
4. **Memory Testing (Linux/macOS)**:
   ```bash
   valgrind --leak-check=full ./build/p2p
   ```
5. **Integration Testing**:
   - Use Docker to set up multi-node environment:
     ```bash
     docker run -v $(pwd)/config.json:/config.json -p 50051:50051 dcf-sdk ./dcf
     ```
   - Test AUTO mode by running `master` and `p2p` with `"mode": "auto"`.

## Distribution Notes
- **GPL-3.0 Compliance**: Include `LICENSE` in all packages. Ensure source code is available (e.g., link to GitHub repository).
- **Dependencies**: Packages assume dependencies are installed (e.g., `libprotobuf-c`, `libcjson`). For Windows, include DLLs in `bin/` if using Visual Studio.
- **Export Compliance**: DCF avoids encryption. Users must ensure custom plugins comply with EAR/ITAR.
- **Installation**:
  - Linux: `sudo dpkg -i dcf-sdk_5.0.0-1_amd64.deb` or `sudo rpm -i dcf-sdk-5.0.0-1.x86_64.rpm`
  - macOS: Mount `dcf-sdk-5.0.0.dmg`, copy to `/usr/local`
  - Windows: Extract `dcf-sdk-5.0.0.zip` or run `dcf-sdk.msi`

## Troubleshooting
- **CMake Errors**: Verify dependency paths (`CMAKE_PREFIX_PATH`). Ensure `protoc` and `grpc` tools are in PATH.
- **Linker Errors**: Check `libprotobuf-c`, `libcjson`, `libncurses` installations.
- **Windows DLL Issues**: Ensure MSYS2 or Visual Studio runtime DLLs are available (e.g., `msvcr100.dll`).
- **Valgrind Issues**: Use `--error-exitcode=1` to catch leaks in CI.
- **AUTO Mode Failures**: Verify master node is running and `node_id` is unique in `config.json`.