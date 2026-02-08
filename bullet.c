#include "bullet.h"
#include "enemy.h"
#include <math.h>

#define GRID_WIDTH 60
#define GRID_HEIGHT 60

Bullet bullets[MAX_BULLETS];

void InitBullets(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].isActive = false;
    }
}

double time_of_impact(Vector2 bulletPosition, Vector2 bulletVelocity, Vector2 targetPos, Vector2 targetVel, double speed) {
    // Relative position and velocity
    Vector2 relativePosition = {targetPos.x - bulletPosition.x, targetPos.y - bulletPosition.y};
    Vector2 relativeVelocity = {targetVel.x - bulletVelocity.x, targetVel.y - bulletVelocity.y};

    // Coefficients
    double a = relativeVelocity.x * relativeVelocity.x + relativeVelocity.y * relativeVelocity.y - speed * speed;
    double b = 2.0 * (relativeVelocity.x * relativePosition.x + relativeVelocity.y * relativePosition.y);
    double c = relativePosition.x * relativePosition.x + relativePosition.y * relativePosition.y;

    double discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0) return -1.0; // No hit

    double sqrt_discriminant = sqrt(discriminant);
    double t1 = (-b - sqrt_discriminant) / (2.0 * a);
    double t2 = (-b + sqrt_discriminant) / (2.0 * a);

    // Smallest non-negative time
    if (t1 >= 0) return t1;
    if (t2 >= 0) return t2;

    return -1.0; // No valid time
}

void UpdateBullets(int map[60][60], void *enemyPtr) {
    Enemy *enemy = (Enemy *)enemyPtr;
    float dt = GetFrameTime();

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].isActive) continue;

        Vector2 newPos = {
            bullets[i].position.x + bullets[i].velocity.x * dt,
            bullets[i].position.y + bullets[i].velocity.y * dt
        };

        // Check hit
        if (enemy->alive) {
            float dx = newPos.x - enemy->position.x;
            float dy = newPos.y - enemy->position.y;
            float dist = sqrtf(dx*dx + dy*dy);

            if (dist < 0.5f) {  // Hit radius
                enemy->alive = false;
                bullets[i].isActive = false;
                TraceLog(LOG_INFO, "ENEMY HIT!");
                continue;
            }
        }

        // Wall collision
        int mapX = (int)newPos.x;
        int mapY = (int)newPos.y;

        if (mapX < 0 || mapX >= GRID_WIDTH ||
            mapY < 0 || mapY >= GRID_HEIGHT ||
            map[mapY][mapX] == 1) {
            bullets[i].isActive = false;
            continue;
        }

        bullets[i].position = newPos;
        bullets[i].life -= dt;

        if (bullets[i].life <= 0.0f) {
            bullets[i].isActive = false;
        }
    }
}

void FireBullet(float playerX, float playerY, float playerAngle) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].isActive) {
            bullets[i].isActive = true;
            bullets[i].position = (Vector2){ playerX, playerY };
            bullets[i].velocity = (Vector2){
                cosf(playerAngle) * BULLET_SPEED,
                sinf(playerAngle) * BULLET_SPEED
            };
            bullets[i].life = BULLET_LIFE;
            break;
        }
    }
}
