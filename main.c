#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "renderer.h"
#include "map.h"

#define PLAYER_SIZE 16

const char* pixelizer = "pixelizer.fs";

int main(void) {
    float depthBuffer[SCREEN_WIDTH];

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycaster");
    InitBullets();
    SetTargetFPS(60);

    Player player;
    InitPlayer(&player, 5.5f, 8.0f);

    Enemy enemy;
    InitEnemy(&enemy, 10.5f, 8.5f);

    RenderTexture2D target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    Shader shader = LoadShader(0, pixelizer);
    if (!IsShaderValid(shader)) {
        TraceLog(LOG_ERROR, "shader failed to load");
    }

    while (!WindowShouldClose()) {
        // Update player
        UpdatePlayer(&player, map);

        // Update bullets
        UpdateBullets(map, &enemy);

        // Update enemy
        UpdateEnemy(&enemy);

        // Fire bullet
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            FireBullet(player.x, player.y, player.angle);

            // Calculate time of impact for prediction
            double t = time_of_impact(
                (Vector2){ player.x, player.y },
                (Vector2){ cosf(player.angle) * BULLET_SPEED, sinf(player.angle) * BULLET_SPEED },
                enemy.position,
                enemy.velocity,
                BULLET_SPEED
            );

            if (t >= 0) TraceLog(LOG_INFO, "Bullet will hit in %.2f seconds.", t);
            else TraceLog(LOG_INFO, "no hit");
        }

        // Draw everything to render texture
        BeginTextureMode(target);
        ClearBackground(BLACK);

        // Render 3D scene
        RenderScene(&player, &enemy, map, depthBuffer);

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
