#ifndef TANK_H
#define TANK_H

#include "common.h"

void removeWallsAroundTank(const Tank& tank);
void renderTank(const Tank& tank, SDL_Texture* texture);
void renderLives(const Tank& tank, bool isPlayer1);
bool notCollision(int newX, int newY);
void handleTankMovement(Tank& tank, bool up, bool down, bool left, bool right);
void updateTankAngle(Tank& tank, bool up, bool down, bool left, bool right);

#endif // TANK_H
