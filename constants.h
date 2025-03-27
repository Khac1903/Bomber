#ifndef CONSTANTS_H
#define CONSTANTS_H

// Kích thước mỗi ô
const int TILE_SIZE = 40;

// Kích thước bản đồ
const int mapCols = 20;
const int mapRows = 15;

// Kích thước cửa sổ
const int WINDOW_WIDTH =mapCols * TILE_SIZE;
const int WINDOW_HEIGHT = mapRows * TILE_SIZE;

#define CHARACTER_TEXTURE_PATH "assets/images/character.png"  // Đường dẫn ảnh nhân vật
#define BOMB_TEXTURE_PATH "assets/images/bomb.png"           // Đường dẫn ảnh bom
#define BOMB_EXPLOSION_TEXTURE_PATH "assets/images/bomb_explosion.png"      // Texture cho hiệu ứng nổ
#define BOMB_RANGE_TEXTURE_PATH "assets/images/bomb_range.png"
#define ENEMY_TEXTURE_PATH "assets/images/enemy.png"        // ảnh cho địch

#define BOMB_ItemTEXTURE_PATH "assets/images/bomb_item.png"     // ảnh vật phẩm bom
#define SPEED_TEXTURE_PATH "assets/images/speed_item.png"   // ảnh vật phẩm tăng tốc chạy
#define LIFE_ItemTEXTURE_PATH "assets/images/life_item.png"     // ảnh vật phẩm tăng mạng
       // Đường dẫn ảnh kẻ địch
#define WALL_TEXTURE_PATH "assets/images/wall.png"
#define GRASS_TEXTURE_PATH "assets/images/grass.png"
#define EMPTY_TEXTURE_PATH "assets/images/empty.png"

#define BACKGROUND_MUSIC_PATH "assets/background_music.mp3"

// Đường dẫn hiệu ứng âm thanh
#define BOMB_EXPLOSION_SOUND_PATH "assets/bomb_explosion.wav" // bom nổ
#define LOSE_HEALTH_SOUND_PATH "assets/lose_health.wav"         // mất máu
#define PICKUP_ITEM_SOUND_PATH "assets/pickup_item.wav"         // nhận vật phẩm
// ảnh cho menu
#define MENU_BACKGROUND_PATH "assets/images/menu_bg.png"
#define GAME_OVER_BACKGROUND_PATH "assets/images/gameover_bg.png"
#define HELP_BACKGROUND_PATH "assets/images/help_bg.png"
#define PAUSE_BACKGROUND_PATH "assets/images/pause_bg.png"
#define PAUSE_BUTTON_PATH "assets/images/pause_button.png"
#define PLAY_BUTTON_PATH "assets/images/play_button.png"
#define HELP_BUTTON_PATH "assets/images/help_button.png"
#define EXIT_BUTTON_PATH "assets/images/exit_button.png"
#define RESUME_BUTTON_PATH "assets/images/resume_button.png"
#define LIFE_TEXTURE_PATH "assets/images/life_icon.png"
#define VICTORY_BACKGROUND_PATH "assets/images/victory.png"




#endif

