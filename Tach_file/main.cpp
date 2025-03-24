#include "common.h"
#include "init.h"
#include "tank.h"
#include "wall.h"
#include "bullet.h"

int main(int argc, char* argv[]) {
    srand(time(0));
    initSDL(window, renderer);
    loadMedia();
    generateMaze();
    removeWallsAroundTank(tankX, tankY, TANK_SIZE);
    bool quit = false;
    SDL_Event e;

    bool keyW = false, keyA = false, keyS = false, keyD = false, keySp = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) { ///lấy sự kiện trong hàng đợi lưu vào e
            if (e.type == SDL_QUIT) quit = true;///thoát game
            else if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP){ ///di chuyển tank từ bàn phím
                bool isPressed = (e.type == SDL_KEYDOWN);
                switch (e.key.keysym.sym){
                    case SDLK_w: keyW = isPressed; break;
                    case SDLK_a: keyA = isPressed; break;
                    case SDLK_s: keyS = isPressed; break;
                    case SDLK_d: keyD = isPressed; break;
                    case SDLK_SPACE:
                        if (isPressed) shootBullet();
                        break;
                }
            }
        }

        int newX = tankX, newY = tankY;

        /// Cập nhật vị trí theo hướng di chuyển
        if (keyW) newY -= TANK_SPEED;
        if (keyA) newX -= TANK_SPEED;
        if (keyS) newY += TANK_SPEED;
        if (keyD) newX += TANK_SPEED;

        /// Kiểm tra va chạm trước khi cập nhật vị trí
        if (notCollision(newX, newY)) {
            tankX = newX;
            tankY = newY;
        }
        /// Xác định góc quay của xe tăng theo 8 hướng
        if (keyW && !keyS) {
            if (keyA && !keyD) tankAngle = 315;
            else if (keyD && !keyA) tankAngle = 45;
            else tankAngle = 0;
        }
        else if (keyS && !keyW) {
            if (keyA && !keyD) tankAngle = 225;
            else if (keyD && !keyA) tankAngle = 135;
            else tankAngle = 180;
        }
        else if (keyA && !keyD) tankAngle = 270;
        else if (keyD && !keyA) tankAngle = 90;

        updateBullets();

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        renderWalls();
        renderTank();
        renderBullet();
        SDL_RenderPresent(renderer);
    }
    quitSDL(window, renderer);
    return 0;
}





