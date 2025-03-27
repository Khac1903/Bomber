
#include "enemy.h"
#include <cstdlib> // Để sử dụng rand()
#include <ctime>
#include <iostream>
#include <SDL_image.h>
#include "constants.h"
#include "menu.h"
SDL_Texture* enemyTexture = nullptr; // Texture của kẻ địch

// Tải ảnh kẻ địch
bool loadEnemyTexture(SDL_Renderer* renderer, const char* filePath) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        std::cerr << "Lỗi tải ảnh kẻ địch: " << IMG_GetError() << std::endl;
        return false;
    }
    enemyTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return enemyTexture != nullptr;
}


// Giải phóng texture
void freeEnemyTexture() {
    if (enemyTexture) {
        SDL_DestroyTexture(enemyTexture);
        enemyTexture = nullptr;
    }
}

// Khởi tạo danh sách kẻ địch
void initializeEnemies(std::vector<Enemy>& enemies, int numEnemies, const std::vector<std::vector<TileType>>& map) {
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < numEnemies; i++) {
        int x, y;
        do {
            x = rand() % map[0].size();
            y = rand() % map.size();
        } while (map[y][x] != EMPTY);
        enemies.push_back({x, y, rand() % 4, SDL_GetTicks()});
    }
}

// Di chuyển kẻ địch
void moveEnemy(Enemy& enemy, const std::vector<std::vector<TileType>>& map, const std::vector<Enemy>& enemies) {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - enemy.lastMoveTime < 500) return;

    int nextX = enemy.x, nextY = enemy.y;
    switch (enemy.direction) {
        case 0: nextY--; break;
        case 1: nextY++; break;
        case 2: nextX--; break;
        case 3: nextX++; break;
    }

    bool occupied = false;
    for (const auto& other : enemies) {
        if (nextX == other.x && nextY == other.y) {
            occupied = true;
            break;
        }
    }

    if (nextX >= 0 && nextX < map[0].size() && nextY >= 0 && nextY < map.size() && map[nextY][nextX] == EMPTY && !occupied) {
        enemy.x = nextX;
        enemy.y = nextY;
    } else {
        enemy.direction = rand() % 4;
    }

    enemy.lastMoveTime = currentTime;
}

// Cập nhật trạng thái kẻ địch
void updateEnemies(std::vector<Enemy>& enemies, const std::vector<std::vector<TileType>>& map, Character& character, SDL_Renderer* renderer, bool& isRunning) {
    Uint32 currentTime = SDL_GetTicks();

    for (auto& enemy : enemies) {
        // Move the enemy (this assumes `moveEnemy` function handles enemy movement logic)
        moveEnemy(enemy, map, enemies);

        // Check for collision with the character
        if (enemy.x == character.x && enemy.y == character.y && !character.isInvincible) {
            // Reduce character's lives
            character.lives--;
            character.isInvincible = true; // Activate invincibility
            character.invincibleStartTime = currentTime; // Set start time for invincibility

            std::cout << "Bạn bị tấn công! Mạng còn lại: " << character.lives << std::endl;

            // End the game if lives are 0
            if (character.lives <= 0) {
                renderGameOverScreen(renderer); // Display Game Over screen
                isRunning = false; // Stop the game loop
                return; // Exit the function
            }
        }
    }

    // Handle invincibility timeout (3 seconds)
    if (character.isInvincible && currentTime - character.invincibleStartTime > 3000) {
        character.isInvincible = false; // End invincibility
    }
}


// Vẽ kẻ địch
void renderEnemies(SDL_Renderer* renderer, const std::vector<Enemy>& enemies) {
    for (const auto& enemy : enemies) {
        SDL_Rect rect = {enemy.x * TILE_SIZE, enemy.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        if (enemyTexture) {
            SDL_RenderCopy(renderer, enemyTexture, nullptr, &rect);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
