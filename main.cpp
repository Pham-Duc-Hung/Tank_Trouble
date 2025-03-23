#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* backgroundTexture = NULL;
SDL_Texture* wallHorizontalTexture = NULL;
SDL_Texture* wallVerticalTexture = NULL;
SDL_Texture* tankTexture = NULL;
SDL_Texture* bulletTexture = NULL;

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 700;
const string WINDOW_TITLE = "Tank Trouble SDL";

int tankX = 100, tankY = 100;
const int TANK_SIZE = 30;
const int TANK_SPEED = 2;
const int BULLET_SIZE = 4;
double tankAngle = 0.0;

struct Wall {
    int x, y, w, h;
};

vector<Wall> walls;

struct Bullet {
    double x, y;
    double angle;
    Uint32 spawntime;
};

vector<Bullet> bullets;
const int BULLET_SPEED = 4;
const Uint32 BULLET_LIFETIME = 3000;

void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture* loadTexture(const char* path);
void loadMedia();
void generateMaze();
void renderWalls();
void removeWallsAroundTank(int tankX, int tankY, int tankSize);
void renderTank();
bool notCollision(int newX, int newY);
void renderBullet();
void shootBullet();
void updateBullets();

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

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer) { ///khởi tạo cửa sổ + bút vẽ
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { /// khởi động tất cả module của SDL, nếu khởi tạo được thì ==0
        logSDLError(cout, "SDL_Init", true);
    }
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ///khởi tạo cửa sổ
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); /// tiêu đề, đặt cửa sổ ở chính giữa, kích thước, hiển thị ngay khi tạo
    if (window == nullptr) logSDLError(cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);///khởi tạo bút vẽ
    if (renderer == nullptr) logSDLError(cout, "CreateRenderer", true);///vẽ trên window, mặc định, sd GPU để tăng tốc, tránh xé hình

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); ///Đặt chất lượng nội suy (scaling) khi phóng to/thu nhỏ hình ảnh
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);///thiết lập tọa độ cho bút vẽ
}

void logSDLError(std::ostream& os, const std::string &msg, bool fatal) {/// trả về thông báo lỗi
    os << msg << " Error: " << SDL_GetError() << endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {///xóa tất cả rồi đóng
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(wallHorizontalTexture);
    SDL_DestroyTexture(wallVerticalTexture);
    SDL_DestroyTexture(tankTexture);
    SDL_DestroyTexture(bulletTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* loadTexture(const char* path) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path); /// load ảnh từ file và lưu vào dạng surface
    if (loadedSurface) {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface); ///vẽ từ surface
        SDL_FreeSurface(loadedSurface);///giải phóng surface
    } else {
        cout << "Không thể load ảnh: " << path << " - " << IMG_GetError() << endl;
    }
    return newTexture;
}

void loadMedia() { /// load ảnh
    backgroundTexture = loadTexture("D:/Study/Game Tank/lib/img/background.jpg");
    wallHorizontalTexture = loadTexture("D:/Study/Game Tank/lib/img/hWall.png");
    wallVerticalTexture = loadTexture("D:/Study/Game Tank/lib/img/vWall.png");
    tankTexture = loadTexture("D:/Study/Game Tank/lib/img/tank1.png");
    bulletTexture = loadTexture("D:/Study/Game Tank/lib/img/bullet.png");

    if (!backgroundTexture || !wallHorizontalTexture || !wallVerticalTexture || !tankTexture) {
        cout << "Lỗi load ảnh!" << endl;
    }
}



///  Hàm vẽ tường từ danh sách walls
void renderWalls() {
    for (const auto& wall : walls) {
        SDL_Rect wallRect = {wall.x, wall.y, wall.w, wall.h};
        SDL_Texture* texture = (wall.w > wall.h) ? wallHorizontalTexture : wallVerticalTexture;
        SDL_RenderCopy(renderer, texture, NULL, &wallRect); /// vẽ vào bộ nhớ đệm, NULL -> vẽ toàn bộ
    }
}
/// vẽ tank
void renderTank(){
    SDL_Rect tankRect = {tankX - TANK_SIZE / 2, tankY - TANK_SIZE / 2, TANK_SIZE, TANK_SIZE};
    SDL_Point center = {TANK_SIZE / 2, TANK_SIZE / 2};
    SDL_RenderCopyEx(renderer, tankTexture, NULL, &tankRect, tankAngle, &center, SDL_FLIP_NONE);///vẽ hình có thể xoay
}

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
    double bulletY = tankY + sin(radian) * TANK_SIZE /2;
    Bullet newBullet = {bulletX, bulletY, tankAngle, SDL_GetTicks()};
    bullets.push_back(newBullet);
}

void generateMaze() {
    const int CELL_SIZE = 100; /// Chiều dài tường
    const int THICKNESS = 10;  /// Độ dày của tường

    ///  **Tạo tường bao viền**
    walls.push_back({0, 0, SCREEN_WIDTH, THICKNESS});                      /// Tường trên
    walls.push_back({0, SCREEN_HEIGHT - THICKNESS, SCREEN_WIDTH, THICKNESS}); /// Tường dưới
    walls.push_back({0, 0, THICKNESS, SCREEN_HEIGHT});                     /// Tường trái
    walls.push_back({SCREEN_WIDTH - THICKNESS, 0, THICKNESS, SCREEN_HEIGHT}); /// Tường phải

    ///  **Tạo tường bên trong theo dạng mê cung **
    for (int y = THICKNESS; y < SCREEN_HEIGHT - THICKNESS; y += CELL_SIZE) {
        for (int x = THICKNESS; x < SCREEN_WIDTH - THICKNESS; x += CELL_SIZE) {
            int r = rand() % 4; /// Tăng số lượng giá trị random để tạo ít tường hơn
            if (r == 0) {
                walls.push_back({x, y, CELL_SIZE, THICKNESS}); /// Tạo tường ngang
            } else if (r == 1) {
                walls.push_back({x, y, THICKNESS, CELL_SIZE}); /// Tạo tường dọc
            }
            /// Nếu r >= 2 => Không tạo tường để có đường đi
        }
    }
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
