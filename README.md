# ShadowCursor

A Python module providing undetectable, low-latency mouse cursor control that bypasses anti-cheat mechanisms by using native, hardware-level cursor manipulation techniques with advanced humanization features.

## Features

- **Undetectable Movement**: Uses alternative mouse movement methods that bypass traditional anti-cheat detection
- **Human-like Movement**: Advanced humanization features including bezier curves, jitter, and variable delays
- **Low-Latency Control**: Optimized for minimal input lag and responsive cursor movement
- **Pixel-Perfect Precision**: Single-pixel movement capability for fine-grained control
- **Multi-Threaded Design**: Background processing to avoid blocking the main application
- **Easy Python Interface**: Simple API for both direct and high-level cursor manipulation
- **Normal Mouse Compatibility**: Use your regular mouse alongside programmatic control

## Requirements

- Windows 10/11
- Python 3.6 or newer
- Visual Studio 2019 or 2022 with C++ workload
- vcpkg package manager

## Quick Start

### 1. Install vcpkg

If you don't have vcpkg installed:

```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

Set the VCPKG_ROOT environment variable:

```bash
setx VCPKG_ROOT "C:\path\to\vcpkg"
```

### 2. Build the Module

Run the build script:

```bash
build.bat
```

### 3. Run the Example

```bash
python mouse_example.py
```

## Usage

### Basic Usage

```python
import mouse_control

# Move the cursor 5 pixels right, 3 pixels down
mouse_control.move(5, 3)

# Move to an absolute position
mouse_control.move_to(500, 500)

# Get the current cursor position
x, y = mouse_control.get_position()
print(f"Cursor position: ({x}, {y})")

# Directional movements
mouse_control.up(5)    # Move 5 pixels up
mouse_control.down(5)  # Move 5 pixels down
mouse_control.left(5)  # Move 5 pixels left
mouse_control.right(5) # Move 5 pixels right
```

### Advanced Usage with Humanization

```python
import mouse_control

# Create a CursorControl instance
control = mouse_control.CursorControl()
control.initialize()

# Configure humanization settings
control.humanization_level = 7       # 0-10 scale (0=none, 10=maximum)
control.jitter_amount = 3            # Pixels of random movement (0-10)
control.movement_delay = 80          # Microseconds between movements
control.path_deviation = 0.2         # Path deviation factor (0.0-1.0)
control.movement_style = 1           # 0=direct, 1=bezier curve, 2=random

# Move the cursor with humanized movement
control.move(5, 5)
control.move_to(800, 600)  # Uses bezier curve for longer movements

# Module-level humanization configuration
mouse_control.set_humanization_level(5)
mouse_control.set_jitter_amount(2)
mouse_control.set_movement_delay(100)
mouse_control.set_path_deviation(0.3)
mouse_control.set_movement_style(1)
```

## How It Works

ShadowCursor uses a signed driver to achieve undetectable mouse control:

1. **Low-level Driver**: Interfaces directly with the operating system and hardware
2. **Signed Driver**: Digitally signed to prevent tampering and reverse engineering 
3. **Multi-threaded Processing**: Handles cursor movement in a background thread
4. **Optimized Low-latency Movement**: Custom implementation for minimal input lag
5. **Movement Limit Control**: Ensures precise movements with configurable speed
6. **Natural Jitter Patterns**: Adds subtle human-like movement variations
7. **Bezier Curve Paths**: Creates smooth, natural movement paths for longer distances
8. **Variable Timing**: Randomizes delays between movements to mimic human behavior

## Example Controls

The included example (mouse_example.py) provides these controls:

- **Arrow keys / WASD**: Move cursor in corresponding direction
- **Hold Shift**: Move cursor faster (3 pixels instead of 1)
- **T**: Toggle mouse control on/off
- **G**: Print current cursor position
- **1-5**: Set humanization level (1=low, 5=high)
- **F1**: Toggle between movement styles (direct, bezier, random)
- **F2/F3**: Decrease/increase jitter amount
- **F4/F5**: Decrease/increase movement delay
- **H**: Show help information
- **Q**: Quit the program

## Advanced Customization

You can easily customize the cursor behavior by modifying these settings in mouse_example.py:

```python
# Movement settings
MOVEMENT_SPEED = 1        # Default pixels per movement
FAST_MOVEMENT_SPEED = 3   # Speed when holding shift
POLL_RATE = 0.001         # Input polling rate (lower = more responsive)

# Humanization settings
controller.humanization_level = 5    # Overall humanization (0-10)
controller.jitter_amount = 2         # Random movement jitter (0-10 pixels)
controller.movement_delay = 100      # Delay between movements (microseconds)
controller.path_deviation = 0.3      # Path randomness factor (0.0-1.0)
controller.movement_style = 1        # Movement algorithm (0-2)
```

## Example Modes

The example program includes three demonstration modes:

1. **Basic Mouse Control**: Interactive keyboard-controlled cursor movement
2. **Module-level Functions**: Demonstrates using the global module functions
3. **Valorant-like Aim Simulation**: Shows how the module can be used for FPS-style aiming with human-like movements

## Building From Source

To build ShadowCursor from source, you'll need to follow these steps:

### Prerequisites

Make sure you have the following tools installed:

- **Windows 10/11**: ShadowCursor is designed for Windows and requires version 10 or newer.
- **Python 3.6 or later**: You can download Python from the [official website](https://www.python.org/downloads/). Make sure to add Python to your PATH during installation.
- **Visual Studio 2019 or 2022**: Install Visual Studio with the "Desktop development with C++" workload. You can use the [Community edition](https://visualstudio.microsoft.com/vs/community/) for free.
- **vcpkg**: Follow the [official instructions](https://github.com/microsoft/vcpkg#getting-started) to install vcpkg. Make sure to integrate it with your Visual Studio installation.

### Build Steps

1. Clone the ShadowCursor repository:
   ```bash
   git clone https://github.com/NeedlessPage819/ShadowCursor.git
   cd ShadowCursor
   ```

2. Set the `VCPKG_ROOT` environment variable to your vcpkg installation directory:
   ```bash
   setx VCPKG_ROOT "C:\path\to\vcpkg"
   ```

3. Run the build script:
   ```bash
   build.bat
   ```
   This script will:
   - Check for the required tools (Python, Visual Studio, vcpkg)
   - Install the pybind11 dependency via vcpkg
   - Configure and run the CMake build
   - Copy the compiled module to the project directory

4. Verify the build by running the example:
   ```bash
   python mouse_example.py
   ```
   If the example runs without errors and you can control the mouse, the build was successful.

### Troubleshooting

- If the build script fails with an error about missing tools, make sure you have installed all the prerequisites correctly and that their paths are set up properly.
- If you encounter build errors related to missing headers or libraries, try updating your vcpkg packages:
  ```bash
  vcpkg update
  vcpkg upgrade --no-dry-run
  ```
- If the example fails to run due to missing DLLs, make sure the compiled module is in the same directory as your Python script and that the module's dependencies are available on your system PATH.

### Manual Build

If you prefer to build ShadowCursor manually using CMake, follow these steps:

1. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

2. Run CMake configuration:
   ```bash
   cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake ..
   ```

3. Build the project:
   ```bash
   cmake --build . --config Release
   ```

4. Copy the compiled module to the project directory:
   ```bash
   copy Release\mouse_control.pyd ..
   ```

### Integration

To use ShadowCursor in your own Python projects:

1. Copy the `mouse_control.pyd` file from the `ShadowCursor` directory to your project directory.

2. Import the module in your Python code:
   ```python
   import mouse_control
   ```

3. Use the ShadowCursor API functions as described in the [Usage](#usage) section.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Disclaimer

This project is for educational purposes only. Use responsibly and in accordance with the terms of service of any software you interact with. 