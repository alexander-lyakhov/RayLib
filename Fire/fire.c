#include "raylib.h"
// #include <windows.h>

#define TARGET_FPS 60
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define INIT_RAND srand(time(NULL))
#define RAND(range) (rand() % range)

int main()
{
	SetTraceLogLevel(LOG_NONE);
	
	SetTargetFPS(TARGET_FPS);

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RayLib");

	Color color_black = {0, 0, 0, 255};
	Color color_teal  = {0, 128, 128, 255};
	Color color_green = {0, 255, 0, 255};
	Color color_red   = {255, 0, 0, 255};
	Color color_white = {255, 255, 255, 255};

	Image image = GenImageColor(WINDOW_WIDTH, WINDOW_HEIGHT, color_black);

	Texture2D texture = LoadTextureFromImage(image);

	Color *pixels = LoadImageColors(image);
	Color *p = pixels;

	for (int i = 0; i < WINDOW_WIDTH; i++)
		*p++ = color_white;

	while(!WindowShouldClose())
	{
		ClearBackground((Color) {0, 0, 0});
	
		BeginDrawing();
		// DrawLine(0, 0, WINDOW_WIDTH - 1, 0, (Color){255, 255, 255, 255});

		for (int row = 1; row < WINDOW_HEIGHT; row++)
		{
			Color *line = pixels + row * WINDOW_WIDTH;

			for (int col = 0; col < WINDOW_WIDTH; col++)
			{
				Color *curr_pixel = line + col;
				Color *prev_pixel = curr_pixel - WINDOW_WIDTH;

				int res = GetRandomValue(0, 99) > 50;

				if (res && prev_pixel->b)
					prev_pixel->b--;

				if (res && !prev_pixel->b && prev_pixel->g)
					prev_pixel->g--;

				if (res && !prev_pixel->b && !prev_pixel->g && prev_pixel->r)
					prev_pixel->r--;

				*curr_pixel = *prev_pixel;
			}
		}

		UpdateTexture(texture, pixels);

		DrawTexture(texture, 0, 0, color_white);

		EndDrawing();
	}

	UnloadTexture(texture);
	UnloadImage(image);
	UnloadImageColors(pixels);

	return 0;
}