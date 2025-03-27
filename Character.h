
#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>     // Để sử dụng SDL_Texture, Uint32
#include <vector>    // Để quản lý các thao tác trên bản đồ
#include <string>    // Để truyền đường dẫn texture dưới dạng chuỗi
#include "map.h"     // Để xử lý tương tác với bản đồ
#include "constants.h" // Để truy cập hằng số đường dẫn

// Enum xác định hướng di chuyển
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Cấu trúc Character
struct Character {
    int x, y;                   // Vị trí trên bản đồ
    int bombRange;              // Phạm vi bom
    int lives;                  // Số mạng còn lại
    float speed;                // Tốc độ di chuyển
    bool isInvincible;          // Trạng thái miễn nhiễm tạm thời
    Uint32 invincibleStartTime; // Thời điểm bắt đầu trạng thái miễn nhiễm
    Direction direction;        // Hướng di chuyển
    SDL_Texture* texture;       // Texture đại diện cho nhân vật
    SDL_Texture* lifeTexture;   // Texture biểu tượng mạng sống
    int bombRangeVertical = 1;   // Phạm vi nổ theo chiều dọc (mặc định là 1)
    int bombRangeHorizontal = 1; // Phạm vi nổ theo chiều ngang (mặc định là 1)

    bool lastRangeIncreaseDirection = false; // Lần tăng cuối cùng (false: dọc, true: ngang)
};


// Khai báo trạng thái phím toàn cục
extern bool keyPressed; // Theo dõi trạng thái phím nhấn

// Texture cho biểu tượng mạng sống
extern SDL_Texture* lifeTexture; // Texture cho biểu tượng mạng sống

// Khởi tạo nhân vật với các thuộc tính cơ bản và texture
void initializeCharacter(Character& character, int startX, int startY, SDL_Renderer* renderer);

// Tải texture biểu tượng mạng sống
void loadLifeTexture(SDL_Renderer* renderer);

// Hiển thị số mạng còn lại
void renderLives(SDL_Renderer* renderer, const Character& character);

void moveCharacter(Character& character, const std::vector<std::vector<TileType>>& map, SDL_Event& event);

// Hiển thị nhân vật trên màn hình
void renderCharacter(SDL_Renderer* renderer, const Character& character);

// Xử lý trạng thái miễn nhiễm cho nhân vật
void updateInvincibility(Character& character, Uint32 currentTime);


// Dọn dẹp texture nhân vật và mạng sống
void cleanupCharacter(Character& character);

#endif // CHARACTER_H
