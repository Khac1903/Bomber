
#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <vector>
#include "map.h" // Tương tác với các ô trên bản đồ
#include "Character.h"

// Cấu trúc kẻ địch
struct Enemy {
    int x, y;             // Vị trí của kẻ địch
    int direction;        // Hướng di chuyển (0 = lên, 1 = xuống, 2 = trái, 3 = phải)
    Uint32 lastMoveTime;  // Thời gian di chuyển lần cuối
};

// Khai báo texture cho kẻ địch
extern SDL_Texture* enemyTexture;

// Khai báo các hàm xử lý kẻ địch
bool loadEnemyTexture(SDL_Renderer* renderer, const char* filePath);
void freeEnemyTexture();

void initializeEnemies(std::vector<Enemy>& enemies, int numEnemies, const std::vector<std::vector<TileType>>& map);
void moveEnemy(Enemy& enemy, const std::vector<std::vector<TileType>>& map, const std::vector<Enemy>& enemies);
void updateEnemies(std::vector<Enemy>& enemies, const std::vector<std::vector<TileType>>& map, Character& character, SDL_Renderer* renderer, bool& isRunning);
void renderEnemies(SDL_Renderer* renderer, const std::vector<Enemy>& enemies);

#endif
