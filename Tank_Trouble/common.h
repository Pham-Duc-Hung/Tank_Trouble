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
extern SDL_Texture* tankTexture;
extern SDL_Texture* bulletTexture;

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 700;
const string WINDOW_TITLE = "Tank Trouble SDL";

extern const int TANK_SIZE;
extern const int TANK_SPEED;
extern const int BULLET_SIZE;
extern const int CELL_SIZE; /// Chiều dài tường
extern const int THICKNESS;  /// Độ dày của tường

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

};
extern vector<Wall> walls;
extern vector<Bullet> bullets;
extern const int BULLET_SPEED;
extern const Uint32 BULLET_LIFETIME;
#endif // COMMON_H
