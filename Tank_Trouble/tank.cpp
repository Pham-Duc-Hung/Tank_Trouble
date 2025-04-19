#include "tank.h"

const int TANK_SIZE = 30;
const int TANK_SPEED = 2;
const int HEART_SIZE = 30;
const int HEART_DISTANCE = 10;
const int HEART_MARGIN = 20;


vector<Wall> walls;
vector<Bullet> bullets;
/// vẽ tank
void renderTank(const Tank& tank, SDL_Texture* texture) {
    SDL_Rect tankRect = {tank.x - TANK_SIZE / 2, tank.y - TANK_SIZE / 2, TANK_SIZE, TANK_SIZE};

    Uint32 now = SDL_GetTicks();

    if(now < tank.invincibleUntil){
        if((now / 200) % 2 == 0) return;
    }
    if (tank.exploded) {
        SDL_RenderCopy(renderer, explosionTexture, NULL, &tankRect); // hiện ảnh nổ
    } else {
        SDL_Point center = {TANK_SIZE / 2, TANK_SIZE / 2};
        SDL_RenderCopyEx(renderer, texture, NULL, &tankRect, tank.tankAngle, &center, SDL_FLIP_NONE);
    }
}

void renderLives(const Tank& tank, bool isPlayer1){
    for(int i = 0; i < tank.lives; i++){
        SDL_Rect heartRect;

        if(isPlayer1){
            heartRect.x = HEART_DISTANCE + i * (HEART_SIZE + HEART_DISTANCE);
        }
        else heartRect.x = 1300 - (i + 1) * (HEART_SIZE + HEART_DISTANCE);

        heartRect.y = 700 - HEART_SIZE - HEART_MARGIN;
        heartRect.w = HEART_SIZE;
        heartRect.h = HEART_SIZE;

        SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect);
    }
}

void removeWallsAroundTank(const Tank& tank) {
    vector<Wall> newWalls;

    SDL_Rect tankRect = {tank.x - TANK_SIZE / 2, tank.y - TANK_SIZE / 2, TANK_SIZE, TANK_SIZE};
    for (const auto& wall : walls) {
        SDL_Rect wallRect = {wall.x, wall.y, wall.w, wall.h};
        if (!SDL_HasIntersection(&tankRect, &wallRect)) {
            newWalls.push_back(wall);
        }
    }

    walls = newWalls;
}

bool notCollision(int newX, int newY){ ///không thể di chuyển xuyên tường
    SDL_Rect newTankRect = {newX - TANK_SIZE/2, newY - TANK_SIZE/2, TANK_SIZE, TANK_SIZE};
    bool canMove = true;
    for(const auto& wall: walls){
        SDL_Rect wallRect = {wall.x, wall.y, wall.w, wall.h};
        if(SDL_HasIntersection(&newTankRect, &wallRect)){
            canMove = false;
            break;
        }
    }
    return canMove;
}

void handleTankMovement(Tank& tank, bool up, bool down, bool left, bool right) {
    if (tank.exploded) return;
    int newX = tank.x;
    int newY = tank.y;

    if (up) newY -= TANK_SPEED;
    if (down) newY += TANK_SPEED;
    if (left) newX -= TANK_SPEED;
    if (right) newX += TANK_SPEED;

    if (notCollision(newX, newY)) {
        tank.x = newX;
        tank.y = newY;
    }
}

void updateTankAngle(Tank& tank, bool up, bool down, bool left, bool right) {
    if (up && !down) {
        if (left && !right) tank.tankAngle = 315;
        else if (right && !left) tank.tankAngle = 45;
        else tank.tankAngle = 0;
    }
    else if (down && !up) {
        if (left && !right) tank.tankAngle = 225;
        else if (right && !left) tank.tankAngle = 135;
        else tank.tankAngle = 180;
    }
    else if (left && !right) tank.tankAngle = 270;
    else if (right && !left) tank.tankAngle = 90;
}


