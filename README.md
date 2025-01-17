### COMP4981 Assignment 1
Simple HTTP Server

Project Requirements:
- HTTP server capable of handling multiple client connections concurrently
- Use POSIX socket API
- Parse and respond to basic HTTP requests
- Compatibility across Linux, FreeBSD, and macOS platforms
- Accept HTTP GET and HEAD requests and respond with appropriate content
- Proper error handling for unsupported methods or missing files

## Contributors

Kevin Nguyen [https://github.com/kvnbanunu]

Evin Gonzales [https://github.com/evin-gg]

## Prerequisites

This project uses a build system written by D'arcy Smith.

To compile using the build system you need:
- D'arcy's libraries built from [https://github.com/programming101dev/scripts]

Tested Platforms:
- Arch Linux 2024.12.01
- Manjaro 24.2
- Ubuntu 2024.04.1
- MacOS 14.2 (clang only)
- FreeBSD 14.0-RELEASE-p4

Dependencies:
- gcc or clang (Makefile specifies gcc)
- make

## Installation

Clone this repository:
```sh
git clone --single-branch -branch comp3980 https://github.com/kvnbanunu/networkroyale
```

Build with make:
```sh
make build
```

Build with D'arcy's system:
1. Link your .flags directory
   ```sh
   ./link-flags.sh <path>/.flags
   ```
2. Change compiler to gcc or clang
   ```sh
   ./change-compiler.sh -c <gcc or clang>
   ```
3. Generate cmakelist
   ```sh
   ./generate-cmakelists.sh
   ```
4. Build
   ```sh
   ./build.sh
   ```

## Usage

1. Change to build directory
   ```sh
   cd build/
   ```
2. Run server
   ```sh
   ./server
   ```
3. Open another terminal on the same computer or another computer on the same network
4. Run client
   ```sh
   ./client <ip address> <port>
   ```
