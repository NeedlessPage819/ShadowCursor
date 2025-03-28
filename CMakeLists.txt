cmake_minimum_required(VERSION 3.10)
project(py_mouse_control)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "pybind11_DIR: ${pybind11_DIR}")

find_package(Python COMPONENTS Interpreter Development REQUIRED)
message(STATUS "Found Python: ${Python_EXECUTABLE}")

if(pybind11_DIR)
    message(STATUS "Using specified pybind11_DIR: ${pybind11_DIR}")
    find_package(pybind11 CONFIG REQUIRED)
else()
    find_package(pybind11 CONFIG QUIET)
    
    if(NOT pybind11_FOUND)
        if(DEFINED ENV{VCPKG_ROOT})
            set(VCPKG_INSTALLED_DIR "$ENV{VCPKG_ROOT}/installed")
            if(EXISTS "${VCPKG_INSTALLED_DIR}")
                file(GLOB VCPKG_ARCH_DIRS "${VCPKG_INSTALLED_DIR}/*")
                foreach(ARCH_DIR ${VCPKG_ARCH_DIRS})
                    set(PYBIND11_DIR "${ARCH_DIR}/share/pybind11")
                    if(EXISTS "${PYBIND11_DIR}")
                        message(STATUS "Found pybind11 in vcpkg: ${PYBIND11_DIR}")
                        list(APPEND CMAKE_PREFIX_PATH "${PYBIND11_DIR}")
                        find_package(pybind11 CONFIG REQUIRED)
                        break()
                    endif()
                endforeach()
            endif()
        endif()
    endif()
    
    if(NOT pybind11_FOUND)
        message(FATAL_ERROR "Could not find pybind11. Please install using 'vcpkg install pybind11:x64-windows'")
    endif()
endif()

message(STATUS "Found pybind11 v${pybind11_VERSION}")

if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)
endif()

pybind11_add_module(mouse_control 
    src/module.cpp
    src/cursor_control.cpp
    src/driver.cpp
)

if(WIN32)
    target_link_libraries(mouse_control PRIVATE user32)
endif()

target_include_directories(mouse_control PRIVATE include)

set_target_properties(mouse_control PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

add_custom_command(TARGET mouse_control POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy 
        "$<TARGET_FILE:mouse_control>" 
        "${CMAKE_CURRENT_SOURCE_DIR}"
)