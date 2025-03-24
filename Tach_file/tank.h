#ifndef TANK_H
#define TANK_H

#include "common.h"

void removeWallsAroundTank(int tankX, int tankY, int tankSize);
void renderTank();
bool notCollision(int newX, int newY);
#endif // TANK_H
