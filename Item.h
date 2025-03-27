

#ifndef ITEM_H
#define ITEM_H

#include <SDL.h>
#include <vector>
#include "map.h"      // Tương tác với bản đồ
#include "Character.h" // Để xử lý tương tác giữa nhân vật và vật phẩm

// Định nghĩa các loại vật phẩm
enum ItemType {
    INCREASE_BOMB_RANGE, // Tăng phạm vi bom
    INCREASE_SPEED,      // Tăng tốc độ di chuyển
    EXTRA_LIFE          // Thêm mạng sống
};

// Cấu trúc vật phẩm
struct Item {
    int x, y;          // Vị trí của vật phẩm trên bản đồ
    ItemType type;     // Loại vật phẩm
};

// Khai báo texture cho vật phẩm
extern SDL_Texture* itemTexture;

// Các hàm liên quan đến vật phẩm
bool loadItemTextures(SDL_Renderer* renderer);
void freeItemTextures();

void generateItems(std::vector<Item>& items, int itemCount, const std::vector<std::vector<TileType>>& map);
void renderItems(SDL_Renderer* renderer, const std::vector<Item>& items);
void handleItemCollision(Character& character, std::vector<Item>& items);

#endif
