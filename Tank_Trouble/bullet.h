#ifndef BULLET_H
#define BULLET_H

#include "common.h"

void renderBullet();
void shootBullet(const Tank& tank);
void updateBullets();
void flipBulletH(double &angle);
void flipBulletV(double &angle);
bool isVerticalWall(double x, double y);
bool bulletHitTheWall(double x, double y);
#endif // BULLET_H
