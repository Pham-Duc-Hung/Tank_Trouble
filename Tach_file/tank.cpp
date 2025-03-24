#include "tank.h"

int tankX = 100, tankY = 100;
const int TANK_SIZE = 30;
const int TANK_SPEED = 2;
double tankAngle = 0.0;


vector<Wall> walls;
vector<Bullet> bullets;
/// vẽ tank
void renderTank(){
    SDL_Rect tankRect = {tankX - TANK_SIZE / 2, tankY - TANK_SIZE / 2, TANK_SIZE, TANK_SIZE};
    SDL_Point center = {TANK_SIZE / 2, TANK_SIZE / 2};
    SDL_RenderCopyEx(renderer, tankTexture, NULL, &tankRect, tankAngle, &center, SDL_FLIP_NONE);///vẽ hình có thể xoay
}
void removeWallsAroundTank(int tankX, int tankY, int tankSize) { ///Xóa tường quanh điểm khởi đầu của tank khỏi trùng
    vector<Wall> newWalls;

    for (const auto& wall : walls) {
        SDL_Rect tankRect = {tankX - tankSize / 2, tankY - tankSize / 2, tankSize, tankSize};
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
