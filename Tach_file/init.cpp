#include "init.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* backgroundTexture = NULL;
SDL_Texture* wallHorizontalTexture = NULL;
SDL_Texture* wallVerticalTexture = NULL;
SDL_Texture* tankTexture = NULL;
SDL_Texture* bulletTexture = NULL;

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
