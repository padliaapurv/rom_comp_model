cmake_minimum_required(VERSION 3.10)
project(rom_comp_model)

set(CMAKE_CXX_STANDARD 14)

find_package(Eigen3 REQUIRED)  # Adjust if needed

include_directories(${EIGEN3_INCLUDE_DIRS} include)

file(GLOB SOURCES src/*.cpp main.cpp)
add_executable(rom_exe ${SOURCES})
target_link_libraries(rom_exe ${EIGEN3_LIBRARIES})
