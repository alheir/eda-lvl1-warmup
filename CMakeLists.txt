cmake_minimum_required (VERSION 3.8)

# Project orbitalsim
project(orbitalsim)

# Main executable
add_executable(orbitalsim main.cpp OrbitalSim.cpp OrbitalBody.cpp)

# Raylib
find_package(raylib CONFIG REQUIRED)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_include_directories(orbitalsim PRIVATE ${RAYLIB_INCLUDE_DIRS})
    target_link_libraries(orbitalsim PRIVATE ${RAYLIB_LIBRARIES} raylib raylib_cpp)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(orbitalsim PRIVATE raylib raylib_cpp m ${CMAKE_DL_LIBS} pthread GL rt X11)
endif()

# raylib-cpp
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v4.1.0
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()
