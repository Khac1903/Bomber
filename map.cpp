
#include "map.h"
#include <SDL_image.h>
#include <queue>
#include "constants.h"
#include<iostream>
// Biến toàn cục để lưu texture
SDL_Texture* wallTexture = nullptr;
SDL_Texture* grassTexture = nullptr;
SDL_Texture* emptyTexture = nullptr;

// Hàm tải texture
bool loadTextures(SDL_Renderer* renderer) {
    wallTexture = IMG_LoadTexture(renderer, WALL_TEXTURE_PATH);
    if (!wallTexture) {
        std::cerr << "Không thể tải ảnh tường: " << WALL_TEXTURE_PATH << " | SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    grassTexture = IMG_LoadTexture(renderer, GRASS_TEXTURE_PATH);
    if (!grassTexture) {
        std::cerr << "Không thể tải ảnh cỏ: " << GRASS_TEXTURE_PATH << " | SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    emptyTexture = IMG_LoadTexture(renderer, EMPTY_TEXTURE_PATH);
    if (!emptyTexture) {
        std::cerr << "Không thể tải ảnh ô trống: " << EMPTY_TEXTURE_PATH << " | SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}


// Hàm giải phóng texture
void freeTextures() {
    // Giải phóng texture tường
    if (wallTexture) {
        SDL_DestroyTexture(wallTexture);
        wallTexture = nullptr;
    }
    // Giải phóng texture cỏ
    if (grassTexture) {
        SDL_DestroyTexture(grassTexture);
        grassTexture = nullptr;
    }
    // Giải phóng texture ô trống
    if (emptyTexture) {
        SDL_DestroyTexture(emptyTexture);
        emptyTexture = nullptr;
    }
}


// Hàm tạo map với tường, bụi cỏ và các ô trống
std::vector<std::vector<TileType>> createMap() {
    std::vector<std::vector<TileType>> map(mapRows, std::vector<TileType>(mapCols, EMPTY));

    // Đặt tường viền
    for (int y = 0; y < mapRows; y++) {
        for (int x = 0; x < mapCols; x++) {
            if (x == 0 || y == 0 || x == mapCols - 1 || y == mapRows - 1) {
                map[y][x] = WALL; // Tường bao quanh
            }
        }
    }

    // Thêm các tường bên trong một cách ngẫu nhiên
    for (int y = 1; y < mapRows - 1; y++) {
        for (int x = 1; x < mapCols - 1; x++) {
            if (rand() % 10 < 2) { // 20% xác suất là tường
                map[y][x] = WALL;
            }
        }
    }

    // Đảm bảo các ô trống được kết nối
    std::vector<std::vector<bool>> visited(mapRows, std::vector<bool>(mapCols, false));
    int startX = 1, startY = 1;

    std::queue<std::pair<int, int>> q;
    q.push({startX, startY});
    visited[startY][startX] = true;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (auto [dx, dy] : std::vector<std::pair<int, int>>{{0, -1}, {0, 1}, {-1, 0}, {1, 0}}) {
            int nx = x + dx, ny = y + dy;
            if (nx > 0 && ny > 0 && nx < mapCols - 1 && ny < mapRows - 1 &&
                map[ny][nx] != WALL && !visited[ny][nx]) {
                visited[ny][nx] = true;
                q.push({nx, ny});
            }
        }
    }

    // Biến các vùng bị cô lập thành tường
    for (int y = 1; y < mapRows - 1; y++) {
        for (int x = 1; x < mapCols - 1; x++) {
            if (!visited[y][x] && map[y][x] == EMPTY) {
                map[y][x] = WALL;
            }
        }
    }

    // Thêm 8 bụi cỏ ngẫu nhiên
    int grassCount = 0;
    while (grassCount < 8) {
        int x = rand() % mapCols;
        int y = rand() % mapRows;

        if (map[y][x] == EMPTY) {
            map[y][x] = GRASS;
            grassCount++;
        }
    }

    return map;
}

// Hàm vẽ map sử dụng texture
void renderMap(SDL_Renderer* renderer, const std::vector<std::vector<TileType>>& map) {
    for (int y = 0; y < mapRows; y++) {
        for (int x = 0; x < mapCols; x++) {
            SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_Texture* texture = nullptr;

            if (map[y][x] == WALL) {
                texture = wallTexture; // Khôi phục lại sử dụng ảnh tường
            } else if (map[y][x] == GRASS) {
                texture = grassTexture; // Giữ nguyên ảnh texture cỏ
            } else {
                texture = emptyTexture; // Giữ nguyên ảnh texture ô trống
            }

            if (texture) {
                SDL_RenderCopy(renderer, texture, nullptr, &rect);
            }
        }
    }
}
