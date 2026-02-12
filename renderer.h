#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include "player.h"
#include "enemy.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FOV (PI / 3.0F)
#define PROJ_PLANE_DIST (SCREEN_WIDTH / (2.0F * tanf(FOV / 2.0F)))

void RenderScene(Player *player, Enemy *enemies, int enemyCount, int map[60][60], float *depthBuffer);
void DrawCrosshair(void);

#endif // RENDERER_H
