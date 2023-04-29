#include "pong.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_mixer.h>
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

Mixer::Mixer(){
    this->pong = Mix_LoadMUS("sounds/press.mp3");
    if (this->pong == NULL){
        std::cout << Mix_GetError() << std::endl;
    }
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

PlayerBar::PlayerBar(SDL_Point pos, SDL_Color color){
    this->rect = {pos.x, pos.y, 20, 80};
    this->color = color;
    this->points = 0;
}

void PlayerBar::draw(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, this->color.r, this->color.g, this->color.b, this->color.a);
    SDL_RenderFillRect(renderer, &this->rect);
}

Physics::Physics(Ball* ball, PlayerBar* player1, PlayerBar* player2, Pong* game){
    this->ball = ball;
    this->player1 = player1;
    this->player2 = player2;
    this->barSpeed = 2;
    this->game = game;
}

void Physics::update(){
    this->ball->pos.x += this->ball->velocity.x;
    this->ball->pos.y += this->ball->velocity.y;
    //playerbar velocity
    this->player1->rect.y += this->barSpeed*(this->player1->downPressed-this->player1->upPressed);
    this->player2->rect.y += this->barSpeed*(this->player2->downPressed-this->player2->upPressed);
    // collision with window
    if (this->ball->pos.x + this->ball->radius > RES_WIDTH){
        this->ball->pos.x = RES_WIDTH - this->ball->radius;
        // this->ball->velocity.x  *= -1;
        // point for player1
        this->player1->points++;
        Mix_PlayMusic(this->game->mixer->pong, -1);
        this->ball->pos = {RES_WIDTH/2,RES_HEIGHT/2};
        this->ball->color = {255, 255, 255, 255};
        this->ball->velocity = {1,2};
        this->barSpeed = 2;
    }
    if (this->ball->pos.y + this->ball->radius > RES_HEIGHT){
        this->ball->pos.y = RES_HEIGHT - this->ball->radius;
        this->ball->velocity.y  *= -1;
        Mix_PlayMusic(this->game->mixer->pong, 0);
    }
    if (this->ball->pos.x - this->ball->radius < 0){
        this->ball->pos.x = 0 + this->ball->radius;
        // point for player2
        this->player2->points++;
        this->ball->pos = {RES_WIDTH/2,RES_HEIGHT/2};
        this->ball->color = {255, 255, 255, 255};
        this->ball->velocity = {-1,-2};
        this->barSpeed = 2;
        Mix_PlayMusic(this->game->mixer->pong, 0);
    }
    if (this->ball->pos.y - this->ball->radius < 0){
        this->ball->pos.y = 0 + this->ball->radius;
        this->ball->velocity.y  *= -1;
        Mix_PlayMusic(this->game->mixer->pong, 0);
    }
    // collision with player bars
    if (this->ball->pos.x - this->ball->radius < this->player1->rect.x + this->player1->rect.w
        && this->ball->pos.x > this->player1->rect.x
        && this->ball->pos.y > this->player1->rect.y
        && this->ball->pos.y < this->player1->rect.y+this->player1->rect.h
        && this->ball->velocity.x < 0
        ){
        this->ball->pos.x = this->player1->rect.x + this->player1->rect.w + this->ball->radius;
        this->ball->color = this->player1->color;
        this->ball->velocity.x  *= -1.1;//ball speeds up
        Mix_PlayMusic(this->game->mixer->pong, 0);
        this->barSpeed = 2*sqrt(abs(this->ball->velocity.x));
    }
    if (this->ball->pos.x + this->ball->radius > this->player2->rect.x
        && this->ball->pos.x < this->player2->rect.x + this->player2->rect.w
        && this->ball->pos.y > this->player2->rect.y
        && this->ball->pos.y < this->player2->rect.y+this->player2->rect.h
        && this->ball->velocity.x > 0
        ){
        this->ball->pos.x = this->player2->rect.x - this->ball->radius;
        this->ball->color = this->player2->color;
        this->ball->velocity.x  *= -1.1;
        Mix_PlayMusic(this->game->mixer->pong, 0);
        this->barSpeed = 2*sqrt(abs(this->ball->velocity.x));
    }
}

Pong::Pong(){
    this->running = true;
    this->ball = new Ball(8, {RES_WIDTH/2,RES_HEIGHT/2}, {1,2});
    this->player1 = new PlayerBar({40, 200}, {0,0,255,0});
    this->player2 = new PlayerBar({580, 200}, {255,0,0,0});
    this->physics = new Physics(this->ball, this->player1, this->player2, this);
}

void Pong::initSDL(){
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        this->window = SDL_CreateWindow("Pong!",
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
        std::cout << "Could not initialize SDL."<<std::endl;
        return;
    }
    // if (Mix_Init(MIX_INIT_MP3) == 0){
    //     this->running = false;
    //     std::cout << Mix_GetError() << std::endl;
    // }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        this->running = false;
        std::cout << "Could not initialize SDL Mixer."<<std::endl;
        return;
    }
    // if (Mix_OpenAudioDevice(44100, MIX_DEFAULT_FORMAT, 2, 2048, NULL,
    //     SDL_AUDIO_ALLOW_FREQUENCY_CHANGE |
    //     SDL_AUDIO_ALLOW_CHANNELS_CHANGE) < 0){
    //     this->running = false;
    //     std::cout << Mix_GetError() << std::endl;
    // }
    // int* freq = new int;
    // Uint16* format = new Uint16;
    // int* channels = new int;
    // Mix_QuerySpec(freq, format, channels);
    // std::cout <<"frequency: " << *freq << " format: " << *format << " channels: " << *channels << std::endl;
    this->mixer = new Mixer();
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
        case SDL_KEYDOWN:
            switch(currentEvent.key.keysym.sym){
            case SDLK_ESCAPE:
                this->closeSDL();
                this->running = false;
                return;
            case SDLK_UP:
                this->player2->upPressed = true;
                break;
            case SDLK_DOWN:
                this->player2->downPressed = true;
                break;
            case SDLK_w:
                this->player1->upPressed = true;
                break;
            case SDLK_s:
                this->player1->downPressed = true;
                break;
            }
            break;
        case SDL_KEYUP:
            switch(currentEvent.key.keysym.sym){
            case SDLK_UP:
                this->player2->upPressed = false;
                break;
            case SDLK_DOWN:
                this->player2->downPressed = false;
                break;
            case SDLK_w:
                this->player1->upPressed = false;
                break;
            case SDLK_s:
                this->player1->downPressed = false;
                break;
            }
            break;
        }
    }
}

void Pong::render(){
    SDL_RenderClear(this->renderer);
    this->ball->draw(this->renderer);
    this->player1->draw(this->renderer);
    for (int i = 0; i < this->player1->points; i++){
        SDL_RenderDrawLine(this->renderer, 10+10*i, 10, 10+10*i, 30);
    }
    this->player2->draw(this->renderer);
    for (int i = 0; i < this->player2->points; i++){
        SDL_RenderDrawLine(this->renderer, RES_WIDTH-10+10*(-i), 10, RES_WIDTH-10+10*(-i), 30);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderPresent(this->renderer);
}

void Pong::update(){ 
    this->physics->update();
}