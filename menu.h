
#ifndef MENU_H
#define MENU_H

#include <SDL.h>

// Khai báo biến texture cho các hình ảnh
extern SDL_Texture* menuTexture;
extern SDL_Texture* gameOverTexture;
extern SDL_Texture* helpTexture;
extern SDL_Texture* pauseTexture;
extern SDL_Texture* pauseButtonTexture; // Texture cho nút Pause
extern SDL_Texture* playButtonTexture;  // Texture cho nút Play

extern SDL_Texture* helpButtonTexture;   // Texture cho nút Help
extern SDL_Texture* exitButtonTexture;  // Texture cho nút Exit
extern SDL_Texture* resumeButtonTexture; // Texture cho nút Resume (Pause menu)

extern SDL_Texture* victoryTexture;     // Texture cho màn hình chiến thắng

/**
 * @brief Tải các texture cho menu, bao gồm các nút, nền, và màn hình chiến thắng.
 *
 * @param renderer Renderer SDL để tải ảnh.
 */
void initMenuTextures(SDL_Renderer* renderer);

/**
 * @brief Giải phóng bộ nhớ của các texture, bao gồm các nút, nền, và màn hình chiến thắng.
 */
void freeMenuTextures();

/**
 * @brief Hiển thị menu chính.
 *
 * @param renderer Renderer SDL để vẽ nội dung menu.
 */
void renderMenu(SDL_Renderer* renderer);

/**
 * @brief Hiển thị nút Play trong menu chính.
 *
 * @param renderer Renderer SDL để vẽ nút Play.
 */
void renderPlayButton(SDL_Renderer* renderer);

/**
 * @brief Hiển thị màn hình Game Over.
 *
 * @param renderer Renderer SDL để vẽ màn hình Game Over.
 */
void renderGameOverScreen(SDL_Renderer* renderer);

/**
 * @brief Hiển thị màn hình trợ giúp.
 *
 * @param renderer Renderer SDL để vẽ màn hình hướng dẫn.
 */
void renderHelp(SDL_Renderer* renderer);

/**
 * @brief Hiển thị menu Pause.
 *
 * @param renderer Renderer SDL để vẽ menu Pause.
 */
void renderPauseMenu(SDL_Renderer* renderer);

/**
 * @brief Hiển thị nút Pause trong menu Pause.
 *
 * @param renderer Renderer SDL để vẽ nút Pause.
 */
void renderPauseButton(SDL_Renderer* renderer);

/**
 * @brief Hiển thị màn hình chiến thắng.
 *
 * @param renderer Renderer SDL để vẽ màn hình chiến thắng.
 */
void renderVictoryScreen(SDL_Renderer* renderer);

/**
 * @brief Xử lý các sự kiện từ menu chính.
 *
 * @param event Sự kiện SDL hiện tại.
 * @param isRunning Biến kiểm tra trạng thái trò chơi.
 * @param startGame Biến kiểm tra trạng thái bắt đầu trò chơi.
 * @param showHelp Biến kiểm tra trạng thái hiển thị trợ giúp.
 */
void handleMenuEvent(SDL_Event& event, bool& isRunning, bool& startGame);

/**
 * @brief Xử lý các sự kiện từ menu Pause.
 *
 * @param event Sự kiện SDL hiện tại.
 * @param isPaused Biến kiểm tra trạng thái Pause.
 * @param isRunning Biến kiểm tra trạng thái trò chơi.
 */
void handlePauseEvent(SDL_Event& event, bool& isPaused, bool& isRunning);

#endif // MENU_H
