
#ifndef MUSIC_H
#define MUSIC_H

#include <SDL_mixer.h> // SDL_mixer để xử lý âm thanh

// Khởi tạo âm thanh
void initializeMusic();

// Phát nhạc nền
void playBackgroundMusic(const char* musicFile);

// Dừng nhạc nền
void stopBackgroundMusic();

// Phát hiệu ứng âm thanh (bom nổ, mất máu, nhận vật phẩm)
void playBombExplosion();
void playLoseHealth();
void playPickupItem();

// Dọn dẹp tài nguyên âm thanh
void cleanupMusic();

#endif // MUSIC_H
