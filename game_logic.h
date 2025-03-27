
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <vector>
#include "constants.h"
#include "Character.h"
#include "enemy.h"
#include "item.h"
#include "boom.h"
// Kiểm tra điều kiện chiến thắng
void checkVictoryCondition(SDL_Renderer* renderer,
                           std::vector<Enemy>& enemies,
                           bool& isRunning);
// Xử lý va chạm với các đối tượng khác
void handleCollision(Character& player);
// va chạm bom với nhân vật

void handlePlayerHitByBomb(Character& player, const Bomb& bomb, Uint32 currentTime, bool& isRunning, SDL_Renderer* renderer);
#endif // GAME_LOGIC_H
