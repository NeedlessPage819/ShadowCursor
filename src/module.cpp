#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "cursor_control.h"

namespace py = pybind11;

PYBIND11_MODULE(mouse_control, m) {
    m.doc() = "Undetectable mouse movement module using Windows low-level APIs";
    
    // Create the main CursorControl class
    py::class_<CursorControl>(m, "CursorControl")
        .def(py::init<>())
        .def("initialize", &CursorControl::Initialize, 
             "Initialize the cursor control module")
        .def("move", &CursorControl::MoveCursor, 
             py::arg("delta_x"), py::arg("delta_y"),
             "Move the cursor relative to its current position")
        .def("move_to", &CursorControl::MoveCursorTo, 
             py::arg("x"), py::arg("y"),
             "Move the cursor to an absolute position")
        .def("get_position", &CursorControl::GetPosition,
             "Get the current cursor position as (x, y) pair")
        .def("set_position", &CursorControl::SetPosition,
             py::arg("x"), py::arg("y"),
             "Set the cursor position (same as move_to)")
        .def_property("enabled", 
                     &CursorControl::IsEnabled, 
                     &CursorControl::SetEnabled,
                     "Enable or disable cursor movement")
        
        // Convenience functions for directional movement
        .def("move_up", [](CursorControl &self, int pixels) { 
                return self.MoveCursor(0, -pixels); 
             }, py::arg("pixels") = 5,
             "Move the cursor up by a number of pixels")
        .def("move_down", [](CursorControl &self, int pixels) { 
                return self.MoveCursor(0, pixels); 
             }, py::arg("pixels") = 5,
             "Move the cursor down by a number of pixels")
        .def("move_left", [](CursorControl &self, int pixels) { 
                return self.MoveCursor(-pixels, 0); 
             }, py::arg("pixels") = 5,
             "Move the cursor left by a number of pixels")
        .def("move_right", [](CursorControl &self, int pixels) { 
                return self.MoveCursor(pixels, 0); 
             }, py::arg("pixels") = 5,
             "Move the cursor right by a number of pixels");
    
    // Module-level convenience functions
    m.def("move", [](int delta_x, int delta_y) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        return controller.MoveCursor(delta_x, delta_y);
    }, py::arg("delta_x"), py::arg("delta_y"),
    "Quick function to move the cursor relative to its current position");
    
    m.def("move_to", [](int x, int y) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        return controller.MoveCursorTo(x, y);
    }, py::arg("x"), py::arg("y"),
    "Quick function to move the cursor to an absolute position");
    
    m.def("get_position", []() {
        static CursorControl controller;
        return controller.GetPosition();
    }, "Quick function to get the current cursor position as (x, y) pair");
    
    // Directional convenience functions
    m.def("up", [](int pixels) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        return controller.MoveCursor(0, -pixels);
    }, py::arg("pixels") = 5, "Move the cursor up by a number of pixels");
    
    m.def("down", [](int pixels) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        return controller.MoveCursor(0, pixels);
    }, py::arg("pixels") = 5, "Move the cursor down by a number of pixels");
    
    m.def("left", [](int pixels) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        return controller.MoveCursor(-pixels, 0);
    }, py::arg("pixels") = 5, "Move the cursor left by a number of pixels");
    
    m.def("right", [](int pixels) {
        static CursorControl controller;
        static bool initialized = false;
        
        if (!initialized) {
            controller.Initialize();
            initialized = true;
        }
        
        return controller.MoveCursor(pixels, 0);
    }, py::arg("pixels") = 5, "Move the cursor right by a number of pixels");
} 