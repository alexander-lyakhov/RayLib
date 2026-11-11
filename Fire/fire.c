#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TARGET_FPS      30
#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT  768
/*
#define WHITE_TO_YELLOW  25 // 15
#define YELLOW_TO_RED    20 //  3
#define RED_TO_BLACK     25 //  5

#define OFFSET_RANDOM_FACTOR 40
#define REPEAT_RANDOM_FACTOR 25
*/

#define WHITE_TO_YELLOW 40
#define YELLOW_TO_RED   30
#define RED_TO_BLACK    20

#define OFFSET_RANDOM_FACTOR 30 // Flame height. Smallest value - heighest flame size
#define REPEAT_RANDOM_FACTOR 20 // Burning speed. Smallest value - fastest speed

#define OFFSET_FACTOR 1

Color color_black = {  0,   0,   0, 255};
Color color_teal  = {  0, 128, 128, 255};
Color color_green = {  0, 255,   0, 255};
Color color_red   = {255,   0,   0, 255};
Color color_white = {255, 255, 255, 255};

typedef struct {
	Color *pixels;
	int    screen_width;
	int    screen_height;
	size_t screen_size;
	Color *screen_base_line;
	Color *screen_next_line;
	Color *screen_last_pixel;
	unsigned char *randoms;
} App;

typedef void (*Frame)(App *app, Color *curr_pixel, Color *prev_pixel, int random_value);

// =============================================================================
// @@@ + App_fill_randoms
// =============================================================================
void App_fill_randoms(App *app)
{
	for (size_t i = 0; i < app->screen_size; i++)
		app->randoms[i] = GetRandomValue(0, 99);
}

// =============================================================================
// @@@ + App_init_frame
// =============================================================================
void App_init_frame(App *app)
{
	app->screen_base_line  = app->pixels + app->screen_size - app->screen_width;
	app->screen_next_line  = app->screen_base_line - app->screen_width;
	app->screen_last_pixel = app->pixels + app->screen_size - 2;

	for (size_t i = 0; i < app->screen_size; i++)
		app->randoms[i] = GetRandomValue(0, 99);

	Color *p = app->screen_base_line;

	for (int i = 0; i < app->screen_width; i++)
		*p++ = color_white;

	/*p = app->pixels + WINDOW_WIDTH * 300;

	for (int i = 0; i < WINDOW_WIDTH; i++)
		*p++ = color_white;*/
}

// =============================================================================
// @@@ + App_pixel_fade
// =============================================================================
Color* App_pixel_fade(Color *color)
{
	if (!color->b && !color->g && color->r)
	{
		if (color->r < 64)
			color->r--;
		else
			color->r -= color->r > RED_TO_BLACK
				? RED_TO_BLACK
				: color->r;
		
		return color;
	}

	if (!color->b && color->g)
	{
		color->g -= color->g > YELLOW_TO_RED
			? YELLOW_TO_RED
			: color->g;
		
		return color;
	}

	if (color->b)
		color->b -= color->b > WHITE_TO_YELLOW
			? WHITE_TO_YELLOW
			: color->b;

	return color;
}

// =============================================================================
// @@@ + App_is_color_black
// =============================================================================
bool App_is_color_black(Color *color)
{
	return !color->r && !color->g && !color->b;
}

// =============================================================================
// @@@ + App_first_frame
// =============================================================================
void App_first_frame(App *app, Color *curr_pixel, Color *prev_pixel, int value)
{
	Color color = *prev_pixel;

	if (GetRandomValue(0, 99) > 50)
		App_pixel_fade(&color);

	*curr_pixel = color;
}

// =============================================================================
// @@@ + App_next_frame
// =============================================================================
void App_next_frame(App *app, Color *curr_pixel, Color *prev_pixel, int value)
{
	// int value = GetRandomValue(0, 99);
	// int value = ((uintptr_t)prev_pixel) % x % 100;
	// int value = ((uintptr_t)prev_pixel ^ app->seed ^ curr_pixel->r ^ curr_pixel->g ^ curr_pixel->b) % 100;

	if (value > OFFSET_RANDOM_FACTOR)
	{
		*curr_pixel = prev_pixel[value % 3 - 1];
		return;
	}

	if (value > REPEAT_RANDOM_FACTOR)
	{
		Color color = *prev_pixel;
		*curr_pixel = *App_pixel_fade(&color);
	}
}

// =============================================================================
// @@@ + App_update
// =============================================================================
void App_update(App *app, Frame frame_callback)
{
	Color *prev_pixel = app->screen_last_pixel;
	Color *curr_pixel = prev_pixel - app->screen_width;

	for (unsigned char *rand_val = app->randoms ;curr_pixel >= app->pixels; rand_val++)
	{
		if (!App_is_color_black(prev_pixel))
			frame_callback(app, curr_pixel, prev_pixel, *rand_val);

		curr_pixel--;
		prev_pixel--;

		*rand_val = GetRandomValue(0, 99);
	}

	// App_fill_randoms(app);
}

int main(int argc, char *argv[])
{
	SetTraceLogLevel(LOG_NONE);

	SetTargetFPS(TARGET_FPS);
	// SetConfigFlags(FLAG_FULLSCREEN_MODE);

	argc == 2 && !strcmp(argv[1], "fullscreen")
		? InitWindow(GetRenderWidth(), GetRenderHeight(), "Flame")
		: InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Flame");

	int w = GetRenderWidth();
	int h = GetRenderHeight();

	Image image = GenImageColor(w, h, color_black);

	Texture2D texture = LoadTextureFromImage(image);

	App app = {
		.screen_width  = w,
		.screen_height = h,
		.screen_size   = w * h,
		.pixels        = LoadImageColors(image),
		.randoms       = malloc(w * h),
	};
	
	App_init_frame(&app);
	App_update(&app, App_first_frame);

	while(!WindowShouldClose() && !IsKeyDown(KEY_Q))
	{
		App_update(&app, App_next_frame);
		UpdateTexture(texture, app.pixels);

		BeginDrawing();
			DrawTexture(texture, 0, 0, color_white);
		EndDrawing();
	}
	
	UnloadTexture(texture);
	UnloadImage(image);
	UnloadImageColors(app.pixels);
	free(app.randoms);

	return 0;
}