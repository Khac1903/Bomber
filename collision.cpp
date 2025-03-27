
#include "collision.h"
#include <cmath> // Để sử dụng abs()

bool checkCollision(const Character& player, const std::vector<Character>& enemies) {
    for (const auto& enemy : enemies) {
        if (player.x == enemy.x && player.y == enemy.y) {
            return true; // Va chạm xảy ra
        }
    }
    return false;
}

bool checkBombCollision(const Character& player, const std::vector<Bomb>& bombs) {
    for (const auto& bomb : bombs) {
        // Kiểm tra phạm vi bom nổ (hình vuông)
        if (abs(player.x - bomb.x) <= bomb.range && abs(player.y - bomb.y) <= bomb.range) {
            return true; // Bị nổ trúng
        }
    }
    return false; // Không bị nổ trúng
}
