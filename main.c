#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "draw.h"

#define FPS 60
#define SPEED 5
#define VELOCITY 1
#define SCREEN_W 512
#define SCREEN_H 448
#define SPRITE_SIZE 32
#define MAP_W 16
#define MAP_H 14
#define MAX_BODY_SIZE MAP_W * MAP_H
#define INITIAL_BODY_SIZE 2

int points_collide(struct point a, struct point b)
{
    if (a.x == b.x && a.y == b.y) {
	return 1;
    }

    return 0;
}

int eats_tail(struct point *body, int body_size)
{
    for (int i = 1; i < body_size; i++) {
	if (points_collide(body[0], body[i])) {
	    return 1;
	}
    }

    return 0;
}

struct point map_coord_to_screen_coord(struct point coord)
{
    return (struct point) { coord.x * 32, coord.y * 32 };
}

struct point random_apple_coord(struct point *body, int body_size)
{
    int touches = 0;
    int x, y;

    do {
	x = rand() % MAP_W;
	y = rand() % MAP_H;

	for (int i = 0; i < body_size; i++) {
	    if (points_collide((struct point) { x, y }, body[i])) {
		touches = 1;
		break;
	    } else {
		touches = 0;
	    }
	}
    }
    while (touches);

    return (struct point) { x, y };
}

int handle_key(SDL_Event e, int *xvel, int *yvel)
{
    if (e.type == SDL_QUIT)
	return 1;

    if (e.key.repeat == 0) {
	if (e.type == SDL_KEYDOWN) {
	    switch (e.key.keysym.sym) {
	    case SDLK_UP:
	    case SDLK_w:
		*yvel = -VELOCITY;
		*xvel = 0;
		break;
	    case SDLK_DOWN:
	    case SDLK_s:
		*yvel = VELOCITY;
		*xvel = 0;
		break;
	    case SDLK_LEFT:
	    case SDLK_a:
		*yvel = 0;
		*xvel = -VELOCITY;
		break;
	    case SDLK_RIGHT:
	    case SDLK_d:
		*yvel = 0;
		*xvel = VELOCITY;
		break;
	    case SDLK_SPACE:
		*yvel = 0;
		*xvel = 0;
	    }
	}
    }

    return 0;
}

void update_body_parts(struct point *body, int body_size)
{
    for (int i = body_size - 1; i > 0; i--) {
	body[i] = body[i - 1];
    }
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *snake_img;
    SDL_Texture *apple_img;

    const int ticks_per_frame = 1000 / FPS;
    int xvel = 0, yvel = 0;
    int last_mov = 0;

    Uint32 frame_start;
    int frame_time;

    SDL_Event e;
    int quit = 0;

    int body_size = INITIAL_BODY_SIZE;
    struct point *snake_body =
	(struct point *) malloc(MAX_BODY_SIZE * sizeof(struct point));
    struct point apple = random_apple_coord(snake_body, body_size);

    window = create_window(SCREEN_W, SCREEN_H, "Snake");
    renderer = create_renderer(window);
    snake_img = create_image(renderer, "assets/snake.png");
    apple_img = create_image(renderer, "assets/apple.png");
    SDL_Rect sprite_rect = { 0, 0, SPRITE_SIZE, SPRITE_SIZE };

    snake_body[0] = (struct point) { MAP_W / 2 - 1, MAP_H / 2 - 1 };
    snake_body[1] = (struct point) { MAP_W / 2, MAP_H / 2 - 1 };

    while (!quit) {
	frame_start = SDL_GetTicks();

	while (SDL_PollEvent(&e)) {
	    quit = handle_key(e, &xvel, &yvel);
	}

	if (last_mov > 10 - SPEED && (xvel != 0 || yvel != 0)) {
	    update_body_parts(snake_body, body_size);
	    snake_body[0].x += xvel;
	    snake_body[0].y += yvel;
	    last_mov = 0;
	} else {
	    last_mov++;
	}

	if (points_collide(snake_body[0], apple)) {
	    snake_body[body_size] = (struct point)
		{ snake_body[body_size - 1].x, snake_body[body_size - 1].y };
	    body_size++;
	    apple = random_apple_coord(snake_body, body_size);
	    printf("%d\n", (body_size - INITIAL_BODY_SIZE) * (10 -SPEED));
	}

	if ((snake_body[0].x > MAP_W - 1 || snake_body[0].x < 0)
	    || (snake_body[0].y > MAP_H - 1 || snake_body[0].y < 0)
	    || eats_tail(snake_body, body_size)) {
	    snake_body[0] = (struct point) { MAP_W / 2 - 1, MAP_H / 2 - 1 };
	    snake_body[1] = (struct point) { MAP_W / 2, MAP_H / 2 - 1 };
	    xvel = 0;
	    yvel = 0;
	    last_mov = 0;
	    body_size = INITIAL_BODY_SIZE;
	    apple = random_apple_coord(snake_body, body_size);
	    printf("Game over\n");
	}

	SDL_RenderClear(renderer);
	draw_image_clip(apple_img, renderer, sprite_rect,
			map_coord_to_screen_coord(apple));

	for (int i = 0; i < body_size; i++) {
	    draw_image_clip(snake_img, renderer, sprite_rect,
			    map_coord_to_screen_coord(snake_body[i]));
	}

	SDL_RenderPresent(renderer);

	frame_time = SDL_GetTicks() - frame_start;

	if (ticks_per_frame > frame_time) {
	    SDL_Delay(ticks_per_frame - frame_time);
	}
    }

    free(snake_body);
    SDL_DestroyTexture(snake_img);
    SDL_DestroyTexture(apple_img);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
