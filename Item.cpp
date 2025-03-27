
#include "Item.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "constants.h"
#include<SDL_image.h>
// Texture cho từng loại vật phẩm
SDL_Texture* bombItemTexture = nullptr;
SDL_Texture* speedTexture = nullptr;
SDL_Texture* lifeTexture = nullptr;

bool loadItemTextures(SDL_Renderer* renderer) {
    SDL_Surface* bombSurface = IMG_Load(BOMB_ItemTEXTURE_PATH);
    if (!bombSurface) {
        std::cerr << "Không thể tải ảnh vật phẩm bom: " << IMG_GetError() << std::endl;
        return false;
    }
    bombItemTexture = SDL_CreateTextureFromSurface(renderer, bombSurface);
    SDL_FreeSurface(bombSurface);

    SDL_Surface* speedSurface = IMG_Load(SPEED_TEXTURE_PATH);
    if (!speedSurface) {
        std::cerr << "Không thể tải ảnh vật phẩm tốc độ: " << IMG_GetError() << std::endl;
        return false;
    }
    speedTexture = SDL_CreateTextureFromSurface(renderer, speedSurface);
    SDL_FreeSurface(speedSurface);

    SDL_Surface* lifeSurface = IMG_Load(LIFE_ItemTEXTURE_PATH);
    if (!lifeSurface) {
        std::cerr << "Không thể tải ảnh vật phẩm mạng sống: " << IMG_GetError() << std::endl;
        return false;
    }
    lifeTexture = SDL_CreateTextureFromSurface(renderer, lifeSurface);
    SDL_FreeSurface(lifeSurface);

    return bombItemTexture && speedTexture && lifeTexture; // Đảm bảo tất cả texture được tải thành công
}



void freeItemTextures() {
    if (bombItemTexture) {
        SDL_DestroyTexture(bombItemTexture);
        bombItemTexture = nullptr;
    }
    if (speedTexture) {
        SDL_DestroyTexture(speedTexture);
        speedTexture = nullptr;
    }
    if (lifeTexture) {
        SDL_DestroyTexture(lifeTexture);
        lifeTexture = nullptr;
    }
}


// Sinh ngẫu nhiên các vật phẩm trên bản đồ
void generateItems(std::vector<Item>& items, int itemCount, const std::vector<std::vector<TileType>>& map) {
    srand(static_cast<unsigned>(time(0))); // Tạo seed cho số ngẫu nhiên

    // Đặt số lượng vật phẩm mong muốn
    int placedItems = 0;
    while (placedItems < itemCount) {
        int x = rand() % mapCols; // Tọa độ cột ngẫu nhiên
        int y = rand() % mapRows; // Tọa độ hàng ngẫu nhiên

        // Đảm bảo vật phẩm chỉ được đặt trên ô trống (EMPTY)
        if (map[y][x] == EMPTY) {
            // Chọn loại vật phẩm ngẫu nhiên
            ItemType type = static_cast<ItemType>(rand() % 3); // INCREASE_BOMB_RANGE, INCREASE_SPEED, EXTRA_LIFE

            // Thêm vật phẩm vào danh sách
            items.push_back({x, y, type});
            placedItems++; // Tăng số lượng vật phẩm đã được đặt
        }
    }
}

// Hiển thị các vật phẩm trên màn hình
void renderItems(SDL_Renderer* renderer, const std::vector<Item>& items) {
    for (const auto& item : items) {
        SDL_Rect rect = {item.x * TILE_SIZE, item.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};

        // Hiển thị đúng texture dựa trên loại vật phẩm
        switch (item.type) {
            case INCREASE_BOMB_RANGE:
                if (bombItemTexture) {
                    SDL_RenderCopy(renderer, bombItemTexture, nullptr, &rect);
                }
                break;
            case INCREASE_SPEED:
                if (speedTexture) {
                    SDL_RenderCopy(renderer, speedTexture, nullptr, &rect);
                }
                break;
            case EXTRA_LIFE:
                if (lifeTexture) {
                    SDL_RenderCopy(renderer, lifeTexture, nullptr, &rect);
                }
                break;
        }
    }
}


void handleItemCollision(Character& character, std::vector<Item>& items) {
    Uint32 currentTime = SDL_GetTicks(); // Lấy thời gian hiện tại

    for (auto it = items.begin(); it != items.end();) {
        // Nếu nhân vật ở vị trí vật phẩm
        if (character.x == it->x && character.y == it->y) {
            // Áp dụng hiệu ứng từ vật phẩm
            switch (it->type) {
                case INCREASE_BOMB_RANGE:
                    character.bombRange++; // Tăng phạm vi bom
                    std::cout << "Đã nhận vật phẩm: Tăng phạm vi bom. Phạm vi hiện tại: " << character.bombRange << std::endl;
                    break;

                case EXTRA_LIFE:
                    character.lives++; // Tăng mạng
                    std::cout << "Đã nhận vật phẩm: Tăng mạng. Số mạng hiện tại: " << character.lives << std::endl;
                    break;

                case INCREASE_SPEED:
                    character.speed = 2.0f; // Cho phép di chuyển 2 ô mỗi lần
                    character.invincibleStartTime = currentTime; // Lưu thời điểm bắt đầu hiệu ứng
                    std::cout << "Đã nhận vật phẩm: Di chuyển nhanh trong 5 giây!" << std::endl;
                    break;
            }
            // Xóa vật phẩm khỏi danh sách sau khi nhận
            it = items.erase(it);
        } else {
            ++it; // Di chuyển đến vật phẩm tiếp theo nếu không có va chạm
        }
    }

    // Kiểm tra thời gian để dừng hiệu ứng "Di chuyển nhanh"
    if (character.speed == 2.0f && (SDL_GetTicks() - character.invincibleStartTime > 5000)) {
        character.speed = 1.0f; // Quay lại tốc độ di chuyển bình thường
        std::cout << "Hiệu ứng di chuyển nhanh đã hết." << std::endl;
    }
}
