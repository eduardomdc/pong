#include "pong.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cmath>
#include <iostream>
#include <random>

Pong::Pong(){
    this->running = true;
}

void Pong::initSDL(){
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        this->window = SDL_CreateWindow("conway's game",
                0,
                0,
                420,
                300,
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
    int r = 100;
    int center = 100;
    int x = r-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (r << 1);
    while ( x >= y ) {
        SDL_RenderDrawPointF(this->renderer, x+center, y+center);
        //int yChange = 2*y + 1;
        //int xChange = 1 - 2*x;
        //int radiusError = std::abs(x*x + y*y - r*r);
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
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderPresent(this->renderer); 
}

void Pong::update(){ 
    
}
