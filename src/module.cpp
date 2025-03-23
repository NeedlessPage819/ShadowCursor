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
                     &CursorControl::SetEnabled);
        
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
} 