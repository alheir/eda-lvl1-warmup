cmake_minimum_required (VERSION 3.8)

# Project orbitalsim
project(orbitalsim)

# Main executable
add_executable(orbitalsim main.cpp orbitalSim.cpp orbitalSimView.cpp)

# Raylib
find_package(raylib CONFIG REQUIRED)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_include_directories(orbitalsim PRIVATE ${RAYLIB_INCLUDE_DIRS})
    target_link_libraries(orbitalsim PRIVATE ${RAYLIB_LIBRARIES})
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(orbitalsim PRIVATE raylib m ${CMAKE_DL_LIBS} pthread GL rt X11)
endif()

# Main test
enable_testing()

add_executable(orbitalsim_test main_test.cpp orbitalSim.cpp)

add_test(NAME test1 COMMAND orbitalsim_test)

target_include_directories(orbitalsim_test PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(orbitalsim_test PRIVATE ${RAYLIB_LIBRARIES})