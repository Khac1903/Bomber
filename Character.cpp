

#include <SDL.h>
#include <SDL_image.h> // Để xử lý ảnh nhân vật
#include "Character.h"
#include "map.h"
#include <iostream>
#include "constants.h" // Bao gồm đường dẫn hằng số

// Biến trạng thái theo dõi phím được nhấn
bool keyPressed = false; // Trạng thái phím nhấn (toàn cục)


// Khởi tạo nhân vật với các thuộc tính cơ bản và texture
void initializeCharacter(Character& character, int startX, int startY, SDL_Renderer* renderer) {
    // Khởi tạo các thuộc tính cơ bản
    character.x = startX;
    character.y = startY;
    character.bombRange = 1;
    character.lives = 3;
    character.speed = 1.0f;
    character.isInvincible = false;
    character.invincibleStartTime = 0;
    character.direction = DOWN;

    // Tải texture nhân vật
    SDL_Surface* surface = IMG_Load(CHARACTER_TEXTURE_PATH); // Sử dụng hằng số từ constants.h
    if (!surface) {
        std::cerr << "Lỗi tải ảnh nhân vật: " << IMG_GetError() << std::endl;
        character.texture = nullptr; // Nếu lỗi, đặt texture về null
        return;
    }
    character.texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!character.texture) {
        std::cerr << "Lỗi tạo texture nhân vật: " << SDL_GetError() << std::endl;
    }

    // Tải texture mạng sống
    SDL_Surface* lifeSurface = IMG_Load(LIFE_TEXTURE_PATH); // Đường dẫn ảnh mạng sống
    if (!lifeSurface) {
        std::cerr << "Lỗi tải ảnh mạng sống: " << IMG_GetError() << std::endl;
        character.lifeTexture = nullptr; // Nếu lỗi, đặt texture mạng sống về null
        return;
    }
    character.lifeTexture = SDL_CreateTextureFromSurface(renderer, lifeSurface);
    SDL_FreeSurface(lifeSurface);

    if (!character.lifeTexture) {
        std::cerr << "Lỗi tạo texture mạng sống: " << SDL_GetError() << std::endl;
    }
}


// Hiển thị số mạng còn lại của nhân vật
void renderLives(SDL_Renderer* renderer, const Character& character) {
    if (!character.lifeTexture) {
        std::cerr << "Texture mạng sống chưa được tải!" << std::endl;
        return;
    }

    for (int i = 0; i < character.lives; i++) {
        SDL_Rect lifeRect = {20 + i * 40, WINDOW_HEIGHT - 40, 30, 30}; // Vị trí và kích thước mạng sống
        SDL_RenderCopy(renderer, character.lifeTexture, nullptr, &lifeRect); // Hiển thị ảnh mạng sống
    }
}


// Xử lý trạng thái miễn nhiễm cho nhân vật (nếu cần)
void updateInvincibility(Character& character, Uint32 currentTime) {
    if (character.isInvincible && currentTime > character.invincibleStartTime + 3000) { // Miễn nhiễm kéo dài 3 giây
        character.isInvincible = false; // Kết thúc trạng thái miễn nhiễm
    }
}

// Xử lý di chuyển của nhân vật
void moveCharacter(Character& character, const std::vector<std::vector<TileType>>& map, SDL_Event& event) {
    int nextX = character.x;
    int nextY = character.y;

    // Chỉ xử lý di chuyển khi nhận sự kiện SDL_KEYUP
    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                nextY -= character.speed; // Di chuyển lên với khoảng cách dựa vào tốc độ
                character.direction = UP;
                break;
            case SDLK_DOWN:
                nextY += character.speed; // Di chuyển xuống
                character.direction = DOWN;
                break;
            case SDLK_LEFT:
                nextX -= character.speed; // Di chuyển trái
                character.direction = LEFT;
                break;
            case SDLK_RIGHT:
                nextX += character.speed; // Di chuyển phải
                character.direction = RIGHT;
                break;
        }

        // Kiểm tra ô tiếp theo hợp lệ (trống hoặc bụi cỏ)
        bool validMovement = true;
        for (int step = 1; step <= character.speed; ++step) {
            int intermediateX = character.x + step * (nextX - character.x) / character.speed;
            int intermediateY = character.y + step * (nextY - character.y) / character.speed;
            if (intermediateX < 0 || intermediateX >= static_cast<int>(map[0].size()) ||
                intermediateY < 0 || intermediateY >= static_cast<int>(map.size()) ||
                (map[intermediateY][intermediateX] != EMPTY && map[intermediateY][intermediateX] != GRASS)) {
                validMovement = false;
                break;
            }
        }

        if (validMovement) {
            character.x = nextX; // Cập nhật vị trí cuối cùng
            character.y = nextY;
        } else {
            std::cerr << "Không thể di chuyển đến vị trí: (" << nextX << ", " << nextY << ")" << std::endl;
        }
    }
}


// Vẽ nhân vật trên bản đồ (sử dụng texture)
void renderCharacter(SDL_Renderer* renderer, const Character& character) {
    if (!character.texture) {
        std::cerr << "Texture nhân vật không được tải!" << std::endl;
        return;
    }

    // Kiểm tra tọa độ nhân vật trước khi vẽ
    if (character.x < 0 || character.y < 0) {
        std::cerr << "Tọa độ nhân vật không hợp lệ: (" << character.x << ", " << character.y << ")" << std::endl;
        return;
    }

    SDL_Rect dstRect = {character.x * TILE_SIZE, character.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopy(renderer, character.texture, nullptr, &dstRect);
}


// Dọn dẹp texture nhân vật
void cleanupCharacter(Character& character) {
    if (character.texture) {
        SDL_DestroyTexture(character.texture);
        character.texture = nullptr;
    }
    if (character.lifeTexture) {
        SDL_DestroyTexture(character.lifeTexture);
        character.lifeTexture = nullptr;
    }
}

