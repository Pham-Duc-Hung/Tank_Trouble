#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* backgroundTexture;
extern SDL_Texture* wallHorizontalTexture;
extern SDL_Texture* wallVerticalTexture;
extern SDL_Texture* tank1Texture;
extern SDL_Texture* tank2Texture;
extern SDL_Texture* bulletTexture;
extern SDL_Texture* explosionTexture;
extern SDL_Texture* heartTexture;
extern SDL_Texture* heartBuffTexture;
const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 700;
const string WINDOW_TITLE = "Tank Trouble SDL";

extern const int TANK_SIZE;
extern const int TANK_SPEED;
extern const int BULLET_SIZE;
extern const int CELL_SIZE; /// Chiều dài tường
extern const int THICKNESS;  /// Độ dày của tường
extern const int HEART_SIZE;
extern const int HEART_DISTANCE;
extern const int HEART_MARGIN;

struct Wall {
    int x, y, w, h;
};
struct Bullet {
    double x, y;
    double angle;
    Uint32 spawntime;
};
struct Tank {
    int x, y;
    double tankAngle;
    bool exploded;
    int lives;
    Uint32 invincibleUntil;
};
struct HeartBuff{
    int x, y;
    bool active;
    Uint32 spawntime;
};
extern vector<Wall> walls;
extern vector<Bullet> bullets;
extern vector<HeartBuff> heartBuff;
extern const int BULLET_SPEED;
extern const Uint32 BULLET_LIFETIME;
extern const Uint32 BUFF_INTERVAL;
extern const int BUFF_SIZE;
#endif // COMMON_H
