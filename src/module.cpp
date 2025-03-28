#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "cursor_control.h"

namespace py = pybind11;

// Define the mouse control module using Pybind11
PYBIND11_MODULE(mouse_control, m) {
    m.doc() = "Undetectable mouse movement module using a signed driver"; // Updated to reflect the use of a signed driver
    
    py::class_<CursorControl>(m, "CursorControl")
        .def(py::init<>()) // Constructor for CursorControl
        .def("initialize", &CursorControl::Initialize) // Initialize the cursor control system
        .def("move", &CursorControl::MoveCursor, 
             py::arg("delta_x"), py::arg("delta_y")) // Move cursor by specified deltas
        .def("move_to", &CursorControl::MoveCursorTo, 
             py::arg("x"), py::arg("y")) // Move cursor to absolute position
        .def("get_position", &CursorControl::GetPosition) // Get current cursor position
        .def("set_position", &CursorControl::SetPosition,
             py::arg("x"), py::arg("y")) // Set cursor to specific position
        .def_property("enabled", 
                     &CursorControl::IsEnabled, 
                     &CursorControl::SetEnabled) // Enable or disable cursor control
        
        // Expose humanization methods for more natural movements
        .def_property("humanization_level",
                     &CursorControl::GetHumanizationLevel,
                     &CursorControl::SetHumanizationLevel) // Get and set humanization level
        .def_property("jitter_amount",
                     &CursorControl::GetJitterAmount,
                     &CursorControl::SetJitterAmount) // Get and set jitter amount
        .def_property("movement_delay",
                     &CursorControl::GetMovementDelay,
                     &CursorControl::SetMovementDelay) // Get and set movement delay
        .def_property("path_deviation",
                     &CursorControl::GetPathDeviation,
                     &CursorControl::SetPathDeviation) // Get and set path deviation
        .def_property("movement_style",
                     &CursorControl::GetMovementStyle,
                     &CursorControl::SetMovementStyle) // Get and set movement style
        
        // Define movement methods for convenience
        .def("move_up", [](CursorControl &self, int pixels) { 
                return self.MoveCursor(0, -pixels); // Move cursor up
             }, py::arg("pixels") = 5)
        .def("move_down", [](CursorControl &self, int pixels) { 
                return self.MoveCursor(0, pixels); // Move cursor down
             }, py::arg("pixels") = 5)
        .def("move_left", [](CursorControl &self, int pixels) { 
                return self.MoveCursor(-pixels, 0); // Move cursor left
             }, py::arg("pixels") = 5)
        .def("move_right", [](CursorControl &self, int pixels) { 
                return self.MoveCursor(pixels, 0); // Move cursor right
             }, py::arg("pixels") = 5)
        .def("is_driver_initialized", &CursorControl::IsDriverInitialized) // Check if the driver is initialized
    ;
    
    // Define module-level movement functions
    m.def("move", [](int delta_x, int delta_y) {
        static CursorControl controller; // Static instance of CursorControl
        static bool initialized = false; // Initialization flag
        
        if (!initialized) {
            controller.Initialize(); // Initialize if not already done
            initialized = true;
        }
        
        return controller.MoveCursor(delta_x, delta_y); // Move cursor by deltas
    }, py::arg("delta_x"), py::arg("delta_y"));
    
    m.def("move_to", [](int x, int y) {
        static CursorControl controller; // Static instance of CursorControl
        static bool initialized = false; // Initialization flag
        
        if (!initialized) {
            controller.Initialize(); // Initialize if not already done
            initialized = true;
        }
        
        return controller.MoveCursorTo(x, y); // Move cursor to absolute position
    }, py::arg("x"), py::arg("y"));
    
    m.def("get_position", []() {
        static CursorControl controller; // Static instance of CursorControl
        return controller.GetPosition(); // Get current cursor position
    });
    
    // Define directional movement functions
    m.def("up", [](int pixels) {
        static CursorControl controller; // Static instance of CursorControl
        static bool initialized = false; // Initialization flag
        
        if (!initialized) {
            controller.Initialize(); // Initialize if not already done
            initialized = true;
        }
        
        return controller.MoveCursor(0, -pixels); // Move cursor up
    }, py::arg("pixels") = 5);
    
    m.def("down", [](int pixels) {
        static CursorControl controller; // Static instance of CursorControl
        static bool initialized = false; // Initialization flag
        
        if (!initialized) {
            controller.Initialize(); // Initialize if not already done
            initialized = true;
        }
        
        return controller.MoveCursor(0, pixels); // Move cursor down
    }, py::arg("pixels") = 5);
    
    m.def("left", [](int pixels) {
        static CursorControl controller; // Static instance of CursorControl
        static bool initialized = false; // Initialization flag
        
        if (!initialized) {
            controller.Initialize(); // Initialize if not already done
            initialized = true;
        }
        
        return controller.MoveCursor(-pixels, 0); // Move cursor left
    }, py::arg("pixels") = 5);
    
    m.def("right", [](int pixels) {
        static CursorControl controller; // Static instance of CursorControl
        static bool initialized = false; // Initialization flag
        
        if (!initialized) {
            controller.Initialize(); // Initialize if not already done
            initialized = true;
        }
        
        return controller.MoveCursor(pixels, 0); // Move cursor right
    }, py::arg("pixels") = 5);
    
    // Add humanization configuration at module level
    m.def("set_humanization_level", [](int level) {
        static CursorControl controller; // Static instance of CursorControl
        static bool initialized = false; // Initialization flag
        
        if (!initialized) {
            controller.Initialize(); // Initialize if not already done
            initialized = true;
        }
        
        controller.SetHumanizationLevel(level); // Set humanization level
    }, py::arg("level"), "Set humanization level (0-10, 0=none, 10=maximum)");
    
    m.def("set_jitter_amount", [](int amount) {
        static CursorControl controller; // Static instance of CursorControl
        static bool initialized = false; // Initialization flag
        
        if (!initialized) {
            controller.Initialize(); // Initialize if not already done
            initialized = true;
        }
        
        controller.SetJitterAmount(amount); // Set jitter amount
    }, py::arg("amount"), "Set jitter amount (0-10 pixels)");
    
    m.def("set_movement_delay", [](int microseconds) {
        static CursorControl controller; // Static instance of CursorControl
        static bool initialized = false; // Initialization flag
        
        if (!initialized) {
            controller.Initialize(); // Initialize if not already done
            initialized = true;
        }
        
        controller.SetMovementDelay(microseconds); // Set movement delay
    }, py::arg("microseconds"), "Set delay between movements in microseconds");
    
    m.def("set_path_deviation", [](double deviation) {
        static CursorControl controller; // Static instance of CursorControl
        static bool initialized = false; // Initialization flag
        
        if (!initialized) {
            controller.Initialize(); // Initialize if not already done
            initialized = true;
        }
        
        controller.SetPathDeviation(deviation); // Set path deviation
    }, py::arg("deviation"), "Set path deviation (0.0-1.0)");
    
    m.def("set_movement_style", [](int style) {
        static CursorControl controller; // Static instance of CursorControl
        static bool initialized = false; // Initialization flag
        
        if (!initialized) {
            controller.Initialize(); // Initialize if not already done
            initialized = true;
        }
        
        controller.SetMovementStyle(style); // Set movement style
    }, py::arg("style"), "Set movement style (0=direct, 1=bezier, 2=random)");
} 