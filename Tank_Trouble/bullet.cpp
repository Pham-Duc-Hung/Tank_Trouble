#include "bullet.h"

const int BULLET_SPEED = 4;
const Uint32 BULLET_LIFETIME = 3000;
const int BULLET_SIZE = 4;

void renderBullet(){
    for (const Bullet& b : bullets) {
        SDL_Rect bulletRect = { (int)b.x - BULLET_SIZE / 2, (int)b.y - BULLET_SIZE / 2, BULLET_SIZE, BULLET_SIZE };
        SDL_RenderCopy(renderer, bulletTexture, NULL, &bulletRect);
    }
}

void shootBullet(){ ///tạo mảng đạn đợi
    int standardization;
    if (tankAngle == 0) standardization = 90;
    else if (tankAngle == 45) standardization = 45;
    else if (tankAngle == 90) standardization = 0;
    else if (tankAngle == 135) standardization = 315;
    else if (tankAngle == 180) standardization = 270;
    else if (tankAngle == 225) standardization = 225;
    else if (tankAngle == 270) standardization = 180;
    else standardization = 135;
    double radian = standardization * M_PI / 180.0;
    double bulletX = tankX + cos(radian) * TANK_SIZE /2;
    double bulletY = tankY - sin(radian) * TANK_SIZE /2;
    Bullet newBullet = {bulletX, bulletY, tankAngle, SDL_GetTicks()};
    bullets.push_back(newBullet);
}







void updateBullets() {
    Uint32 currentTime = SDL_GetTicks();
    vector<Bullet> newBullets; /// Lưu đạn chưa hết thời gian

    for (auto& bullet : bullets) {
        /// Di chuyển đạn theo hướng của nó (8 hướng cố định)
        switch ((int)bullet.angle) {
            case 0:   bullet.y -= BULLET_SPEED; break; /// Lên
            case 45:  bullet.x += BULLET_SPEED; bullet.y -= BULLET_SPEED; break; /// Lên phải
            case 90:  bullet.x += BULLET_SPEED; break; /// Phải
            case 135: bullet.x += BULLET_SPEED; bullet.y += BULLET_SPEED; break; /// Xuống phải
            case 180: bullet.y += BULLET_SPEED; break; /// Xuống
            case 225: bullet.x -= BULLET_SPEED; bullet.y += BULLET_SPEED; break; /// Xuống trái
            case 270: bullet.x -= BULLET_SPEED; break; /// Trái
            case 315: bullet.x -= BULLET_SPEED; bullet.y -= BULLET_SPEED; break; /// Lên trái
        }
    }
}
