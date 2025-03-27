#include <SDL.h>
#include <SDL_image.h>
#include "map.h"
#include "Character.h"
#include "boom.h"
#include "enemy.h"
#include "Item.h"
#include "menu.h"
#include "constants.h"
#include "music.h"
#include <iostream>
#include <vector>
#include "game_logic.h"

// Theo dõi trạng thái phím nhấn
bool keyStates[SDL_NUM_SCANCODES] = {false}; // Mảng để lưu trạng thái của tất cả các phím

int main(int argc, char* argv[]) {
    Uint32 lastBombTime = 0; // Thời gian đặt bom cuối cùng, bắt đầu từ 0

    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "Không thể khởi tạo SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Bomber Friends", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer) {
        std::cerr << "Không thể tạo cửa sổ hoặc renderer: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Khởi tạo âm thanh
    initializeMusic();
    playBackgroundMusic(BACKGROUND_MUSIC_PATH); // Phát nhạc nền

    // *** TẢI TẤT CẢ TEXTURE ***
    if (!loadTextures(renderer)) {
        std::cerr << "Lỗi khi tải texture bản đồ." << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Tải texture cho bom và hiệu ứng nổ
    if (!loadBombTexture(renderer)) {
        std::cerr << "Lỗi khi tải texture của bom và hiệu ứng nổ." << std::endl;
        freeTextures(); // Giải phóng các texture của bản đồ
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (!loadEnemyTexture(renderer, ENEMY_TEXTURE_PATH)) {
        std::cerr << "Lỗi khi tải texture của kẻ địch." << std::endl;
        freeTextures();
        freeBombTexture(); // Giải phóng các texture của bom
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (!loadItemTextures(renderer)) {
        std::cerr << "Lỗi khi tải texture của vật phẩm." << std::endl;
        freeTextures();
        freeBombTexture(); // Giải phóng các texture của bom
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    initMenuTextures(renderer); // Tải texture cho menu

    Character player;
    initializeCharacter(player, 1, 1, renderer); // Khởi tạo texture nhân vật
    player.lives = 3; // Khởi tạo số mạng nhân vật

    // Menu
    bool isRunning = true;
    bool startGame = false;
    bool showHelp = false;
    bool isPaused = false;

    // *** HIỂN THỊ MENU CHÍNH ***
    while (!startGame && isRunning) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
    handleMenuEvent(event, isRunning, startGame);
}

renderMenu(renderer); // Hiển thị menu chính

    }

    // *** KHỞI TẠO GAME ***
    std::vector<std::vector<TileType>> gameMap = createMap();
    std::vector<Bomb> bombs;
    std::vector<Enemy> enemies;
    initializeEnemies(enemies,12, gameMap);
    std::vector<Item> items;
    generateItems(items, 15, gameMap);

    // Vòng lặp chính
    SDL_Event event;
    while (isRunning) {
        Uint32 currentTime = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false; // Kết thúc game nếu người chơi thoát
            }
            if (event.type == SDL_KEYDOWN) {
                keyStates[event.key.keysym.scancode] = true; // Lưu trạng thái phím nhấn
            }
            if (event.type == SDL_KEYUP) {
                keyStates[event.key.keysym.scancode] = false; // Lưu trạng thái phím nhả
                moveCharacter(player, gameMap, event);
            }

            // Đặt bom khi nhấn SPACE
            if (keyStates[SDL_SCANCODE_SPACE]) {
                if (currentTime - lastBombTime > 1000) { // Cooldown 1 giây
                    placeBomb(player.x, player.y, player.bombRange, player.direction, bombs, renderer, gameMap);
SDL_RenderPresent(renderer); // Hiển thị phạm vi bom ngay lập tức

                    playBombExplosion();
                    lastBombTime = currentTime;
                }
            }

            // Xử lý nút tạm dừng
            if (!isPaused && event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                SDL_Rect pauseButton = {WINDOW_WIDTH - 120, 20, 100, 50};
                if (mouseX >= pauseButton.x && mouseX <= pauseButton.x + pauseButton.w &&
                    mouseY >= pauseButton.y && mouseY <= pauseButton.y + pauseButton.h) {
                    isPaused = true;
                    Mix_PauseMusic();
                }
            }
        }

        // Xử lý khi game bị tạm dừng
        if (isPaused) {
            renderPauseMenu(renderer);
            SDL_Event pauseEvent;
            while (SDL_WaitEvent(&pauseEvent)) {
                if (pauseEvent.type == SDL_QUIT) {
                    isRunning = false;
                    break;
                }
                handlePauseEvent(pauseEvent, isPaused, isRunning);
                if (!isPaused) {
                    Mix_ResumeMusic();
                    break;
                }
            }
            continue;
        }

        // Cập nhật trạng thái miễn nhiễm của nhân vật
        if (player.isInvincible && currentTime - player.invincibleStartTime > 3000) {
            player.isInvincible = false; // Kết thúc miễn nhiễm sau 3 giây
        }

        // Cập nhật trạng thái game
        updateBombs(bombs, gameMap, enemies, gameMap.size(), gameMap[0].size(), renderer, player, isRunning);
        updateEnemies(enemies, gameMap, player, renderer, isRunning);


        // Kiểm tra va chạm giữa người chơi và vật phẩm
        handleItemCollision(player, items);

        // Kiểm tra chiến thắng hoặc thất bại
        checkVictoryCondition(renderer, enemies, isRunning);

        // Xử lý kết thúc trò chơi nếu nhân vật hết mạng
        if (player.lives <= 0) {
            Mix_HaltMusic();
            playLoseHealth();
            renderGameOverScreen(renderer);
            isRunning = false;
        }

        // Render tất cả các phần tử trong game
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderMap(renderer, gameMap);
        renderCharacter(renderer, player);
        renderBombs(renderer, bombs);
        renderEnemies(renderer, enemies);
        renderItems(renderer, items);
        renderLives(renderer, player);

        SDL_Rect pauseButton = {WINDOW_WIDTH - 120, 20, 100, 50};
        if (pauseButtonTexture) {
            SDL_RenderCopy(renderer, pauseButtonTexture, nullptr, &pauseButton);
        }

        SDL_RenderPresent(renderer);
    }

    // *** DỌN DẸP TÀI NGUYÊN ***
    freeTextures();
    freeBombTexture();               // Dọn dẹp texture của bom
    freeItemTextures();              // Dọn dẹp texture của vật phẩm
    cleanupCharacter(player);        // Dọn dẹp texture của nhân vật
    cleanupMusic();                  // Dừng và giải phóng âm nhạc
    SDL_DestroyRenderer(renderer);   // Hủy renderer
    SDL_DestroyWindow(window);       // Hủy cửa sổ game
    SDL_Quit();                      // Kết thúc SDL

    return 0;
}
