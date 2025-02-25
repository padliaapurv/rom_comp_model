cmake_minimum_required(VERSION 3.10)
project(navier2d_rom LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Find Eigen (header-only)
find_package(Eigen3 REQUIRED)

# Optionally include Eigen's headers manually (depending on your setup)
include_directories(${EIGEN3_INCLUDE_DIRS})

# Grab all .cpp files in src/
file(GLOB SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(navier2d_rom_exe ${SOURCES})

# Link to Eigen if needed
target_link_libraries(navier2d_rom_exe Eigen3::Eigen)
