#include "sdldispaly.h"

SDLDispaly::SDLDispaly()
{}

bool SDLDispaly::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
    {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool SDLDispaly::UnInit()
{
    SDL_Quit();
    return true;
}


int SDLDispaly::SetHeight(int height)
{
    int oldHeight;
    oldHeight=screen_h;
    screen_h=height;
    return oldHeight;
}

int SDLDispaly::SetWidth(int width)
{
    int oldWidth;
    oldWidth=screen_w;
    screen_w=width;
    return oldWidth;
}
