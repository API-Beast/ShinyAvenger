project(shinyavenger)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake-Modules/")

include(EnableCPP11Flag)
ENABLE_CPP11()

find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(Springbok REQUIRED)

include_directories(${SPRINGBOK_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

add_executable(shinyavenger main.cpp Source/Game.cpp Source/Player.cpp Source/PlaySpace.cpp Source/PhysicsObject.h Source/Space.h Source/Game.h Source/Player.h Source/EnemyBehavior.h Source/Enemy.h Source/PlaySpace.h)
target_link_libraries(shinyavenger ${GLFW_LIBRARY} ${SPRINGBOK_LIBRARY} ${OPENGL_LIBRARY})
