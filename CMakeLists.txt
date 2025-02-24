cmake_minimum_required(VERSION 3.10)
project(rom_comp_model LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find Eigen
find_package(Eigen3 REQUIRED)  # This sets Eigen3::Eigen target

# 2. Library sources
file(GLOB ROM_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/rom/src/*.cpp
)

# 3. Create library
add_library(rom_lib ${ROM_SOURCES})

# 4. Link Eigen and add includes for your library
target_link_libraries(rom_lib
    PUBLIC
        Eigen3::Eigen
)
target_include_directories(rom_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/rom/include  # So #include "rom/DEIM.h" works
)

# 5. Create executable
add_executable(rom_exe main.cpp)
target_link_libraries(rom_exe PRIVATE rom_lib)
