#include "renderer.h"
#include <math.h>

#define GRID_WIDTH 60
#define GRID_HEIGHT 60

void RenderScene(Player *player, Enemy *enemy, int map[60][60], float *depthBuffer) {
    // 3D VIEW
    float fov = PI / 3.0f;
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float rayAngle = (player->angle - fov/2.0f) + ((float)x / SCREEN_WIDTH) * fov;

        float rayDirX = cosf(rayAngle);
        float rayDirY = sinf(rayAngle);

        float rayX = player->x;
        float rayY = player->y;
        float distance = 0.0f;
        bool hit = false;

        while (!hit && distance < 20.0f) {
            distance += 0.05f;
            rayX = player->x + rayDirX * distance;
            rayY = player->y + rayDirY * distance;

            int mapX = (int)rayX;
            int mapY = (int)rayY;

            if (mapX < 0 || mapX >= GRID_WIDTH || mapY < 0 || mapY >= GRID_HEIGHT) {
                hit = true;
            } else if (map[mapY][mapX] == 1) {
                hit = true;
            }
        }

        distance *= cosf(rayAngle - player->angle);
        depthBuffer[x] = distance;

        int lineHeight = (int)(SCREEN_HEIGHT / (distance + 0.0001f));
        int drawStart = (SCREEN_HEIGHT / 2) - (lineHeight / 2);
        int drawEnd = (SCREEN_HEIGHT / 2) + (lineHeight / 2);

        DrawLine(x, 0, x, drawStart, DARKGRAY);
        DrawLine(x, drawStart, x, drawEnd, DARKPURPLE);
        DrawLine(x, drawEnd, x, SCREEN_HEIGHT, GRAY);
    }

    // Draw enemy
    if (enemy->alive) {
        float dx = enemy->position.x - player->x;
        float dy = enemy->position.y - player->y;

        float distance = sqrtf(dx*dx + dy*dy);
        float angleToEnemy = atan2f(dy, dx);
        float relativeAngle = angleToEnemy - player->angle;

        while (relativeAngle > PI) relativeAngle -= 2*PI;
        while (relativeAngle < -PI) relativeAngle += 2*PI;

        if (fabs(relativeAngle) < FOV / 2.0f && distance > 0.1f) {
            int screenx = (int)((0.5f + relativeAngle / FOV) * SCREEN_WIDTH);

            int spriteHeight = (int)(SCREEN_HEIGHT / distance);
            int spriteWidth = spriteHeight;

            int drawStartY = SCREEN_HEIGHT / 2 - spriteHeight / 2;
            int drawEndY = SCREEN_HEIGHT / 2 + spriteHeight / 2;

            int drawStartX = screenx - spriteWidth / 2;
            int drawEndX = screenx + spriteWidth / 2;

            for (int x = drawStartX; x < drawEndX; x++) {
                if (x < 0 || x >= SCREEN_WIDTH) continue;
                if (distance >= depthBuffer[x]) continue;
                DrawLine(x, drawStartY, x, drawEndY, RED);
            }
        }
    }
}

void DrawCrosshair(void) {
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    DrawLine(centerX - 5, centerY, centerX + 5, centerY, WHITE);
    DrawLine(centerX, centerY - 5, centerX, centerY + 5, WHITE);
}
