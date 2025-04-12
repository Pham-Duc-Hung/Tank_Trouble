#include "wall.h"
const int CELL_SIZE = 100; /// Chiều dài tường
const int THICKNESS = 4;  /// Độ dày của tường
void generateMaze() {

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

///  Hàm vẽ tường từ danh sách walls
void renderWalls() {
    for (const auto& wall : walls) {
        SDL_Rect wallRect = {wall.x, wall.y, wall.w, wall.h};
        SDL_Texture* texture = (wall.w > wall.h) ? wallHorizontalTexture : wallVerticalTexture;
        SDL_RenderCopy(renderer, texture, NULL, &wallRect); /// vẽ vào bộ nhớ đệm, NULL -> vẽ toàn bộ
    }
}
