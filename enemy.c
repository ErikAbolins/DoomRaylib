#include "enemy.h"
#include "raymath.h"
#include <math.h>


#define GRID_WIDTH 60
#define GRID_HEIGHT 60



void InitEnemy(Enemy *enemy, float x, float y) {
    enemy->position = (Vector2){ x, y };
    enemy->size = (Vector2){ 1.0f, 1.0f };
    enemy->velocity = (Vector2){ 0, 0 };
    enemy->alive = true;
    enemy->speed = 1.0f;
    enemy->detectionRange = 10.0f;
    enemy->angle = 0.0f;
}


bool WouldEnemyCollideWithWall(float newX, float newY, int map[60][60]) {
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

void UpdateEnemy(Enemy *enemy, int map[60][60]) {
    if (!enemy->alive) return;
    float dt = GetFrameTime();
    enemy->position.x += enemy->velocity.x * dt;
    enemy->position.y += enemy->velocity.y * dt;

    if (enemy->position.x + enemy->size.x < 0) {
        enemy->alive = false;
    }

}



void EnemyPatrol(Enemy *enemy, Vector2 playerPos, int map[60][60]) {
    float dist = Vector2Distance(enemy->position, playerPos);
    float newX = enemy->position.x;
    float newY = enemy->position.y;

    if (dist < enemy->detectionRange && !WouldEnemyCollideWithWall(newX, newY, map)) {
        //point at player then follow
        float dx = playerPos.x - enemy->position.x;
        float dy = playerPos.y - enemy->position.y;
        enemy->angle = atan2f(dy, dx);
        enemy->position.x += cosf(enemy->angle) * enemy->speed * GetFrameTime();
        enemy->position.y += sinf(enemy->angle) * enemy->speed * GetFrameTime();
    }
}