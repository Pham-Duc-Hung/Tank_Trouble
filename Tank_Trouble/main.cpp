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
    Tank player1 = {100, 100, 0.0};
    Tank player2 = {1100, 600, 180.0};
    removeWallsAroundTank(player1);
    removeWallsAroundTank(player2);
    bool quit = false;
    SDL_Event e;

    bool keyW = false, keyA = false, keyS = false, keyD = false;
    bool keyUp = false, keyDown = false, keyLeft = false, keyRight = false;
    bool keyEnter = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) { // Lấy sự kiện từ bàn phím
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                bool isPressed = (e.type == SDL_KEYDOWN);
                switch (e.key.keysym.sym) {
                    case SDLK_w: keyW = isPressed; break;
                    case SDLK_a: keyA = isPressed; break;
                    case SDLK_s: keyS = isPressed; break;
                    case SDLK_d: keyD = isPressed; break;
                    case SDLK_UP: keyUp = isPressed; break;
                    case SDLK_DOWN: keyDown = isPressed; break;
                    case SDLK_LEFT: keyLeft = isPressed; break;
                    case SDLK_RIGHT: keyRight = isPressed; break;
                    case SDLK_SPACE: // Bắn cho player1
                        if (isPressed) shootBullet(player1);
                        break;
                    case SDLK_RETURN: // Bắn cho player2
                        if (isPressed) shootBullet(player2);
                        break;
                }
            }
        }
        handleTankMovement(player1, keyW, keyS, keyA, keyD);
        updateTankAngle(player1, keyW, keyS, keyA, keyD);

        handleTankMovement(player2, keyUp, keyDown, keyLeft, keyRight);
        updateTankAngle(player2, keyUp, keyDown, keyLeft, keyRight);
        updateBullets();

        // Vẽ lại mọi thứ
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        renderWalls();
        renderTank(player1); // Vẽ Tank 1
        renderTank(player2); // Vẽ Tank 2
        renderBullet();
        SDL_RenderPresent(renderer);
    }

    quitSDL(window, renderer);
    return 0;
}
