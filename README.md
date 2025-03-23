# ShadowCursor

A Python module providing undetectable, low-latency mouse cursor control that bypasses anti-cheat mechanisms by using native, hardware-level cursor manipulation techniques.

## Features

- **Undetectable Movement**: Uses alternative mouse movement methods that bypass traditional anti-cheat detection
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

### Advanced Usage

```python
import mouse_control

# Create a CursorControl instance
control = mouse_control.CursorControl()
control.initialize()

# Move the cursor
control.move(1, 1)     # Move 1 pixel diagonally
control.move_to(800, 600)  # Move to coordinates (800, 600)

# Get current position
x, y = control.get_position()

# Enable/disable cursor control
control.enabled = False
# ... later ...
control.enabled = True
```

## How It Works

ShadowCursor uses several techniques to avoid detection:

1. **Direct Hardware Access**: Bypasses standard OS input APIs
2. **Multi-threaded Processing**: Handles cursor movement in a background thread
3. **Optimized Low-latency Movement**: Custom implementation for minimal input lag
4. **Movement Limit Control**: Ensures precise movements with configurable speed
5. **Natural Jitter Patterns**: Adds subtle human-like movement variations

## Example Controls

The included example (mouse_example.py) provides these controls:

- **Arrow keys / WASD**: Move cursor in corresponding direction
- **Hold Shift**: Move cursor faster (3 pixels instead of 1)
- **T**: Toggle mouse control on/off
- **G**: Print current cursor position
- **H**: Show help information
- **Q**: Quit the program

## Advanced Customization

You can easily customize the cursor behavior by modifying these settings in mouse_example.py:

```python
# Movement settings
MOVEMENT_SPEED = 1        # Default pixels per movement
FAST_MOVEMENT_SPEED = 3   # Speed when holding shift
POLL_RATE = 0.001         # Input polling rate (lower = more responsive)
```

## Building From Source

For detailed build instructions, see [VCPKG_SETUP.md](VCPKG_SETUP.md).

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Disclaimer

This project is for educational purposes only. Use responsibly and in accordance with the terms of service of any software you interact with. 
