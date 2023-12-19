#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <stdint.h>
void snake_init();
void snake_update();
void snake_handleInput(uint8_t code);
#endif