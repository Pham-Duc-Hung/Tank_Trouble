#include <iostream>
#include <vector>
#include <ctime>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* backgroundTexture = NULL;
SDL_Texture* wallHorizontalTexture = NULL;
SDL_Texture* wallVerticalTexture = NULL;

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 700;
const string WINDOW_TITLE = "Tank Trouble SDL";

SDL_Texture* tankTexture = NULL;
int tankX = 100, tankY = 100;
const int TANK_SIZE = 30;
const int TANK_SPEED = 3;

struct Wall {
    int x, y, w, h;
};

vector<Wall> walls; // Lưu danh sách tường

void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture* loadTexture(const char* path);
void loadMedia();
void generateMaze(); // Hàm tạo mê cung ngẫu nhiên
void renderWalls();
void removeWallsAroundTank(int tankX, int tankY, int tankSize);
void renderTank();

int main(int argc, char* argv[]) {
    srand(time(0));
    initSDL(window, renderer);
    loadMedia();
    generateMaze();
    removeWallsAroundTank(tankX, tankY, TANK_SIZE);
    bool quit = false;
    SDL_Event e;

    bool keyW=false, keyA=false, keyS=false, keyD=false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            else if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP){
                bool isPressed = (e.type == SDL_KEYDOWN);
                switch (e.key.keysym.sym){
                    case SDLK_w: keyW = isPressed; break;
                    case SDLK_a: keyA = isPressed; break;
                    case SDLK_s: keyS = isPressed; break;
                    case SDLK_d: keyD = isPressed; break;
                }
            }
        }
        //cập nhật vị trí mới
        int newX = tankX, newY=tankY;
        if(keyW) newY-=TANK_SPEED;
        if(keyA) newX-=TANK_SPEED;
        if(keyS) newY+=TANK_SPEED;
        if(keyD) newX+=TANK_SPEED;

        //Check va chạm
        SDL_Rect newTankRect = {newX - TANK_SIZE/2, newY - TANK_SIZE/2, TANK_SIZE, TANK_SIZE};
        bool canMove = true;
        for(const auto& wall: walls){
            SDL_Rect wallRect = {wall.x, wall.y, wall.w, wall.h};
            if(SDL_HasIntersection(&newTankRect, &wallRect)){
                canMove = false;
                break;
            }
        }
        if(canMove){
            tankX=newX;
            tankY=newY;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        renderWalls();
        renderTank();
        SDL_RenderPresent(renderer);
    }

    quitSDL(window, renderer);
    return 0;
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError(cout, "SDL_Init", true);
    }
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logSDLError(cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logSDLError(cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void logSDLError(std::ostream& os, const std::string &msg, bool fatal) {
    os << msg << " Error: " << SDL_GetError() << endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(wallHorizontalTexture);
    SDL_DestroyTexture(wallVerticalTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* loadTexture(const char* path) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface) {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    } else {
        cout << "Không thể load ảnh: " << path << " - " << IMG_GetError() << endl;
    }
    return newTexture;
}

void loadMedia() {
    backgroundTexture = loadTexture("D:/Study/Game Tank/lib/img/background.jpg");
    wallHorizontalTexture = loadTexture("D:/Study/Game Tank/lib/img/hWall.png");
    wallVerticalTexture = loadTexture("D:/Study/Game Tank/lib/img/vWall.png");
    tankTexture = loadTexture("D:/Study/Game Tank/lib/img/tank1.png");

    if (!backgroundTexture || !wallHorizontalTexture || !wallVerticalTexture || !tankTexture) {
        cout << "Lỗi load ảnh!" << endl;
    }
}



//  Hàm vẽ tường từ danh sách walls
void renderWalls() {
    for (const auto& wall : walls) {
        SDL_Rect wallRect = {wall.x, wall.y, wall.w, wall.h};
        SDL_Texture* texture = (wall.w > wall.h) ? wallHorizontalTexture : wallVerticalTexture;
        SDL_RenderCopy(renderer, texture, NULL, &wallRect);
    }
}

void renderTank(){
    SDL_Rect tankRect = {tankX - TANK_SIZE / 2, tankY - TANK_SIZE / 2, TANK_SIZE, TANK_SIZE};
    SDL_RenderCopy(renderer, tankTexture, NULL, &tankRect);
}

void generateMaze() {
    const int CELL_SIZE = 100; // Chiều dài tường
    const int THICKNESS = 10;  // Độ dày của tường

    //  **Tạo tường bao viền**
    walls.push_back({0, 0, SCREEN_WIDTH, THICKNESS});                      // Tường trên
    walls.push_back({0, SCREEN_HEIGHT - THICKNESS, SCREEN_WIDTH, THICKNESS}); // Tường dưới
    walls.push_back({0, 0, THICKNESS, SCREEN_HEIGHT});                     // Tường trái
    walls.push_back({SCREEN_WIDTH - THICKNESS, 0, THICKNESS, SCREEN_HEIGHT}); // Tường phải

    //  **Tạo tường bên trong theo dạng mê cung (GIẢM MẬT ĐỘ)**
    for (int y = THICKNESS; y < SCREEN_HEIGHT - THICKNESS; y += CELL_SIZE) {
        for (int x = THICKNESS; x < SCREEN_WIDTH - THICKNESS; x += CELL_SIZE) {
            int r = rand() % 4; // Tăng số lượng giá trị random để tạo ít tường hơn
            if (r == 0) {
                walls.push_back({x, y, CELL_SIZE, THICKNESS}); // Tạo tường ngang
            } else if (r == 1) {
                walls.push_back({x, y, THICKNESS, CELL_SIZE}); // Tạo tường dọc
            }
            // Nếu r >= 2 => Không tạo tường để có đường đi
        }
    }
}

void removeWallsAroundTank(int tankX, int tankY, int tankSize) {
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
