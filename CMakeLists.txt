project(shinyavenger)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake-Modules/")

include(EnableCPP11Flag)
ENABLE_CPP11()

find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(Springbok REQUIRED)

include_directories(${SPRINGBOK_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

add_executable(shinyavenger main.cpp Source/Behavior.cpp Source/Bullet.cpp Source/GravitySource.cpp Source/Particle.cpp Source/Game.cpp Source/Ship.cpp Source/PlaySpace.cpp Source/UI.cpp Source/Sector.cpp Source/Leader.h Source/GravitySource.h Source/PhysicsObject.h Source/Space.h Source/Game.h Source/PlaySpace.h Source/Bullet.h Source/Ship.h Source/UI.h Source/Particle.h Source/Sector.h)
target_link_libraries(shinyavenger ${GLFW_LIBRARY} ${SPRINGBOK_LIBRARY} ${OPENGL_LIBRARY})