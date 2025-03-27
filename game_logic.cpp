
#include "game_logic.h"
#include <iostream> // Để xuất thông tin debug nếu cần
// Hàm kiểm tra điều kiện chiến thắng
#include "menu.h"
#include "constants.h"
#include "Character.h"
#include "boom.h"
void checkVictoryCondition(SDL_Renderer* renderer,
                           std::vector<Enemy>& enemies,
                           bool& isRunning) {
    if (enemies.empty()) { // Nếu không còn kẻ địch
        // Kiểm tra texture của màn hình chiến thắng
        if (victoryTexture) {
            SDL_RenderCopy(renderer, victoryTexture, nullptr, nullptr); // Hiển thị ảnh chiến thắng
        } else {
            // Màu nền và thông báo mặc định nếu không có texture
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Màu nền xanh lá cây
            SDL_RenderClear(renderer);

            SDL_Rect victoryRect = {100, 200, 600, 100}; // Kích thước hộp
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Màu chữ trắng
            SDL_RenderFillRect(renderer, &victoryRect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(3000); // Hiển thị màn hình chiến thắng trong 3 giây

        isRunning = false; // Kết thúc game
    }
}
// Xử lý va chạm với các đối tượng khác
void handleCollision(Character& player) {
    if (player.lives > 0) {
        player.lives--; // Trừ mạng
        std::cout << "Bạn bị trừ 1 mạng. Số mạng còn lại: " << player.lives << std::endl;
    }

    if (player.lives <= 0) {
        std::cout << "Game Over! Bạn đã thua." << std::endl;
        SDL_Delay(2000);
        player.lives = 0; // Đảm bảo trạng thái không gây lỗi
    }
}

void handlePlayerHitByBomb(Character& player, const Bomb& bomb, Uint32 currentTime, bool& isRunning, SDL_Renderer* renderer) {
    // Kiểm tra trạng thái miễn nhiễm
    if (player.isInvincible) {
        Uint32 elapsedTime = currentTime - player.invincibleStartTime;
        if (elapsedTime < 3000) { // Miễn nhiễm trong 3 giây
            return;
        }
        player.isInvincible = false; // Hết miễn nhiễm
    }

    // Kiểm tra nếu nhân vật nằm trong phạm vi nổ của bom
    for (int i = 0; i <= bomb.range; ++i) {
        if ((player.x == bomb.x + i && player.y == bomb.y) ||  // Nổ sang phải
            (player.x == bomb.x - i && player.y == bomb.y) ||  // Nổ sang trái
            (player.x == bomb.x && player.y == bomb.y + i) ||  // Nổ xuống
            (player.x == bomb.x && player.y == bomb.y - i)) {  // Nổ lên

            // Trừ mạng của nhân vật
            player.lives -= 1;
            player.isInvincible = true; // Bắt đầu miễn nhiễm
            player.invincibleStartTime = currentTime;

            std::cout << "Nhân vật bị bom nổ trúng! Số mạng còn lại: " << player.lives << std::endl;

            // Kết thúc trò chơi nếu hết mạng
            if (player.lives <= 0) {
                renderGameOverScreen(renderer); // Gọi màn hình Game Over (nếu đã triển khai)
                isRunning = false; // Kết thúc game
            }

            return;
        }
    }
}
