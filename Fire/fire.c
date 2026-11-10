#include "raylib.h"
#include <stdio.h>

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

#define WHITE_TO_YELLOW   40 // 15
#define YELLOW_TO_RED     30 //
#define RED_TO_BLACK      20 //  5

#define OFFSET_RANDOM_FACTOR 30 // Flame height. Smallest value - heighest flame size
#define REPEAT_RANDOM_FACTOR 20 // Burning speed. Smallest value - fastest speed

#define OFFSET_FACTOR 1

Color color_black = {0, 0, 0, 255};
Color color_teal  = {0, 128, 128, 255};
Color color_green = {0, 255, 0, 255};
Color color_red   = {255, 0, 0, 255};
Color color_white = {255, 255, 255, 255};

typedef struct {
	Color *pixels;
	int    width;
	int    heigh;
	size_t size;
} App;

typedef void (*Frame)(App *app, Color *curr_pixel, Color *prev_pixel);

// =============================================================================
// @@@ + App_init_frame
// =============================================================================
void App_init_frame(App *app)
{
	Color *p = app->pixels + app->size - WINDOW_WIDTH;

	for (int i = 0; i < WINDOW_WIDTH; i++)
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
	if (color->b)
		color->b -= color->b > WHITE_TO_YELLOW
			? WHITE_TO_YELLOW
			: color->b;

	if (!color->b && color->g)
		color->g -= color->g > YELLOW_TO_RED
			? YELLOW_TO_RED
			: color->g;

	if (!color->b && !color->g && color->r)
		if (color->r < 64)
			color->r--;
		else
			color->r -= color->r > RED_TO_BLACK
				? RED_TO_BLACK
				: color->r;

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
void App_first_frame(App *app, Color *curr_pixel, Color *prev_pixel)
{
	Color color = *prev_pixel;

	if (GetRandomValue(0, 99) > 50)
		App_pixel_fade(&color);

	*curr_pixel = color;
}

// =============================================================================
// @@@ + App_next_frame
// =============================================================================
void App_next_frame(App *app, Color *curr_pixel, Color *prev_pixel)
{
	int value = GetRandomValue(0, 99);

	if (value > OFFSET_RANDOM_FACTOR)
	{
		int offset = curr_pixel > app->pixels && prev_pixel < app->pixels + app->size - 1
			? GetRandomValue(0, 2) - 1
			: 0;

		*curr_pixel = prev_pixel[offset];

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
	for (int row = WINDOW_HEIGHT - 2; row >= 0; row--)
	{
		Color *curr_pixel = app->pixels + row * WINDOW_WIDTH;
		Color *prev_pixel = curr_pixel + WINDOW_WIDTH;

		for (int col = 0; col < WINDOW_WIDTH; col++)
		{
			if (!App_is_color_black(prev_pixel))
				frame_callback(app, curr_pixel, prev_pixel);

			/*if (row & 1)
				frame_callback(app, curr_pixel, prev_pixel);
			else 
				*curr_pixel = *prev_pixel;*/

			curr_pixel++;
			prev_pixel++;
		}
	}
}

// =============================================================================
// @@@ + App_listen
// =============================================================================
/*int App_listen(App *app)
{
	if (IsKeyDown(KEY_Q))
		return 0;
}*/

int main()
{
	SetTraceLogLevel(LOG_NONE);

	// SetConfigFlags(FLAG_FULLSCREEN_MODE);
	SetTargetFPS(TARGET_FPS);

	int w = GetRenderWidth();
	int h = GetRenderHeight();

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Flame");

	Image image = GenImageColor(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		color_black
	);

	Texture2D texture = LoadTextureFromImage(image);

	App app = {
		// .width  = w,
		// .heigh  = h,
		.size   = WINDOW_WIDTH * WINDOW_HEIGHT,
		.pixels = LoadImageColors(image),
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

	return 0;
}