cmake_minimum_required (VERSION 3.8)

# Project
project(orbitalsim)

# raylib: required for macOS
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_STANDARD 11)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

# Sources
set(SOURCES main.cpp OrbitalSim.cpp OrbitalBody.cpp)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Raylib
find_package(raylib CONFIG REQUIRED)

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

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${RAYLIB_LIBRARIES} raylib_cpp)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib m ${CMAKE_DL_LIBS} pthread GL rt X11 raylib_cpp)
endif()
