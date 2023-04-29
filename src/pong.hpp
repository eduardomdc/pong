#ifndef PONG
#define PONG

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <vector>

struct vector {
    float x;
    float y;
} typedef vector;

class Ball {
public:
    Ball(int radius, SDL_Point pos, vector velocity);
    void update();
    void draw(SDL_Renderer* renderer);
    int radius;
    SDL_Point pos;
    SDL_Color color;
    vector velocity;
};

class PlayerBar {
public:
    PlayerBar();
    void update();
    void draw(SDL_Renderer* renderer);
    SDL_Rect rect;
    bool upPressed;
    bool downPressed;
};

class Physics {
public:
    Physics(Ball* ball, PlayerBar* player1);
    void update();
    Ball* ball;
    PlayerBar* player1;
};

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
    Ball* ball;
    PlayerBar* player1;
    Physics* physics;
};

#endif
