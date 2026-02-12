#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "EnemySpawner.h"
#include "renderer.h"
#include "map.h"

#define PLAYER_SIZE 16
#define MAX_ENEMIES 7


const char* pixelizer = "pixelizer.fs";

int main(void)
{
    float depthBuffer[SCREEN_WIDTH];

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycaster");
    InitBullets();
    SetTargetFPS(60);

    Player player;
    InitPlayer(&player, 5.5f, 8.0f);

    Enemy enemies[MAX_ENEMIES];

    SpawnEnemies(enemies, map);

    RenderTexture2D target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    Shader shader = LoadShader(0, pixelizer);

    while (!WindowShouldClose())
    {
        // ---------------- UPDATE ----------------

        UpdatePlayer(&player, map);

        // Update all enemies
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            //SpawnEnemies(&enemies[i], map);
            UpdateEnemy(&enemies[i], map);
            EnemyPatrol(&enemies[i], (Vector2){ player.x, player.y }, map);
        }

        // Update bullets
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            UpdateBullets(map, &enemies[i]);
        }

        // Fire bullet
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            FireBullet(player.x, player.y, player.angle);
        }

        // ---------------- RENDER ----------------

        BeginTextureMode(target);
        ClearBackground(BLACK);

        RenderScene(&player, enemies, MAX_ENEMIES, map, depthBuffer);

        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        BeginShaderMode(shader);

        DrawTextureRec(target.texture,
                       (Rectangle){0, 0, SCREEN_WIDTH, -SCREEN_HEIGHT},
                       (Vector2){0, 0}, WHITE);

        EndShaderMode();

        DrawCrosshair();
        DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadRenderTexture(target);
    UnloadShader(shader);
    CloseWindow();
    return 0;
}
