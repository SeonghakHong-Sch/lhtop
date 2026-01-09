# lhtop

**Light htop** - A lightweight system monitoring tool for Linux

A lightweight system monitoring tool built for learning Linux system programming.

## Overview

`lhtop` is a terminal-based system monitoring tool similar to `htop`. It directly parses the `/proc` filesystem to display real-time information about CPU, memory, and processes.

## Build Requirements

- **Operating System**: Linux
- **Compiler**: GCC (C11 or later)
- **Required Libraries**:
  - ncurses
- **Build Tools**:
  - CMake 3.10 or later

### Installing ncurses

```bash
# Ubuntu/Debian
sudo apt-get install libncurses5-dev libncursesw5-dev

# Fedora/RHEL/CentOS
sudo dnf install ncurses-devel

# Arch Linux
sudo pacman -S ncurses
```

## Building and Installation

```bash
# Clone the repository
git clone <repository-url>
cd lhtop

# Create and enter build directory
mkdir -p build
cd build

# Configure and build with CMake
cmake ..
make

# Run the program
./lhtop
```

## Project Structure

```
lhtop/
├── include/           # Header files
│   ├── common.h      # Common constants and type definitions
│   ├── system.h      # System information structures and functions
│   ├── process.h     # Process information structures and functions
│   ├── stats.h       # Statistics calculation functions
│   ├── display.h     # ncurses-based display output
│   └── utils.h       # Utility functions
├── src/              # Source files
│   ├── system.c      # Parse /proc/stat, /proc/meminfo
│   ├── process.c     # Parse /proc/[pid]/
│   ├── stats.c       # Calculate CPU/memory usage
│   ├── display.c     # TUI rendering
│   └── utils.c       # Utility implementations
├── test/             # Test code
├── main.c            # Main entry point
└── CMakeLists.txt    # CMake build configuration
```

## Implementation Details

### `/proc` Filesystem Usage

- `/proc/stat`: System-wide and per-core CPU time information
- `/proc/meminfo`: Memory and swap information
- `/proc/[pid]/stat`: Process state and resource usage information
- `/proc/[pid]/status`: Detailed process information

## Learning Objectives

This project was created to learn the following Linux system programming concepts:

1. **Filesystem Interface**: Accessing kernel information through the `/proc` virtual filesystem
2. **Memory Management**: Dynamic memory allocation/deallocation and memory leak prevention
3. **System Calls**: Using system calls like `sysconf()`, `sleep()`, etc.
4. **String Parsing**: Parsing text data using `sscanf()`, `fgets()`, etc.
5. **TUI Programming**: Implementing terminal UI with ncurses
6. **Real-time Monitoring**: Periodic data collection and statistics calculation
