#include "raylib.h"
#include <stdio.h>

#define TARGET_FPS 60
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define FADE_TO_YELLOW 15
#define FADE_TO_RED 3
#define FADE_TO_BLACK 5

#define OFFSET_FACTOR 2

Color color_black = {0, 0, 0, 255};
Color color_teal  = {0, 128, 128, 255};
Color color_green = {0, 255, 0, 255};
Color color_red   = {255, 0, 0, 255};
Color color_white = {255, 255, 255, 255};

typedef struct {
	Color *pixels;
} App;

// =============================================================================
// @@@ + App_first_frame
// =============================================================================
void App_first_frame(App *app)
{
	Color *p = app->pixels;

	for (int i = 0; i < WINDOW_WIDTH; i++)
		*p++ = color_white;
	
	for (int row = 1; row < WINDOW_HEIGHT; row++)
	{
		Color *line = app->pixels + row * WINDOW_WIDTH;
		Color *curr_pixel = line;
		Color *prev_pixel = curr_pixel - WINDOW_WIDTH;

		for (int col = 0; col < WINDOW_WIDTH; col++)
		{
			if (GetRandomValue(0, 99) > 50)
			{
				if (prev_pixel->b)
					prev_pixel->b -= FADE_TO_YELLOW;

				if (!prev_pixel->b && prev_pixel->g)
					prev_pixel->g -= FADE_TO_RED;

				if (!prev_pixel->b && !prev_pixel->g && prev_pixel->r)
					prev_pixel->r -= FADE_TO_BLACK;
			}

			*curr_pixel++ = *prev_pixel++;
		}
	}
}

// =============================================================================
// @@@ + App_next_frame
// =============================================================================
void App_next_frame(App *app)
{
	// printf("%d\n", GetRandomValue(0, 99));

	for (int row = 1; row < WINDOW_HEIGHT; row++)
	{
		Color *line = app->pixels + row * WINDOW_WIDTH;
		Color *curr_pixel = line;
		Color *prev_pixel = curr_pixel - WINDOW_WIDTH;

		for (int col = 0; col < WINDOW_WIDTH; col++)
		{
		    int value = GetRandomValue(0, 99);

			if (value > 70)
			// if (value > 80)
			{
    			int offset = prev_pixel > (app->pixels + OFFSET_FACTOR) ? GetRandomValue(0, 2) - 1 : 0;
    			
    			*curr_pixel = prev_pixel[offset * OFFSET_FACTOR];

    			prev_pixel++;
    			curr_pixel++;
    			
    			continue;
			}

			if (value > 62)
			// if (value > 50)
			{
				if (prev_pixel->b)
					prev_pixel->b -= FADE_TO_YELLOW;

				if (!prev_pixel->b && prev_pixel->g)
					prev_pixel->g -= FADE_TO_RED;

				if (!prev_pixel->b && !prev_pixel->g && prev_pixel->r)
					prev_pixel->r -= FADE_TO_BLACK;
				
				*curr_pixel++ = *prev_pixel++;

				continue;
			}

			prev_pixel++;
			curr_pixel++;
		}
	}
}

int main()
{
	SetTraceLogLevel(LOG_NONE);

	App app;
	
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