#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include <stdbool.h>


typedef enum { IDLE, CHASE, ATTACK } EnemyState;


typedef struct Enemy {
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
    float speed;
    bool alive;
    float angle;
    float detectionRange;
    EnemyState state;
} Enemy;

void InitEnemy(Enemy *enemy, float x, float y);
void UpdateEnemy(Enemy *enemy, int map[60][60]);
void EnemyChase(Enemy *enemy, Vector2 playerPos, int map [60][60]);
void EnemyIdle(Enemy *enemy, Vector2 playerPos, int map[60][60]);
bool WouldEnemyCollideWithWall(float newX, float newY, int map[60][60]);


#endif // ENEMY_H
