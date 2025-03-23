#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "cursor_control.h"

namespace py = pybind11;

PYBIND11_MODULE(mouse_control, m) {
    m.doc() = "Undetectable mouse movement module using Windows low-level APIs";
    
    py::class_<CursorControl>(m, "CursorControl")
        .def(py::init<>())
        .def("initialize", &CursorControl::Initialize)
        .def("move", &CursorControl::MoveCursor, 
             py::arg("delta_x"), py::arg("delta_y"))
        .def("move_to", &CursorControl::MoveCursorTo, 
             py::arg("x"), py::arg("y"))
        .def("get_position", &CursorControl::GetPosition)
        .def("set_position", &CursorControl::SetPosition,
             py::arg("x"), py::arg("y"))
        .def_property("enabled", 
                     &CursorControl::IsEnabled, 
                     &CursorControl::SetEnabled)
        
        // Expose humanization methods
        .def_property("humanization_level",
                     &CursorControl::GetHumanizationLevel,
                     &CursorControl::SetHumanizationLevel)
        .def_property("jitter_amount",
                     &CursorControl::GetJitterAmount,
                     &CursorControl::SetJitterAmount)
        .def_property("movement_delay",
                     &CursorControl::GetMovementDelay,
                     &CursorControl::SetMovementDelay)
        .def_property("path_deviation",
                     &CursorControl::GetPathDeviation,
                     &CursorControl::SetPathDeviation)
        .def_property("movement_style",
                     &CursorControl::GetMovementStyle,
                     &CursorControl::SetMovementStyle)
        
        .def("move_up", [](CursorControl &self, int pixels) { 
                return self.MoveCursor(0, -pixels); 
             }, py::arg("pixels") = 5)
        .def("move_down", [](CursorControl &self, int pixels) { 
                return self.MoveCursor(0, pixels); 
             }, py::arg("pixels") = 5)
        .def("move_left", [](CursorControl &self, int pixels) { 
                return self.MoveCursor(-pixels, 0); 
             }, py::arg("pixels") = 5)
        .def("move_right", [](CursorControl &self, int pixels) { 
                return self.MoveCursor(pixels, 0); 
             }, py::arg("pixels") = 5);
    
    m.def("move", [](int delta_x, int delta_y) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        return controller.MoveCursor(delta_x, delta_y);
    }, py::arg("delta_x"), py::arg("delta_y"));
    
    m.def("move_to", [](int x, int y) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        return controller.MoveCursorTo(x, y);
    }, py::arg("x"), py::arg("y"));
    
    m.def("get_position", []() {
        static CursorControl controller;
        return controller.GetPosition();
    });
    
    m.def("up", [](int pixels) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        return controller.MoveCursor(0, -pixels);
    }, py::arg("pixels") = 5);
    
    m.def("down", [](int pixels) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        return controller.MoveCursor(0, pixels);
    }, py::arg("pixels") = 5);
    
    m.def("left", [](int pixels) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        return controller.MoveCursor(-pixels, 0);
    }, py::arg("pixels") = 5);
    
    m.def("right", [](int pixels) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        return controller.MoveCursor(pixels, 0);
    }, py::arg("pixels") = 5);
    
    // Add humanization configuration at module level
    m.def("set_humanization_level", [](int level) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        controller.SetHumanizationLevel(level);
    }, py::arg("level"), "Set humanization level (0-10, 0=none, 10=maximum)");
    
    m.def("set_jitter_amount", [](int amount) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        controller.SetJitterAmount(amount);
    }, py::arg("amount"), "Set jitter amount (0-10 pixels)");
    
    m.def("set_movement_delay", [](int microseconds) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        controller.SetMovementDelay(microseconds);
    }, py::arg("microseconds"), "Set delay between movements in microseconds");
    
    m.def("set_path_deviation", [](double deviation) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        controller.SetPathDeviation(deviation);
    }, py::arg("deviation"), "Set path deviation (0.0-1.0)");
    
    m.def("set_movement_style", [](int style) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        controller.SetMovementStyle(style);
    }, py::arg("style"), "Set movement style (0=direct, 1=bezier, 2=random)");
} 