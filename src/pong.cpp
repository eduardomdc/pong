#include "pong.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cmath>
#include <iostream>
#include <random>

void drawCircle(SDL_Point center, int radius, SDL_Renderer* renderer){
    int r = radius;
    int x = r-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (r << 1);
    // int pointCount = r*4;
    // int pointIndex = 0;
    // SDL_Point* points = new SDL_Point[pointCount];
    while ( x >= y ) {
        SDL_RenderDrawPoint(renderer, x+center.x, y+center.y);
        SDL_RenderDrawPoint(renderer, x+center.x, -y+center.y);
        SDL_RenderDrawPoint(renderer, -x+center.x, y+center.y);
        SDL_RenderDrawPoint(renderer, -x+center.x, -y+center.y);
        SDL_RenderDrawPoint(renderer, y+center.x, x+center.y);
        SDL_RenderDrawPoint(renderer, y+center.x, -x+center.y);
        SDL_RenderDrawPoint(renderer, -y+center.x, x+center.y);
        SDL_RenderDrawPoint(renderer, -y+center.x, -x+center.y);
        if ( err <= 0){
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0){
            x--;
            dx +=2;
            err += dx - (r << 1);
        }
    }
}

Pong::Pong(){
    this->running = true;
}

void Pong::initSDL(){
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        this->window = SDL_CreateWindow("conway's game",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                640,
                480,
                SDL_WINDOW_BORDERLESS
                );
        this->renderer = SDL_CreateRenderer(this->window, -1, 0);
    }
    else return;
    if (this->renderer){
        SDL_SetRenderDrawColor(this->renderer, 0,0,0,255);
        std::cout << "Renderer Created!"<<std::endl;
    }
}

void Pong::closeSDL(){
    SDL_DestroyWindow(this->window);
    SDL_DestroyRenderer(this->renderer);
    SDL_Quit();
    std::cout << "SDL cleaned out." << std::endl;
}

void Pong::getInput(){
    while(SDL_PollEvent(&this->currentEvent)){
        switch(this->currentEvent.type){
        case SDL_QUIT:
            this->closeSDL();
            this->running = false;
            return;
        }
    }
}

void Pong::render(){ 
    SDL_RenderClear(this->renderer);
    SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
    drawCircle({100,100}, 8, this->renderer);
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderPresent(this->renderer); 
}

void Pong::update(){ 
    
}
