#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include <stdbool.h>

typedef struct Enemy {
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
    bool alive;
} Enemy;

void InitEnemy(Enemy *enemy, float x, float y);
void UpdateEnemy(Enemy *enemy);

#endif // ENEMY_H
