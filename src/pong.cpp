#include "pong.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cmath>
#include <iostream>
#include <random>

#define RES_WIDTH 640
#define RES_HEIGHT 480

void drawCircle(SDL_Point center, int radius, SDL_Renderer* renderer, SDL_Color color){
    /** how many points does a circle with r radius has?
     * pass this into ball class
     * **/
    SDL_Point* points = new SDL_Point[radius*10];
    int pointsIndex = 0;
    int r = radius;
    int x = r-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (r << 1);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    while ( x >= y ) {
        *(points+pointsIndex) = SDL_Point({x+center.x, y+center.y});
        pointsIndex++;
        *(points+pointsIndex) = SDL_Point({x+center.x, -y+center.y});
        pointsIndex++;
        *(points+pointsIndex) = SDL_Point({-x+center.x, y+center.y});
        pointsIndex++;
        *(points+pointsIndex) = SDL_Point({-x+center.x, -y+center.y});
        pointsIndex++;
        *(points+pointsIndex) = SDL_Point({y+center.x, x+center.y});
        pointsIndex++;
        *(points+pointsIndex) = SDL_Point({y+center.x, -x+center.y});
        pointsIndex++;
        *(points+pointsIndex) = SDL_Point({-y+center.x, x+center.y});
        pointsIndex++;
        *(points+pointsIndex) = SDL_Point({-y+center.x, -x+center.y});
        pointsIndex++;
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
    SDL_RenderDrawPoints(renderer, points, pointsIndex);
    delete [] points;
}

Ball::Ball(int radius, SDL_Point pos, vector velocity){
    this->radius = radius;
    this->pos = pos;
    this->velocity = velocity;
    this->color = {255,255,255,0};
}


void Ball::draw(SDL_Renderer* renderer){
    drawCircle(this->pos, this->radius, renderer, this->color);
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
    this->ball = new Ball(8, {320,240}, {1,1});
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
    SDL_RenderClear(this->renderer);
    this->ball->draw(this->renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderPresent(this->renderer);
}

void Pong::update(){ 
    this->ball->update();
}
