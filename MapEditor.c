#include "raylib.h"
#include <stdio.h>

#define GRID_WIDTH 60
#define GRID_HEIGHT 60
#define TILE_SIZE 64

int main(void) {
    InitWindow(2560, 1080, "Map Editor");
    SetTargetFPS(60);
    
    int map[GRID_HEIGHT][GRID_WIDTH] = {0};
    
    while (!WindowShouldClose()) {
        // Toggle tiles with mouse
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            int gridX = GetMouseX() / TILE_SIZE;
            int gridY = GetMouseY() / TILE_SIZE;
            if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT) {
                map[gridY][gridX] = 1;
            }
        }
        
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            int gridX = GetMouseX() / TILE_SIZE;
            int gridY = GetMouseY() / TILE_SIZE;
            if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT) {
                map[gridY][gridX] = 0;
            }
        }
        
        // Print map to console
        if (IsKeyPressed(KEY_P)) {
            printf("\nint map[%d][%d] = {\n", GRID_HEIGHT, GRID_WIDTH);
            for (int y = 0; y < GRID_HEIGHT; y++) {
                printf("    {");
                for (int x = 0; x < GRID_WIDTH; x++) {
                    printf("%d", map[y][x]);
                    if (x < GRID_WIDTH - 1) printf(", ");
                }
                printf("}");
                if (y < GRID_HEIGHT - 1) printf(",");
                printf("\n");
            }
            printf("};\n");
        }
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        // Draw grid
        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                Color tileColor = map[y][x] ? GRAY : DARKGRAY;
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, tileColor);
                DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, LIGHTGRAY);
            }
        }
        
        DrawText("Left Click: Add Wall | Right Click: Erase | P: Print to console", 10, 10, 10, WHITE);
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}