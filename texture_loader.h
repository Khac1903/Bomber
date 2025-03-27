
#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <SDL.h>
#include <string>

// Hàm tải ảnh và tạo texture từ ảnh
SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);

#endif // TEXTURE_LOADER_H
