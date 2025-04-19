#include "bullet.h"

const int BULLET_SPEED = 3;
const Uint32 BULLET_LIFETIME = 3000;
const int BULLET_SIZE = 4;

void renderBullet(){
    for (const Bullet& b : bullets) {
        SDL_Rect bulletRect = { (int)b.x - BULLET_SIZE / 2, (int)b.y - BULLET_SIZE / 2, BULLET_SIZE, BULLET_SIZE };
        SDL_RenderCopy(renderer, bulletTexture, NULL, &bulletRect);
    }
}

void shootBullet(const Tank& tank){ ///tạo mảng đạn đợi
    if (tank.exploded) return;
    int standardization;
    standardization = - (tank.tankAngle - 90);
    if (standardization < 0) standardization = 360 + standardization;
    double radian =  standardization * M_PI / 180.0;
    double bulletX = tank.x + cos(radian) * (TANK_SIZE /2 + 9);
    double bulletY = tank.y - sin(radian) * (TANK_SIZE /2 + 9);
    Bullet newBullet = {bulletX, bulletY, tank.tankAngle, SDL_GetTicks()};
    bullets.push_back(newBullet);
}
bool bulletHitTheWall(double x, double y) {
    for (Wall& wall : walls) {
        SDL_Rect bulletRect = {(int) x - BULLET_SIZE / 2, (int) y - BULLET_SIZE / 2, BULLET_SIZE, BULLET_SIZE};
        SDL_Rect wallRect = {wall.x, wall.y, wall.w, wall.h};
        if(SDL_HasIntersection(&bulletRect, &wallRect)){
            return true;
        }
    }
    return false;
}
void flipBulletH(double &angle) {
    if (angle > 180) angle = -angle + 540;
    else angle = -angle + 180;
}

/// Đảo góc khi va vào tường dọc
void flipBulletV(double &angle) {
    angle = -angle + 360;
}

/// Kiểm tra trục của tường
bool isVerticalWall(double x, double y) {
    SDL_Rect bulletRect = {(int)x - BULLET_SIZE / 2, (int)y - BULLET_SIZE / 2, BULLET_SIZE, BULLET_SIZE};

    for (Wall& wall : walls) {
        SDL_Rect wallRect = {wall.x, wall.y, wall.w, wall.h};
        if (SDL_HasIntersection(&bulletRect, &wallRect)) {
            if (wall.w == 4) return true;
            else return false;
        }
    }
    return false;
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
        /// Kiểm tra va chạm với tường
        if (bulletHitTheWall(bullet.x, bullet.y)) {
            if (isVerticalWall(bullet.x, bullet.y)) {
                flipBulletV(bullet.angle);
            } else flipBulletH(bullet.angle);
        }

        /// Kiểm tra nếu đạn chưa hết thời gian thì giữ lại
        if (currentTime - bullet.spawntime < BULLET_LIFETIME) {
            newBullets.push_back(bullet);
        }
    }
    bullets = newBullets;
}


void checkBulletTankCollision(Tank& tank) {
    if (tank.exploded) return;

    Uint32 now = SDL_GetTicks();

    if(now < tank.invincibleUntil) return;
    SDL_Rect tankRect = {tank.x - TANK_SIZE / 2, tank.y - TANK_SIZE / 2, TANK_SIZE, TANK_SIZE};

    for (auto it = bullets.begin(); it != bullets.end(); ) {
    SDL_Rect bulletRect = {(int)it->x - BULLET_SIZE / 2, (int)it->y - BULLET_SIZE / 2, BULLET_SIZE, BULLET_SIZE};

    if (SDL_HasIntersection(&tankRect, &bulletRect)) {
        tank.lives--;
        it = bullets.erase(it);
        if(tank.lives <= 0){
            tank.exploded = true;
        }
        else{
            tank.invincibleUntil = now + 2000;
        }
        break; // Chỉ xử lý 1 viên đạn trúng
    } else {
        ++it;
    }
}

}
