
#include "texture_loader.h"
#include <SDL_image.h>
#include <iostream>

// Định nghĩa hàm loadTexture
SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;

    // Tải ảnh dưới dạng surface
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        // In thông báo lỗi nếu không tải được ảnh
        std::cerr << "Không thể tải ảnh: " << path << " SDL_image Error: " << IMG_GetError() << std::endl;
    } else {
        // Tạo texture từ surface
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (!newTexture) {
            // In thông báo lỗi nếu không tạo được texture
            std::cerr << "Không thể tạo texture từ ảnh! SDL Error: " << SDL_GetError() << std::endl;
        }

        // Giải phóng surface sau khi sử dụng
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}
