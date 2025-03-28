# ShadowCursor

## Undetectable Mouse Control for Python with Advanced Humanization

ShadowCursor provides a sophisticated approach to programmatic mouse control, specifically designed to bypass detection mechanisms while maintaining responsive performance and mimicking natural human movement patterns.

### Key Features

- **Stealth Operation**: Uses alternative, hardware-level cursor manipulation techniques that evade common detection methods
- **Human-like Movement**: Advanced humanization algorithms including bezier curves, variable timing, and natural jitter
- **Responsive Control**: Optimized for minimal input lag with multi-threaded background processing
- **Precision Movement**: Supports single-pixel increments for fine-grained positioning
- **Seamless Integration**: Normal mouse input remains functional alongside programmatic control
- **Simple API**: Intuitive Python interface for easy integration into existing projects

### Humanization Features

- **Bezier Curve Paths**: Creates smooth, natural arcs for longer movements
- **Micro-jitter**: Adds subtle, random variations to mimic natural hand tremor
- **Variable Timing**: Randomizes delays between movements to avoid mechanical patterns
- **Path Deviation**: Controls how much movement paths deviate from straight lines
- **Multiple Movement Styles**: Choose between direct, curved, or randomized movement patterns

### Technical Implementation

ShadowCursor achieves undetectable mouse control by using a signed driver to interface with the operating system and hardware at a low level. The driver is able to precisely manipulate the cursor without relying on standard input APIs that can be hooked or monitored.

By operating through a driver, ShadowCursor avoids many of the detection vectors used by anti-cheat systems. The driver is digitally signed, making it extremely difficult to reverse engineer or tamper with.

The core of ShadowCursor is still built with optimized C++ for maximum performance, and exposes a user-friendly Python API via pybind11. This architecture allows for responsive, real-time cursor control while abstracting away the low-level complexities.

### Typical Applications

- Automation scripts requiring mouse control in environments with input monitoring
- Gaming bots that need to mimic human-like aiming and movement
- Testing tools for security research
- Accessibility solutions requiring alternative cursor control mechanisms
- Research into human-computer interaction patterns

### Example

```python
import mouse_control

# Basic movement
mouse_control.move(5, 5)

# Configure humanization
mouse_control.set_humanization_level(7)  # Higher humanization
mouse_control.set_jitter_amount(3)       # Some jitter for natural movement
mouse_control.set_movement_delay(80)     # Faster movements
mouse_control.set_path_deviation(0.2)    # Less deviation for more accuracy
mouse_control.set_movement_style(1)      # Bezier curve for smooth movement

# Get current position
x, y = mouse_control.get_position()
print(f"Cursor at: {x}, {y}")

# Move to absolute coordinates with humanized movement
mouse_control.move_to(500, 500)  # Will use bezier curve for longer distances
```

### Advanced Example

```python
# Create a controller instance for more control
controller = mouse_control.CursorControl()
controller.initialize()

# Configure for FPS-like aiming
controller.humanization_level = 7       # Higher humanization for realism
controller.jitter_amount = 3            # Some jitter for natural hand movement
controller.movement_delay = 80          # Faster movements
controller.path_deviation = 0.2         # Less deviation for more accuracy
controller.movement_style = 1           # Bezier curve for smooth aiming

# Simulate aiming at a target
controller.move_to(target_x, target_y)

# Make micro-adjustments (like a human would)
controller.move(random.randint(-5, 5), random.randint(-5, 5))
``` 