# CGI for C Language

A lightweight, cross-platform graphics library for C that provides a simple API for creating windows, rendering graphics, and handling user input on both Windows and Linux.

[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-blue)]()
[![Language](https://img.shields.io/badge/language-C11-orange)]()
[![License](https://img.shields.io/badge/license-MIT-green)]()

## Features

- ✅ **Cross-platform**: Works on Windows and Linux with platform-specific implementations
- ✅ **Simple API**: Easy-to-use functions for window management and graphics rendering
- ✅ **Lightweight**: Minimal dependencies, uses OpenGL for rendering
- ✅ **C-compatible**: Pure C library with C++ compatibility
- ✅ **Window Management**: Create, show, close, and manage multiple windows
- ✅ **Input Handling**: Keyboard input detection and handling
- ✅ **Graphics Rendering**: Pixel-level drawing and color manipulation
- ✅ **Event Loop**: Built-in event processing and window update cycle

## Table of Contents

- [Prerequisites](#prerequisites)
- [Building](#building)
  - [Windows](#windows)
  - [Linux](#linux)
- [Usage](#usage)
  - [Basic Example](#basic-example)
  - [API Reference](#api-reference)
- [Project Structure](#project-structure)
- [CMake Options](#cmake-options)
- [Contributing](#contributing)
- [License](#license)

## Prerequisites

### Windows
- **CMake** 3.10 or higher
- **Visual Studio 2017+** or **MinGW** with GCC
- **OpenGL** (usually comes with graphics drivers)

### Linux
- **CMake** 3.10 or higher
- **GCC** or **Clang**
- **OpenGL** development libraries
- **X11** development libraries

Install dependencies on Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install build-essential cmake libgl1-mesa-dev libx11-dev
```

Install dependencies on Fedora/RHEL:
```bash
sudo dnf install gcc gcc-c++ cmake mesa-libGL-devel libX11-devel
```

## Building

### Windows

#### Using Visual Studio
```cmd
# Clone the repository
git clone https://github.com/devSiddharthKarn/CGI-for-C-language.git
cd CGI-for-C-language

# Configure with Visual Studio generator
cmake -B build -G "Visual Studio 17 2022"

# Build the project
cmake --build build --config Release

# Run tests (optional)
cd build
ctest -C Release
```

#### Using MinGW
```cmd
# Configure with MinGW
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Run tests
cd build
ctest
```

### Linux

```bash
# Clone the repository
git clone https://github.com/devSiddharthKarn/CGI-for-C-language.git
cd CGI-for-C-language

# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Run tests (optional)
cd build
ctest

# Install system-wide (optional)
sudo cmake --install build
```

### Build Output

After building, you'll find:
- **Library**: `build/lib/libcgi.a` (or `cgi.lib` on Windows)
- **Test executables**: `build/bin/test_c`, `build/bin/test_cpp`

## Usage

### Basic Example

Here's a simple example that creates a window and responds to keyboard input:

```c
#define CGI_IMPLEMENTATION_MODE_VERBOSE
#include "cgi.h"
#include <stdio.h>

int main() {
    // Initialize CGI
    CGI* cgi = CGIStart();
    
    // Create a window (500x500 pixels)
    CGIWindow* window = CGICreateWindow(
        "mywindow",              // Class name
        "My CGI Window",         // Window title
        100,                     // X position
        100,                     // Y position
        500,                     // Width
        500,                     // Height
        CGIMakeColor(23, 45, 67) // Background color
    );
    
    // Show the window
    CGIShowWindow(window);
    
    // Position for drawing
    CGIPoint p = {250, 250};
    
    // Main event loop
    while(CGIIsWindowOpen(window)) {
        // Refresh window
        CGIRefreshWindow(window);
        
        // Handle keyboard input
        if(CGIIsKeyPressed(window, CGI_input_key_w)) {
            p.y--;  // Move up
        }
        if(CGIIsKeyPressed(window, CGI_input_key_s)) {
            p.y++;  // Move down
        }
        if(CGIIsKeyPressed(window, CGI_input_key_a)) {
            p.x--;  // Move left
        }
        if(CGIIsKeyPressed(window, CGI_input_key_d)) {
            p.x++;  // Move right
        }
        
        // Draw pixel at current position
        CGISetPixel(window, p.x, p.y, CGIMakeColor(255, 255, 255));
    }
    
    // Cleanup
    CGIWindowCleanup(window);
    CGIEnd(cgi);
    
    return 0;
}
```

### Compiling Your Program

**Using CMake:**
```cmake
cmake_minimum_required(VERSION 3.10)
project(MyApp C)

# Add CGI library
add_subdirectory(path/to/CGI-for-C-language)

# Create your executable
add_executable(myapp main.c)

# Link with CGI library
target_link_libraries(myapp PRIVATE cgi)
```

**Manual compilation on Linux:**
```bash
gcc main.c -I/path/to/CGI/include -L/path/to/CGI/build/lib -lcgi -lGL -lX11 -lm -o myapp
```

**Manual compilation on Windows:**
```cmd
gcc main.c -I/path/to/CGI/include -L/path/to/CGI/build/lib -lcgi -lopengl32 -lgdi32 -luser32 -o myapp.exe
```

## API Reference

### Initialization & Cleanup

```c
CGI* CGIStart();
```
Initialize the CGI library. Call this before using any other functions.

```c
CGIBool CGIEnd(CGI* cgi);
```
Cleanup and shutdown the CGI library.

### Window Management

```c
CGIWindow* CGICreateWindow(
    char* classname,
    char* window_name,
    unsigned int x_pos,
    unsigned int y_pos,
    unsigned int width,
    unsigned int height,
    CGIColor_t color
);
```
Create a new window with specified properties.

```c
CGIBool CGIShowWindow(CGIWindow* window);
```
Display the window on screen.

```c
CGIBool CGIIsWindowOpen(const CGIWindow* window);
```
Check if the window is still open.

```c
CGIBool CGICloseWindow(CGIWindow* window);
```
Close the window.

```c
CGIBool CGIWindowCleanup(CGIWindow* window);
```
Free resources associated with the window.

```c
CGIBool CGIRefreshWindow(CGIWindow* window);
```
Update the window and process events. Call this in your main loop.

### Graphics Functions

```c
void CGISetPixel(CGIWindow* window, int x_pos, int y_pos, CGIColor_t color);
```
Set a pixel at the specified coordinates to the given color.

```c
CGIColor_t CGIMakeColor(unsigned char r, unsigned char g, unsigned char b);
```
Create a color from RGB values (0-255 for each channel).

### Input Handling

```c
CGIBool CGIIsKeyPressed(CGIWindow* window, CGIKey key);
```
Check if a specific key is currently pressed.

**Available Keys:**
- `CGI_input_key_a` through `CGI_input_key_z`
- `CGI_input_key_0` through `CGI_input_key_9`
- Arrow keys, function keys, etc.

### Queries

```c
const void* CGIPerformQuery(CGIQuery query, CGI* cgi, CGIWindow* window);
```
Query information about the CGI instance or window.

### Types

```c
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} CGIColor_t;

typedef enum {
    CGI_false = 0,
    CGI_true = 1
} CGIBool;

typedef struct {
    int x;
    int y;
} CGIPoint;
```

## Project Structure

```
CGI-for-C-language/
├── CMakeLists.txt           # Main CMake configuration
├── README.md                # This file
├── include/
│   ├── cgi.h               # Main API header
│   ├── typedefs.h          # Type definitions and enums
│   └── opengl/             # OpenGL/GLAD headers
│       ├── include/
│       └── src/
│           └── glad.c      # OpenGL loader
├── src/
│   ├── platform.c          # Core platform implementation
│   ├── win32.c            # Windows-specific code
│   └── linux.c            # Linux-specific code
└── test/
    ├── CMakeLists.txt     # Test configuration
    ├── test.c             # C test example
    └── test2.c            # Additional test
```

## CMake Options

Configure the build with these options:

```bash
# Build as shared library instead of static
cmake -B build -DBUILD_SHARED_LIBS=ON

# Disable tests
cmake -B build -DBUILD_TESTS=OFF

# Debug build
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Verbose build output
cmake -B build -DCMAKE_VERBOSE_MAKEFILE=ON
```

**Available Options:**
- `BUILD_SHARED_LIBS` (OFF): Build shared library (.dll/.so) instead of static
- `BUILD_TESTS` (ON): Build test programs
- `CMAKE_BUILD_TYPE`: Debug, Release, RelWithDebInfo, MinSizeRel

## Examples

Check the `test/` directory for more examples:
- `test/test.c` - Interactive window with keyboard control
- `test/test2.c` - Additional usage examples

## Troubleshooting

### Windows Issues

**"OpenGL library not found":**
- Ensure you have graphics drivers installed
- Link against `opengl32.lib` manually if needed

**MinGW compilation errors:**
- Make sure MinGW bin directory is in PATH
- Use `cmake -G "MinGW Makefiles"` explicitly

### Linux Issues

**"X11/Xlib.h not found":**
```bash
sudo apt-get install libx11-dev
```

**"GL/gl.h not found":**
```bash
sudo apt-get install libgl1-mesa-dev
```

**Linking errors:**
Make sure to link with `-lGL -lX11 -lm -lpthread -ldl`

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Author

**Siddharth Karn**
- GitHub: [@devSiddharthKarn](https://github.com/devSiddharthKarn)

## Acknowledgments

- OpenGL for rendering backend
- GLAD for OpenGL loading
- Cross-platform windowing inspired by GLFW

---

**Note:** This is an educational project demonstrating cross-platform graphics programming in C. For production applications, consider using more mature libraries like SDL2, GLFW, or Allegro.

## See Also

- [CMake Tutorial](CMake_Tutorial_Beginner_to_Advanced.md) - Learn how the build system works
- [OpenGL Documentation](https://www.opengl.org/documentation/)
- [X11 Programming Manual](https://tronche.com/gui/x/xlib/)
