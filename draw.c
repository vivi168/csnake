#include <stdlib.h>
#include <stdio.h>

#include "draw.h"

SDL_Window *create_window(int screen_w, int screen_h, const char *title)
{
    SDL_Window *window;
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
			      SDL_WINDOWPOS_CENTERED,
			      screen_w, screen_h, SDL_WINDOW_SHOWN);

    if (window == NULL) {
	fprintf(stderr, "%s", "Error while creating SDL_Window\n");
	exit(EXIT_FAILURE);
    }

    return window;
}

SDL_Renderer *create_renderer(SDL_Window * window)
{
    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
	fprintf(stderr, "%s", "Error while creating SDL_Renderer\n");
	exit(EXIT_FAILURE);
    }

    return renderer;
}

SDL_Texture *create_image(SDL_Renderer * renderer, const char *path)
{
    SDL_Texture *texture;
    SDL_Surface *surface;
    surface = SDL_LoadBMP(path);
    if (surface == NULL) {
	fprintf(stderr, "%s : %s\n", "Error while creating SDL_Surface", path);
	exit(EXIT_FAILURE);
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture == NULL) {
	fprintf(stderr, "%s : %s\n", "Error while creating SDL_Image", path);
	exit(EXIT_FAILURE);
    }

    SDL_FreeSurface(surface);

    return texture;
}

void draw_image(SDL_Texture * texture, SDL_Renderer * renderer)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void
draw_image_clip(SDL_Texture * texture,
		SDL_Renderer * renderer, SDL_Rect clip, struct point dp)
{
    SDL_Rect dest = { dp.x, dp.y, clip.w, clip.h };

    SDL_RenderCopy(renderer, texture, &clip, &dest);
}
