
#ifndef COLLISION_H
#define COLLISION_H

#include "Character.h"
#include "boom.h"
#include <vector>

// Kiểm tra va chạm với kẻ địch
bool checkCollision(const Character& player, const std::vector<Character>& enemies);

// Kiểm tra va chạm với bom nổ
bool checkBombCollision(const Character& player, const std::vector<Bomb>& bombs);

#endif
