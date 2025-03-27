#include "music.h"
#include "constants.h" // Bao gồm hằng số đường dẫn
#include <iostream>

// Biến toàn cục để lưu trữ âm thanh
Mix_Music* backgroundMusic = nullptr;      // Nhạc nền
Mix_Chunk* bombExplosionSound = nullptr;   // Âm thanh bom nổ
Mix_Chunk* loseHealthSound = nullptr;      // Âm thanh mất máu
Mix_Chunk* pickupItemSound = nullptr;      // Âm thanh nhận vật phẩm

// Khởi tạo âm thanh
void initializeMusic() {
    // Khởi tạo SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Không thể khởi tạo SDL_mixer: " << Mix_GetError() << std::endl;
        return;
    }

    // Cấp số lượng kênh âm thanh (nếu cần nhiều kênh hơn cho hiệu ứng)
    Mix_AllocateChannels(16);

    // Tải nhạc nền
    backgroundMusic = Mix_LoadMUS(BACKGROUND_MUSIC_PATH);
    if (!backgroundMusic) {
        std::cerr << "Lỗi khi tải nhạc nền: " << Mix_GetError() << std::endl;
    } else {
        std::cerr << "Nhạc nền đã tải thành công!" << std::endl;
    }

    // Tải âm thanh hiệu ứng
    bombExplosionSound = Mix_LoadWAV(BOMB_EXPLOSION_SOUND_PATH);
    if (!bombExplosionSound) {
        std::cerr << "Lỗi khi tải âm thanh bom nổ: " << Mix_GetError() << std::endl;
    } else {
        std::cerr << "Âm thanh bom nổ đã tải thành công!" << std::endl;
    }

    loseHealthSound = Mix_LoadWAV(LOSE_HEALTH_SOUND_PATH);
    if (!loseHealthSound) {
        std::cerr << "Lỗi khi tải âm thanh mất máu: " << Mix_GetError() << std::endl;
    } else {
        std::cerr << "Âm thanh mất máu đã tải thành công!" << std::endl;
    }

    pickupItemSound = Mix_LoadWAV(PICKUP_ITEM_SOUND_PATH);
    if (!pickupItemSound) {
        std::cerr << "Lỗi khi tải âm thanh nhận vật phẩm: " << Mix_GetError() << std::endl;
    } else {
        std::cerr << "Âm thanh nhận vật phẩm đã tải thành công!" << std::endl;
    }
}

// Phát nhạc nền
void playBackgroundMusic(const char* musicFile) {
    if (backgroundMusic) {
        if (Mix_PlayingMusic() == 0) {
            Mix_PlayMusic(backgroundMusic, -1); // Lặp lại nhạc nền vô hạn
            std::cerr << "Đang phát nhạc nền!" << std::endl;
        }
    } else {
        std::cerr << "Nhạc nền chưa được tải!" << std::endl;
    }
}

// Dừng nhạc nền
void stopBackgroundMusic() {
    if (Mix_PlayingMusic() != 0) {
        Mix_HaltMusic();
        std::cerr << "Nhạc nền đã dừng!" << std::endl;
    }
}

// Phát hiệu ứng âm thanh
void playBombExplosion() {
    if (bombExplosionSound) {
        Mix_PlayChannel(-1, bombExplosionSound, 0);
        std::cerr << "Đang phát âm thanh bom nổ!" << std::endl;
    } else {
        std::cerr << "Âm thanh bom nổ chưa được tải!" << std::endl;
    }
}

void playLoseHealth() {
    if (loseHealthSound) {
        Mix_PlayChannel(-1, loseHealthSound, 0);
        std::cerr << "Đang phát âm thanh mất máu!" << std::endl;
    } else {
        std::cerr << "Âm thanh mất máu chưa được tải!" << std::endl;
    }
}

void playPickupItem() {
    if (pickupItemSound) {
        Mix_PlayChannel(-1, pickupItemSound, 0);
        std::cerr << "Đang phát âm thanh nhận vật phẩm!" << std::endl;
    } else {
        std::cerr << "Âm thanh nhận vật phẩm chưa được tải!" << std::endl;
    }
}

// Dọn dẹp tài nguyên âm thanh
void cleanupMusic() {
    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
        std::cerr << "Nhạc nền đã được dọn dẹp!" << std::endl;
    }
    if (bombExplosionSound) {
        Mix_FreeChunk(bombExplosionSound);
        bombExplosionSound = nullptr;
        std::cerr << "Âm thanh bom nổ đã được dọn dẹp!" << std::endl;
    }
    if (loseHealthSound) {
        Mix_FreeChunk(loseHealthSound);
        loseHealthSound = nullptr;
        std::cerr << "Âm thanh mất máu đã được dọn dẹp!" << std::endl;
    }
    if (pickupItemSound) {
        Mix_FreeChunk(pickupItemSound);
        pickupItemSound = nullptr;
        std::cerr << "Âm thanh nhận vật phẩm đã được dọn dẹp!" << std::endl;
    }

    Mix_CloseAudio();
    std::cerr << "Đóng SDL_mixer!" << std::endl;
}
