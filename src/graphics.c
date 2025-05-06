#include "../include/graphics.h"

void TestRay(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set the game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Hello, raylib!", 350, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();
}