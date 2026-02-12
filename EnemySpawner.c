#include "EnemySpawner.h"

#include <stdio.h>
#include <math.h>

#include "map.h"
#include "enemy.h"
#include "raylib.h"

#define GRID_WIDTH 60
#define GRID_HEIGHT 60
#define TILE_SIZE 32
#define MAX_OBJECTS 7



void SpawnEnemies(Enemy enemies[MAX_OBJECTS], int map[60][60])
{

    float playerX = 5.5f;
    float playerY = 8.0f;
    float spawnRadius = 15.0f;
    float minDistance = 3.0f;

    for (int i = 0; i < MAX_OBJECTS; i++)
    {
        bool validPosition = false;
        int tileX, tileY;
        int attempts = 0;

        while (!validPosition && attempts < 100)
        {
            float angle = GetRandomValue(0, 360) * (PI / 180.0f);
            float distance = GetRandomValue((int)minDistance, (int)spawnRadius);

            tileX = (int)(playerX + cosf(angle) * distance);
            tileY = (int)(playerY + sinf(angle) * distance);

            if (tileX >= 0 && tileX < GRID_WIDTH && tileY >= 0 && tileY < GRID_HEIGHT)
            {
                if (map[tileY][tileX] == 0)
                    validPosition = true;
            }
            attempts++;
        }

        if (!validPosition) {
            tileX = (int)playerX + 5;
            tileY = (int)playerY + 5;
        }

        float worldX = tileX + 0.5f;
        float worldY = tileY + 0.5f;

        InitEnemy(&enemies[i], worldX, worldY);
        printf("Enemy %d spawned at (%.2f, %.2f)\n", i, worldX, worldY);
    }
}
