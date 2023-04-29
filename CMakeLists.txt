cmake_minimum_required(VERSION 3.1...3.26)
project(
    pong
    VERSION 0.1
    LANGUAGES CXX)

set(SDL_MIXER_INCLUDE_DIRS /usr/include/SDL2)
set(SDL_MIXER_LIBRARIES /usr/lib/x86_64-linux-gnu/libSDL2_mixer.so)
# find_package(SDL_mixer REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL_MIXER_INCLUDE_DIRS})
add_executable(pong src/main.cpp src/pong.cpp)
target_link_libraries(pong ${SDL2_LIBRARIES} ${SDL_MIXER_LIBRARIES})
