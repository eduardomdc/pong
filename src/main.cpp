#include "main.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

const int fps = 60;
Uint32 frameLength = 1000/fps;

int main(){
	Pong* game = new Pong();
    Uint32 currentTime = SDL_GetTicks();
    Uint32 lastFrame = currentTime;
    Uint32 diff;

    game->initSDL();
	while(game->running){
		game->render();
        game->update();
        game->getInput();
        currentTime = SDL_GetTicks();
        diff = currentTime - lastFrame;
        lastFrame = currentTime;
        if (diff < frameLength){
            SDL_Delay(frameLength - diff);
        }
	}
	return 0;
}
