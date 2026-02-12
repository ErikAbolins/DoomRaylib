#include "renderer.h"
#include <math.h>

#define GRID_WIDTH 60
#define GRID_HEIGHT 60

// Helper struct for sorting enemies by distance
typedef struct {
    int index;
    float distance;
} EnemyDistance;

void RenderScene(Player *player, Enemy *enemies, int enemyCount, int map[60][60], float *depthBuffer) {
    // 3D VIEW - Render walls first
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

    // Calculate distances for all alive enemies
    EnemyDistance enemyDistances[100];
    int aliveCount = 0;

    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i].alive) {
            float dx = enemies[i].position.x - player->x;
            float dy = enemies[i].position.y - player->y;
            float distance = sqrtf(dx*dx + dy*dy);

            enemyDistances[aliveCount].index = i;
            enemyDistances[aliveCount].distance = distance;
            aliveCount++;

            // Debug: Print enemy info on first frame
            static bool printed = false;
            if (!printed && i == 0) {
                TraceLog(LOG_INFO, "Enemy %d: pos(%.2f, %.2f) player(%.2f, %.2f) dist=%.2f alive=%d",
                    i, enemies[i].position.x, enemies[i].position.y,
                    player->x, player->y, distance, enemies[i].alive);
            }
        }
    }

    static bool printedCount = false;
    if (!printedCount) {
        TraceLog(LOG_INFO, "Total alive enemies: %d out of %d", aliveCount, enemyCount);
        printedCount = true;
    }

    // Sort enemies by distance
    for (int i = 0; i < aliveCount - 1; i++) {
        for (int j = 0; j < aliveCount - i - 1; j++) {
            if (enemyDistances[j].distance < enemyDistances[j + 1].distance) {
                EnemyDistance temp = enemyDistances[j];
                enemyDistances[j] = enemyDistances[j + 1];
                enemyDistances[j + 1] = temp;
            }
        }
    }

    // Draw all enemies from farthest to nearest
    for (int i = 0; i < aliveCount; i++) {
        Enemy *enemy = &enemies[enemyDistances[i].index];
        float distance = enemyDistances[i].distance;

        float dx = enemy->position.x - player->x;
        float dy = enemy->position.y - player->y;

        float angleToEnemy = atan2f(dy, dx);
        float relativeAngle = angleToEnemy - player->angle;

        while (relativeAngle > PI) relativeAngle -= 2*PI;
        while (relativeAngle < -PI) relativeAngle += 2*PI;

        static bool printedRender = false;
        if (!printedRender && i == 0) {
            TraceLog(LOG_INFO, "Enemy %d render: relAngle=%.2f FOV=%.2f inFOV=%d dist=%.2f",
                enemyDistances[i].index, relativeAngle, FOV,
                (fabs(relativeAngle) < FOV / 2.0f), distance);
            printedRender = true;
        }

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
