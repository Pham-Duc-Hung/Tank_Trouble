#ifndef INIT_H
#define INIT_H

#include "common.h"

void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture* loadTexture(const char* path);
void loadMedia();

#endif // INIT_H
