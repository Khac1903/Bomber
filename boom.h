
#ifndef BOOM_H
#define BOOM_H

#include <vector>
#include <SDL.h>
#include "map.h"
#include "enemy.h"
#include "Character.h" // Lưu ý tên file viết hoa chữ "C"
#include "constants.h"

// Cấu trúc Bom
struct Bomb {
    int x, y;                   // Vị trí bom
    int range;        // Phạm vi nổ theo chiều ngang
    Uint32 explodeTime;         // Thời gian bom nổ
    Direction direction;        // Hướng nổ
};


// Texture cho bom và hiệu ứng
extern SDL_Texture* bombTexture;            // Texture cho bom
extern SDL_Texture* bombExplosionTexture;   // Texture cho hiệu ứng nổ bom
extern SDL_Texture* bombRangeTexture;       // Texture cho phạm vi bom

// Khai báo các hàm
bool loadBombTexture(SDL_Renderer* renderer);           // Tải texture cho bom và các hiệu ứng
void freeBombTexture();                                 // Giải phóng texture của bom và các hiệu ứng

void placeBomb(int x, int y, int range, Direction direction, std::vector<Bomb>& bombs, SDL_Renderer* renderer, std::vector<std::vector<TileType>>& map) ; // Đặt bom
void handleExplosion(int x, int y, int range,
                     std::vector<Enemy>& enemies,
                     std::vector<std::vector<TileType>>& map,
                     SDL_Renderer* renderer,
                     Character& character,
                     bool& isRunning); // xử lý hiệu ứng nổ bom

void updateBombs(std::vector<Bomb>& bombs,
                 std::vector<std::vector<TileType>>& map,
                 std::vector<Enemy>& enemies,
                 int mapRows,
                 int mapCols,
                 SDL_Renderer* renderer,
                 Character& player,
                 bool& isRunning) ;
 // Cập nhật trạng thái bom

void renderBombs(SDL_Renderer* renderer, const std::vector<Bomb>& bombs);       // Hiển thị bom trên màn hình


#endif // BOOM_H
