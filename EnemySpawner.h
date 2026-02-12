#ifndef ENEMYSPAWNER_H
#define ENEMYSPAWNER_H


#include "raylib.h"
#include "enemy.h"

#define MAX_OBJECTS 7

void SpawnEnemy(Enemy *enemy, int map [60][60]);


void SpawnEnemies(Enemy enemies[MAX_OBJECTS], int map [60][60]);


#endif //ENEMYSPAWNER_H