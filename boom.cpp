
#include "boom.h"
#include "constants.h"
#include <iostream>
#include <SDL_image.h>
#include "music.h"
#include "menu.h"
#include "map.h"

// Texture cho bom và hiệu ứng
SDL_Texture* bombTexture = nullptr;            // Texture cho bom
SDL_Texture* bombExplosionTexture = nullptr;   // Texture cho hiệu ứng nổ bom
SDL_Texture* bombRangeTexture = nullptr;       // Texture cho phạm vi nổ bom

// Tải texture cho bom và các hiệu ứng
bool loadBombTexture(SDL_Renderer* renderer) {
    // Tải ảnh bom
    SDL_Surface* bombSurface = IMG_Load(BOMB_TEXTURE_PATH);
    if (!bombSurface) {
        std::cerr << "Không thể tải ảnh bom: " << IMG_GetError() << std::endl;
        return false;
    }
    bombTexture = SDL_CreateTextureFromSurface(renderer, bombSurface);
    SDL_FreeSurface(bombSurface);

    // Tải ảnh hiệu ứng nổ bom
    SDL_Surface* explosionSurface = IMG_Load(BOMB_EXPLOSION_TEXTURE_PATH);
    if (!explosionSurface) {
        std::cerr << "Không thể tải ảnh hiệu ứng nổ bom: " << IMG_GetError() << std::endl;
        return false;
    }
    bombExplosionTexture = SDL_CreateTextureFromSurface(renderer, explosionSurface);
    SDL_FreeSurface(explosionSurface);

    // Tải ảnh phạm vi bom nổ
    SDL_Surface* rangeSurface = IMG_Load(BOMB_RANGE_TEXTURE_PATH);
    if (!rangeSurface) {
        std::cerr << "Không thể tải ảnh phạm vi bom: " << IMG_GetError() << std::endl;
        return false;
    }
    bombRangeTexture = SDL_CreateTextureFromSurface(renderer, rangeSurface);
    SDL_FreeSurface(rangeSurface);

    return bombTexture && bombExplosionTexture && bombRangeTexture;
}

// Dọn dẹp texture bom và hiệu ứng
void freeBombTexture() {
    if (bombTexture) {
        SDL_DestroyTexture(bombTexture);
        bombTexture = nullptr;
    }
    if (bombExplosionTexture) {
        SDL_DestroyTexture(bombExplosionTexture);
        bombExplosionTexture = nullptr;
    }
    if (bombRangeTexture) {
        SDL_DestroyTexture(bombRangeTexture);
        bombRangeTexture = nullptr;
    }
}

// Đặt bom trên bản đồ
void placeBomb(int x, int y, int range, Direction direction, std::vector<Bomb>& bombs, SDL_Renderer* renderer, std::vector<std::vector<TileType>>& map) {
    // Kiểm tra bom trùng lặp
    for (const auto& bomb : bombs) {
        if (bomb.x == x && bomb.y == y) {
            std::cerr << "Đã có bom tại vị trí: (" << x << ", " << y << ")" << std::endl;
            return; // Ngừng nếu có bom trùng lặp
        }
    }

    // Kiểm tra vị trí trên bản đồ
    if (map[y][x] != EMPTY && map[y][x] != GRASS) {
        std::cerr << "Không thể đặt bom tại vị trí: (" << x << ", " << y << ")" << std::endl;
        return; // Ngừng nếu không thể đặt bom tại ô này
    }

    // Thêm bom vào danh sách
    Bomb bomb = {x, y, range, SDL_GetTicks() + 3000, direction}; // Bom sẽ nổ sau 3 giây
    bombs.push_back(bomb);

    // Hiển thị phạm vi bom ngay sau khi đặt
    renderBombRange(renderer, bomb, map, map.size(), map[0].size());

    std::cout << "Đã đặt bom tại vị trí: (" << x << ", " << y << ")" << std::endl; // Thông báo đặt bom
}



// Xử lý hiệu ứng nổ bom
void handleExplosion(int x, int y, int range,
                     std::vector<Enemy>& enemies,
                     std::vector<std::vector<TileType>>& map,
                     SDL_Renderer* renderer,
                     Character& character,
                     bool& isRunning) {
    // Phá vật cản hoặc tiêu diệt kẻ địch trong phạm vi nổ
    for (int i = 0; i <= range; i++) { // Phạm vi nổ là 2 ô
        // Phá ngang (trái và phải)
        if ((x + i >= 0 && x + i < map[0].size()) && (map[y][x + i] == WALL || map[y][x + i] == GRASS)) {
            map[y][x + i] = EMPTY; // Phá tường hoặc bụi cỏ
        }
        if ((x - i >= 0 && x - i < map[0].size()) && (map[y][x - i] == WALL || map[y][x - i] == GRASS)) {
            map[y][x - i] = EMPTY;
        }

        // Phá dọc (trên và dưới)
        if ((y + i >= 0 && y + i < map.size()) && (map[y + i][x] == WALL || map[y + i][x] == GRASS)) {
            map[y + i][x] = EMPTY;
        }
        if ((y - i >= 0 && y - i < map.size()) && (map[y - i][x] == WALL || map[y - i][x] == GRASS)) {
            map[y - i][x] = EMPTY;
        }

        // Tiêu diệt kẻ địch
        for (auto it = enemies.begin(); it != enemies.end();) {
            if ((it->x == x + i && it->y == y) || (it->x == x - i && it->y == y) ||
                (it->x == x && it->y == y + i) || (it->x == x && it->y == y - i)) {
                it = enemies.erase(it); // Xóa kẻ địch khỏi danh sách
            } else {
                ++it;
            }
        }
    }

    // Kiểm tra va chạm với nhân vật
    for (int i = 0; i <= range; i++) { // Phạm vi nổ là 2 ô
        if ((character.x == x + i && character.y == y) ||  // Phạm vi phải
            (character.x == x - i && character.y == y) ||  // Phạm vi trái
            (character.x == x && character.y == y + i) ||  // Phạm vi dưới
            (character.x == x && character.y == y - i)) {  // Phạm vi trên

            // Giảm số mạng của nhân vật nếu bị trúng bom
            if (!character.isInvincible) { // Chỉ trừ mạng nếu không miễn nhiễm
                character.lives -= 1;
                character.isInvincible = true; // Bật trạng thái miễn nhiễm
                character.invincibleStartTime = SDL_GetTicks(); // Ghi nhận thời gian bắt đầu miễn nhiễm

                std::cout << "Nhân vật bị bom nổ trúng! Số mạng còn lại: " << character.lives << std::endl;

                // Kết thúc trò chơi nếu nhân vật hết mạng
                if (character.lives <= 0) {
                    renderGameOverScreen(renderer); // Hiển thị màn hình Game Over
                    isRunning = false; // Kết thúc game
                }
            }
        }
    }

    // Hiển thị hiệu ứng nổ tại vị trí bom
    renderBombExplosion(renderer, x, y);
}


// Cập nhật trạng thái bom
void updateBombs(std::vector<Bomb>& bombs,
                 std::vector<std::vector<TileType>>& map,
                 std::vector<Enemy>& enemies,
                 int mapRows,
                 int mapCols,
                 SDL_Renderer* renderer,
                 Character& player,
                 bool& isRunning) {
    Uint32 currentTime = SDL_GetTicks();

    // Lặp qua danh sách các bom
    for (auto it = bombs.begin(); it != bombs.end();) {
        // Hiển thị phạm vi nổ của bom cho đến khi bom phát nổ
        renderBombRange(renderer, *it, map, mapRows, mapCols);

        // Kiểm tra nếu đã đến thời gian bom phát nổ
        if (currentTime >= it->explodeTime) {
            std::cout << "Bom phát nổ tại vị trí: (" << it->x << ", " << it->y << ")" << std::endl;

            // Xóa phạm vi bom bằng cách vẽ lại bản đồ
            renderMap(renderer, map);

            // Xử lý logic vụ nổ bom
            handleExplosion(it->x, it->y, it->range, enemies, map, renderer, player, isRunning);


            // Phát âm thanh hiệu ứng bom nổ
            playBombExplosion();

            // Hiển thị hiệu ứng nổ bom tại vị trí
            renderBombExplosion(renderer, it->x, it->y);

            // Xóa bom khỏi danh sách sau khi nổ
            it = bombs.erase(it);
        } else {
            ++it; // Tiếp tục với các bom khác
        }
    }

    // Kiểm tra trạng thái miễn nhiễm của nhân vật
    Uint32 currentTimeForPlayer = SDL_GetTicks();
    if (player.isInvincible && currentTimeForPlayer - player.invincibleStartTime > 3000) {
        player.isInvincible = false; // Hết miễn nhiễm sau 3 giây
        std::cout << "Nhân vật không còn miễn nhiễm." << std::endl;
    }
}



// Hiển thị bom trên màn hình
void renderBombs(SDL_Renderer* renderer, const std::vector<Bomb>& bombs) {
    for (const auto& bomb : bombs) {
        SDL_Rect rect = {bomb.x * TILE_SIZE, bomb.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        if (bombTexture) {
            SDL_RenderCopy(renderer, bombTexture, nullptr, &rect); // Vẽ bom
        } else {
            std::cerr << "Texture của bom chưa được tải." << std::endl;
        }
    }
}

// Hiển thị phạm vi bom
void renderBombRange(SDL_Renderer* renderer, const Bomb& bomb, const std::vector<std::vector<TileType>>& map, int mapRows, int mapCols) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Màu đỏ RGBA

    for (int i = 1; i <= bomb.range; i++) {
        // Vẽ ô bên phải
        if (bomb.x + i < mapCols && map[bomb.y][bomb.x + i] == EMPTY) {
            SDL_Rect rectRight = {(bomb.x + i) * TILE_SIZE, bomb.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &rectRight);
        } else {
            break; // Dừng nếu gặp vật cản
        }

        // Vẽ ô bên trái
        if (bomb.x - i >= 0 && map[bomb.y][bomb.x - i] == EMPTY) {
            SDL_Rect rectLeft = {(bomb.x - i) * TILE_SIZE, bomb.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &rectLeft);
        } else {
            break; // Dừng nếu gặp vật cản
        }

        // Vẽ ô phía dưới
        if (bomb.y + i < mapRows && map[bomb.y + i][bomb.x] == EMPTY) {
            SDL_Rect rectDown = {bomb.x * TILE_SIZE, (bomb.y + i) * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &rectDown);
        } else {
            break; // Dừng nếu gặp vật cản
        }

        // Vẽ ô phía trên
        if (bomb.y - i >= 0 && map[bomb.y - i][bomb.x] == EMPTY) {
            SDL_Rect rectUp = {bomb.x * TILE_SIZE, (bomb.y - i) * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &rectUp);
        } else {
            break; // Dừng nếu gặp vật cản
        }
    }
}




// Hiển thị hiệu ứng nổ bom
void renderBombExplosion(SDL_Renderer* renderer, int x, int y) {
    if (bombExplosionTexture) {
        SDL_Rect rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, bombExplosionTexture, nullptr, &rect); // Hiển thị hiệu ứng nổ
    } else {
        std::cerr << "Texture của hiệu ứng nổ bom chưa được tải!" << std::endl;
    }
}
