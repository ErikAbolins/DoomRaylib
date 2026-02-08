#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_BULLETS 500
#define BULLET_SPEED 8.0f
#define BULLET_RADIUS 0.05f
#define BULLET_LIFE 2.0f

typedef struct Bullet {
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
    bool isActive;
    float life;
} Bullet;

extern Bullet bullets[MAX_BULLETS];

void InitBullets(void);
void UpdateBullets(int map[60][60], void *enemy);
void FireBullet(float playerX, float playerY, float playerAngle);
double time_of_impact(Vector2 bulletPosition, Vector2 bulletVelocity, Vector2 targetPos, Vector2 targetVel, double speed);

#endif // BULLET_H
