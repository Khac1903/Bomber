#include "menu.h"
#include "constants.h" // Bao gồm hằng số đường dẫn
#include <SDL_image.h>
#include <iostream>

// Khai báo biến lưu texture của các hình ảnh
// Xóa các khai báo không cần thiết
SDL_Texture* menuTexture = nullptr;
SDL_Texture* gameOverTexture = nullptr;
SDL_Texture* pauseTexture = nullptr;
SDL_Texture* pauseButtonTexture = nullptr; // Texture cho nút Pause
SDL_Texture* playButtonTexture = nullptr;  // Texture cho nút Play
SDL_Texture* exitButtonTexture = nullptr;  // Texture cho nút Exit
SDL_Texture* resumeButtonTexture = nullptr; // Texture cho nút Resume
SDL_Texture* victoryTexture = nullptr;     // Texture cho màn hình chiến thắng

// Hàm tải ảnh
SDL_Texture* loadTexture(const char* filePath, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(filePath);
    if (!loadedSurface) {
        std::cerr << "Không thể tải ảnh: " << filePath << " SDL_image Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

// Hàm khởi tạo ảnh nền và nút
void initMenuTextures(SDL_Renderer* renderer) {
    // Tải ảnh nền menu
    menuTexture = loadTexture(MENU_BACKGROUND_PATH, renderer);

    // Tải ảnh khi kết thúc trò chơi (Game Over)
    gameOverTexture = loadTexture(GAME_OVER_BACKGROUND_PATH, renderer);

    // Tải ảnh cho nút Play
    playButtonTexture = loadTexture(PLAY_BUTTON_PATH, renderer);

    // Tải ảnh cho nút Pause
    pauseButtonTexture = loadTexture(PAUSE_BUTTON_PATH, renderer);

    // Tải ảnh cho nút Exit
    exitButtonTexture = loadTexture(EXIT_BUTTON_PATH, renderer);

    // Tải ảnh cho nút Resume
    resumeButtonTexture = loadTexture(RESUME_BUTTON_PATH, renderer);

    // Tải ảnh cho màn hình chiến thắng
    victoryTexture = loadTexture(VICTORY_BACKGROUND_PATH, renderer);
}



// Hiển thị menu chính
void renderMenu(SDL_Renderer* renderer) {
    if (menuTexture) {
        SDL_RenderCopy(renderer, menuTexture, nullptr, nullptr);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    SDL_Rect playButton = {300, 150, 200, 100};
    SDL_Rect exitButton = {300, 250, 200, 100}; // Thay đổi vị trí nút Exit

    // Hiển thị ảnh nút Play
    if (playButtonTexture) {
        SDL_RenderCopy(renderer, playButtonTexture, nullptr, &playButton);
    }

    // Hiển thị ảnh nút Exit
    if (exitButtonTexture) {
        SDL_RenderCopy(renderer, exitButtonTexture, nullptr, &exitButton);
    }

    SDL_RenderPresent(renderer);
}

// Hiển thị menu Pause
void renderPauseMenu(SDL_Renderer* renderer) {
    if (pauseTexture) {
        SDL_RenderCopy(renderer, pauseTexture, nullptr, nullptr);
    } else {
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 200);
        SDL_RenderClear(renderer);
    }

    SDL_Rect resumeButton = {250, 200, 300, 100}; // Nút Resume
    SDL_Rect quitButton = {250, 300, 300, 100};  // Nút Quit

    // Hiển thị ảnh nút Resume
    if (resumeButtonTexture) {
        SDL_RenderCopy(renderer, resumeButtonTexture, nullptr, &resumeButton);
    }

    // Hiển thị ảnh nút Quit
    if (exitButtonTexture) {
        SDL_RenderCopy(renderer, exitButtonTexture, nullptr, &quitButton);
    }

    SDL_RenderPresent(renderer);
}

// Hiển thị nút Pause ở góc trên bên phải
void renderPauseButton(SDL_Renderer* renderer) {
    SDL_Rect pauseButton = {WINDOW_WIDTH - 120, 20, 100, 50}; // Góc trên bên phải
    if (pauseButtonTexture) {
        SDL_RenderCopy(renderer, pauseButtonTexture, nullptr, &pauseButton);
    }
}

// Hiển thị màn hình Game Over
void renderGameOverScreen(SDL_Renderer* renderer) {
    if (gameOverTexture) {
        SDL_RenderCopy(renderer, gameOverTexture, nullptr, nullptr);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);
        std::cerr << "Texture Game Over chưa được tải!" << std::endl;
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
}


// Hiển thị màn hình trợ giúp

// Hiển thị màn hình chiến thắng
void renderVictoryScreen(SDL_Renderer* renderer) {
    if (victoryTexture) {
        SDL_RenderCopy(renderer, victoryTexture, nullptr, nullptr); // Hiển thị toàn màn hình
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Màu nền mặc định (xanh lá cây)
        SDL_RenderClear(renderer);
        std::cerr << "Texture màn hình chiến thắng chưa được tải, hiển thị màu nền mặc định." << std::endl;
    }

    SDL_RenderPresent(renderer);

    // Tạm dừng 3 giây để người chơi thấy màn hình chiến thắng
    SDL_Delay(3000);
}

// Xử lý sự kiện menu chính
void handleMenuEvent(SDL_Event& event, bool& isRunning, bool& startGame) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        // Nhấn vào nút Play
        if (mouseX >= 300 && mouseX <= 500 && mouseY >= 150 && mouseY <= 250) {
            startGame = true;
        }

        // Nhấn vào nút Exit
        if (mouseX >= 300 && mouseX <= 500 && mouseY >= 250 && mouseY <= 350) {
            isRunning = false;
        }
    }
}


// Xử lý sự kiện menu Pause
void handlePauseEvent(SDL_Event& event, bool& isPaused, bool& isRunning) {
    if (event.type == SDL_MOUSEBUTTONDOWN) { // Kiểm tra sự kiện nhấn chuột
        int mouseX = event.button.x;        // Tọa độ X chuột
        int mouseY = event.button.y;        // Tọa độ Y chuột

        // Vị trí nút Resume
        SDL_Rect resumeButton = {250, 200, 300, 100};
        // Vị trí nút Quit
        SDL_Rect quitButton = {250, 300, 300, 100};

        // Nhấn vào nút Resume
        if (mouseX >= resumeButton.x && mouseX <= resumeButton.x + resumeButton.w &&
            mouseY >= resumeButton.y && mouseY <= resumeButton.y + resumeButton.h) {
            isPaused = false; // Tiếp tục trò chơi
        }

        // Nhấn vào nút Quit
        if (mouseX >= quitButton.x && mouseX <= quitButton.x + quitButton.w &&
            mouseY >= quitButton.y && mouseY <= quitButton.y + quitButton.h) {
            isRunning = false; // Thoát trò chơi
        }
    }

    if (event.type == SDL_QUIT) { // Người chơi đóng cửa sổ
        isRunning = false; // Kết thúc game
    }
}
void freeMenuTextures() {
    if (menuTexture) {
        SDL_DestroyTexture(menuTexture);
        menuTexture = nullptr;
    }
    if (gameOverTexture) {
        SDL_DestroyTexture(gameOverTexture);
        gameOverTexture = nullptr;
    }
    if (pauseTexture) {
        SDL_DestroyTexture(pauseTexture);
        pauseTexture = nullptr;
    }
    if (playButtonTexture) {
        SDL_DestroyTexture(playButtonTexture);
        playButtonTexture = nullptr;
    }
    if (pauseButtonTexture) {
        SDL_DestroyTexture(pauseButtonTexture);
        pauseButtonTexture = nullptr;
    }
    if (exitButtonTexture) {
        SDL_DestroyTexture(exitButtonTexture);
        exitButtonTexture = nullptr;
    }
    if (resumeButtonTexture) {
        SDL_DestroyTexture(resumeButtonTexture);
        resumeButtonTexture = nullptr;
    }
    if (victoryTexture) {
        SDL_DestroyTexture(victoryTexture);
        victoryTexture = nullptr;
    }

    std::cout << "Tất cả texture trong menu đã được giải phóng!" << std::endl;
}
