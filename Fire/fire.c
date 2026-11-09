#include "raylib.h"
#include <stdio.h>

#define TARGET_FPS      30
#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT  768
/*
#define FADE_TO_YELLOW  25 // 15
#define FADE_TO_RED     20 //  3
#define FADE_TO_BLACK   15 //  5

#define OFFSET_RANDOM_FACTOR 40
#define REPEAT_RANDOM_FACTOR 25
*/

#define FADE_TO_YELLOW  40 // 15
#define FADE_TO_RED     30 //  3
#define FADE_TO_BLACK   20 //  5

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
	size_t size;
} App;

// =============================================================================
// @@@ + App_pixel_fade
// =============================================================================
Color* App_pixel_fade(Color *color)
{
	if (color->b)
		color->b -= color->b > FADE_TO_YELLOW
			? FADE_TO_YELLOW
			: color->b;

	if (!color->b && color->g)
		color->g -= color->g > FADE_TO_RED
			? FADE_TO_RED
			: color->g;

	if (!color->b && !color->g && color->r)
		color->r -= color->r > FADE_TO_BLACK
			? FADE_TO_BLACK
			: color->r;

	return color;
}

// void App_next_frame(App *app);

// =============================================================================
// @@@ + App_first_frame
// =============================================================================
void App_first_frame(App *app)
{
	Color *p = app->pixels + (WINDOW_HEIGHT - 1) * WINDOW_WIDTH;

	for (int i = 0; i < WINDOW_WIDTH; i++)
		*p++ = color_white;
	
	for (int row = WINDOW_HEIGHT - 2; row >= 0; row--)
	{
		Color *curr_pixel = app->pixels + row * WINDOW_WIDTH;
		Color *prev_pixel = curr_pixel + WINDOW_WIDTH;

		for (int col = 0; col < WINDOW_WIDTH; col++)
		{
			Color color = *prev_pixel;

			if (GetRandomValue(0, 99) > 50)
				App_pixel_fade(&color);

			*curr_pixel = color;

			curr_pixel++;
			prev_pixel++;
		}
	}
	// App_next_frame(app);
}

// =============================================================================
// @@@ + App_next_frame
// =============================================================================
void App_next_frame(App *app)
{
	for (int row = WINDOW_HEIGHT - 2; row >= 0; row--)
	{
		Color *curr_pixel = app->pixels + row * WINDOW_WIDTH;
		Color *prev_pixel = curr_pixel + WINDOW_WIDTH;

		for (int col = 0; col < WINDOW_WIDTH; col++)
		{
		    int value = GetRandomValue(0, 99);

			if (value > OFFSET_RANDOM_FACTOR)
			{
    			int offset = curr_pixel > (app->pixels + OFFSET_FACTOR) && prev_pixel < (app->pixels + app->size - OFFSET_FACTOR - 1) ? GetRandomValue(0, 2) - 1 : 0;
    			*curr_pixel = prev_pixel[offset * OFFSET_FACTOR];

    			prev_pixel++;
    			curr_pixel++;
    			
    			continue;
			}

			if (value > REPEAT_RANDOM_FACTOR)
			{
				Color color = *prev_pixel;
				*curr_pixel = *App_pixel_fade(&color);
			}

			prev_pixel++;
			curr_pixel++;
		}
	}
}

int main()
{
	SetTraceLogLevel(LOG_NONE);

	App app = {
		.size = WINDOW_WIDTH * WINDOW_HEIGHT
	};
	
	SetTargetFPS(TARGET_FPS);

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RayLib");
	// SetRandomSeed();

	Image image = GenImageColor(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		color_black
	);

	Texture2D texture = LoadTextureFromImage(image);
	// Color *pixels = LoadImageColors(image);

	app.pixels = LoadImageColors(image);
	App_first_frame(&app);

	while(!WindowShouldClose())
	{
		// ClearBackground((Color) {0, 0, 0});
		
		App_next_frame(&app);
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