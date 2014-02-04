project(shinyavenger)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake-Modules/")

include(EnableCPP11Flag)
ENABLE_CPP11()

find_package(GLFW REQUIRED)
find_package(Springbok REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${SPRINGBOK_INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIR})

add_executable(shinyavenger main.cpp)