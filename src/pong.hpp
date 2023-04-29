#ifndef PONG
#define PONG

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_mixer.h>
#include <vector>

class Pong;

struct vector {
    float x;
    float y;
} typedef vector;

class Mixer {
public:
    Mixer();
    Mix_Music* pong = NULL;
};

class Ball {
public:
    Ball(int radius, SDL_Point pos, vector velocity);
    void draw(SDL_Renderer* renderer);
    int radius;
    SDL_Point pos;
    SDL_Color color;
    vector velocity;
};

class PlayerBar {
public:
    PlayerBar(SDL_Point pos, SDL_Color color);
    void draw(SDL_Renderer* renderer);
    SDL_Rect rect;
    SDL_Color color;
    bool upPressed;
    bool downPressed;
    int points;
};

class Physics {
public:
    Physics(Ball* ball, PlayerBar* player1, PlayerBar* player2, Pong* game);
    void update();
    float barSpeed;
    Pong* game;
    Ball* ball;
    PlayerBar* player1;
    PlayerBar* player2;
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
    Mixer* mixer;
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event currentEvent;
    Ball* ball;
    PlayerBar* player1;
    PlayerBar* player2;
    Physics* physics;
};

#endif
