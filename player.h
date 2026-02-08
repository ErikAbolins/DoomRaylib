#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct {
    float x, y;
    float angle;
    float speed;
    float health;
    float maxHealth;
} Player;

void InitPlayer(Player *player, float x, float y);
void UpdatePlayer(Player *player, int map[60][60]);
bool WouldCollideWithWall(float newX, float newY, int map[60][60]);

#endif // PLAYER_H
