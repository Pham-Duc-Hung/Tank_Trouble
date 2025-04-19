#ifndef BUFF_H
#define BUFF_H

#include "common.h"

void renderHeartBuff(SDL_Texture* heartBuffTexture);
bool HeartPositionIsValid(int x, int y);
void spawnHeartBuff();
void checkTankCollectBuff(Tank &tank);

#endif // BUFF_H
