#include "pong.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cmath>
#include <iostream>
#include <random>

#define RES_WIDTH 640
#define RES_HEIGHT 480

void drawPixel(SDL_Point pos, SDL_Color color, Uint8* pixelptr){
    pixelptr = pixelptr + ((pos.y * RES_WIDTH + pos.x)*4);
    *pixelptr = color.b;
    *(pixelptr+1) = color.g;
    *(pixelptr+2) = color.r;
    *(pixelptr+3) = color.a;
}

void drawCircle(SDL_Point center, int radius, SDL_Surface* surface, SDL_Color color){
    int r = radius;
    int x = r-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (r << 1);
    while ( x >= y ) {
        drawPixel({x+center.x, y+center.y}, color, (Uint8*)surface->pixels);
        drawPixel({x+center.x, -y+center.y}, color, (Uint8*)surface->pixels);
        drawPixel({-x+center.x, y+center.y}, color, (Uint8*)surface->pixels);
        drawPixel({-x+center.x, -y+center.y}, color, (Uint8*)surface->pixels);
        drawPixel({y+center.x, x+center.y}, color, (Uint8*)surface->pixels);
        drawPixel({y+center.x, -x+center.y}, color, (Uint8*)surface->pixels);
        drawPixel({-y+center.x, x+center.y}, color, (Uint8*)surface->pixels);
        drawPixel({-y+center.x, -x+center.y}, color, (Uint8*)surface->pixels);
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

Ball::Ball(int radius, SDL_Point pos, vector velocity){
    this->radius = radius;
    this->pos = pos;
    this->velocity = velocity;
    this->color = {0,0,255,0};
}


void Ball::draw(SDL_Surface* surface){
    drawCircle(this->pos, this->radius, surface, this->color);
}

void Ball::update(){
    this->pos.x += this->velocity.x;
    this->pos.y += this->velocity.y;
    if (this->pos.x + this->radius > 640){
        this->pos.x = 640 - this->radius;
        this->velocity.x  *= -1;
    }
    if (this->pos.y + this->radius > 480){
        this->pos.y = 480 - this->radius;
        this->velocity.y  *= -1;
    }
    if (this->pos.x - this->radius < 0){
        this->pos.x = 0 + this->radius;
        this->velocity.x  *= -1;
    }
    if (this->pos.y - this->radius < 0){
        this->pos.y = 0 + this->radius;
        this->velocity.y  *= -1;
    }
}

Pong::Pong(){
    this->running = true;
    this->ball = new Ball(8, {320,240}, {8,8});
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
        this->surface = SDL_GetWindowSurface(this->window);
    }
    else {
        this->running = false;
        return;
    }
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
    SDL_FillRect(this->surface, NULL, 0x000000);
    this->ball->draw(this->surface);
    SDL_UpdateWindowSurface(this->window);
}

void Pong::update(){ 
    this->ball->update();
}
