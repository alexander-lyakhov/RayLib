#include "raylib.h"

#define TARGET_FPS 60
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

int main()
{
	SetTraceLogLevel(LOG_NONE);
	
	SetTargetFPS(TARGET_FPS);

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RayLib");

	while(!WindowShouldClose())
	{
		ClearBackground((Color) {0, 0, 0});
	
		BeginDrawing();
		DrawLine(0, 0, WINDOW_WIDTH - 1, 0, (Color){255, 255, 255, 255});

		for (int row = 0; row < WINDOW_HEIGHT; row++)
		{
			for (int col = 0; col < WINDOW_WIDTH; col++)
			{
			}
		}
		EndDrawing();
	}

	return 0;
}