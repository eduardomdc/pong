#ifndef PONG
#define PONG

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <vector>

struct pos {
    int x;
    int y;
} typedef pos;

class Pong {
public:
    Pong();
    void initSDL();
    void render();
    void update();
    void closeSDL();
    void getInput();
    bool running;
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event currentEvent;
};

#endif
