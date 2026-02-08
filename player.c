#include "player.h"
#include "raymath.h"
#include <math.h>

#define GRID_WIDTH 60
#define GRID_HEIGHT 60

void InitPlayer(Player *player, float x, float y) {
    player->x = x;
    player->y = y;
    player->angle = 0.0f;
    player->speed = 0.1f;
    player->health = 100.0f;
    player->maxHealth = 100.0f;
}

bool WouldCollideWithWall(float newX, float newY, int map[60][60]) {
    int gridX1 = (int)(newX - 0.2f);
    int gridY1 = (int)(newY - 0.2f);
    int gridX2 = (int)(newX + 0.2f);
    int gridY2 = (int)(newY + 0.2f);

    if (gridX1 < 0 || gridX2 >= GRID_WIDTH || gridY1 < 0 || gridY2 >= GRID_HEIGHT) return true;
    if (map[gridY1][gridX1] == 1) return true;
    if (map[gridY1][gridX2] == 1) return true;
    if (map[gridY2][gridX1] == 1) return true;
    if (map[gridY2][gridX2] == 1) return true;

    return false;
}

void UpdatePlayer(Player *player, int map[60][60]) {
    // Rotation
    if (IsKeyDown(KEY_LEFT)) player->angle -= 0.05f;
    if (IsKeyDown(KEY_RIGHT)) player->angle += 0.05f;

    Vector2 mouseDelta = GetMouseDelta();
    float sensitivity = 0.05f;
    mouseDelta = Vector2Scale(mouseDelta, sensitivity);
    DisableCursor();
    player->angle += mouseDelta.x * GetFrameTime();

    // Movement
    float newX = player->x;
    float newY = player->y;

    if (IsKeyDown(KEY_W)) {
        newX += cosf(player->angle) * player->speed;
        newY += sinf(player->angle) * player->speed;
    }
    if (IsKeyDown(KEY_S)) {
        newX -= cosf(player->angle) * player->speed;
        newY -= sinf(player->angle) * player->speed;
    }
    if (IsKeyDown(KEY_A)) {
        newX += sinf(player->angle) * player->speed;
        newY -= cosf(player->angle) * player->speed;
    }
    if (IsKeyDown(KEY_D)) {
        newX -= sinf(player->angle) * player->speed;
        newY += cosf(player->angle) * player->speed;
    }

    if (!WouldCollideWithWall(newX, newY, map)) {
        player->x = newX;
        player->y = newY;
    }
}
