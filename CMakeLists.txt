cmake_minimum_required(VERSION 3.1...3.26)
project(
    pong
    VERSION 0.1
    LANGUAGES CXX)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(pong src/main.cpp src/pong.cpp)
target_link_libraries(pong ${SDL2_LIBRARIES})
