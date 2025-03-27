

#ifndef MAP_H
#define MAP_H

#include <vector>
#include <SDL.h>
#include "constants.h" // Để sử dụng kích thước ô và bản đồ

// Định nghĩa các loại ô trên bản đồ
enum TileType { EMPTY, WALL, GRASS };

// Khai báo các biến texture cho bản đồ
extern SDL_Texture* wallTexture;
extern SDL_Texture* grassTexture;
extern SDL_Texture* emptyTexture;

// Khai báo các hàm xử lý bản đồ
bool loadTextures(SDL_Renderer* renderer); // Tải các texture bản đồ
/**
 * @brief Tạo bản đồ với các ô kết nối hợp lý (tường, bụi cỏ, ô trống).
 *
 * @return std::vector<std::vector<TileType>> Bản đồ được tạo ra.
 */
std::vector<std::vector<TileType>> createMap();
/**
 * @brief Vẽ bản đồ trên màn hình.
 *
 * @param renderer Renderer của SDL để hiển thị.
 * @param map Bản đồ chứa các ô (tường, bụi cỏ, ô trống).
 */
void renderMap(SDL_Renderer* renderer, const std::vector<std::vector<TileType>>& map);

/**
 * @brief Tải ảnh texture cho bản đồ.
 *
 * @param renderer Renderer SDL để tải ảnh.
 * @return true nếu tải thành công, false nếu thất bại.
 */
bool loadMapTextures(SDL_Renderer* renderer);

/**
 * @brief Giải phóng bộ nhớ của texture bản đồ.
 */
void freeTextures();

#endif
