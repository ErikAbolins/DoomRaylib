#include "enemy.h"

void InitEnemy(Enemy *enemy, float x, float y) {
    enemy->position = (Vector2){ x, y };
    enemy->size = (Vector2){ 1.0f, 1.0f };
    enemy->velocity = (Vector2){ 0, 0 };
    enemy->alive = true;
}

void UpdateEnemy(Enemy *enemy) {
    if (!enemy->alive) return;
    float dt = GetFrameTime();
    enemy->position.x += enemy->velocity.x * dt;
    enemy->position.y += enemy->velocity.y * dt;

    if (enemy->position.x + enemy->size.x < 0) {
        enemy->alive = false;
    }
}
