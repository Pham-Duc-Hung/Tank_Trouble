#include "buff.h"

const Uint32 BUFF_INTERVAL = 15000;
const int BUFF_SIZE = 15;

vector<HeartBuff> heartBuff;

void renderHeartBuff(SDL_Texture* heartBuffTexture){
    for(HeartBuff & hb: heartBuff){
        if(hb.active){
            SDL_Rect heartBuffRect = {hb.x, hb.y, BUFF_SIZE, BUFF_SIZE};
            SDL_RenderCopy(renderer, heartTexture, NULL, &heartBuffRect);
        }
    }
}

bool HeartPositionIsValid(int x, int y){
    SDL_Rect heartBuffRect = {x, y, BUFF_SIZE, BUFF_SIZE};
    for(Wall &wall: walls){
        SDL_Rect wallRect = {wall.x, wall.y, wall.w, wall.h};
        if(SDL_HasIntersection(&heartBuffRect, &wallRect)) return false;
    }
    return true;
}

void spawnHeartBuff(){
    static Uint32 lastSpawnTime = 0;
    Uint32 currentTime = SDL_GetTicks();

    if (currentTime - lastSpawnTime >= BUFF_INTERVAL){
        HeartBuff newHeart;
        newHeart.x = rand() % (SCREEN_WIDTH - HEART_SIZE);
        newHeart.y = rand() % (SCREEN_HEIGHT - HEART_SIZE);
        newHeart.active = true;
        newHeart.spawntime = currentTime;

        if (HeartPositionIsValid(newHeart.x, newHeart.y)) {
            heartBuff.push_back(newHeart);
            lastSpawnTime = currentTime; // Cập nhật lại mốc thời gian
        }
    }
}

void checkTankCollectBuff(Tank &tank){
    if (tank.exploded) return;

    SDL_Rect tankRect = {tank.x - TANK_SIZE / 2, tank.y - TANK_SIZE / 2, TANK_SIZE, TANK_SIZE};

    for(HeartBuff &hb: heartBuff){
        if(hb.active){
            SDL_Rect heartBuffRect = {hb.x, hb.y, BUFF_SIZE, BUFF_SIZE};
            if (SDL_HasIntersection(&tankRect, &heartBuffRect)) {
                if (tank.lives < 7) tank.lives += 1;
                hb.active = false;
                hb.spawntime = SDL_GetTicks();
            }
        }
    }
}
