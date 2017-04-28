#ifndef SDLDISPALY_H
#define SDLDISPALY_H

#define SDL_MAIN_HANDLED
extern "C"
{
#include<stdio.h>
#include<SDL.h>
}
class SDLDispaly
{
public:
    SDLDispaly();
    bool Init();
    bool UnInit();

private:
    //SDL vars
    int screen_w=0, screen_h=0;
    SDL_Window *screen=NULL;
    SDL_Renderer* sdlRenderer=NULL;
    SDL_Texture* sdlTexture=NULL;
    SDL_Rect sdlRect;
    SDL_Thread *video_tid=NULL;
    SDL_Event event;
};

#endif // SDLDISPALY_H
