#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct point {
    int x, y;
};

SDL_Window *create_window(int, int, const char *);
SDL_Renderer *create_renderer(SDL_Window *);
SDL_Texture *create_image(SDL_Renderer *, const char *);
void draw_image(SDL_Texture *, SDL_Renderer *);
void draw_image_clip(SDL_Texture *, SDL_Renderer *, SDL_Rect,
		     struct point);

#endif
