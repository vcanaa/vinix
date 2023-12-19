#include "game.h"
#include "../drivers/screen.h"
#include <stdint.h>
#include "../libc/string.h"

#define GRID_EMPTY 0
#define GRID_SNAKE 1
#define GRID_APPLE 2

static char gridSymbols[] = {' ', ' ', 0x04};
static char gridColors[] = {0x00, 0x20, 0x0c};

#define ROWS MAX_ROWS
#define COLS MAX_COLS

static char grid[ROWS][COLS];

// X => row
// Y => col
static int dx;
static int dy;
static int snakeX[ROWS*COLS];
static int snakeY[ROWS*COLS];
static int head;
static int tail;

static int tailExtra;

// #define TIME_MULT 1
#define TIME_MULT 1

static int ticks = 0;
static int tickRate = 1 * TIME_MULT; // 1s

static unsigned int seed = 58349;

static int apple_cd = 0;
#define APPLE_MULT 5
static int apple_cd_min = 1 * TIME_MULT * APPLE_MULT;
static int apple_cd_max = 4 * TIME_MULT * APPLE_MULT;

typedef struct {
  int length;
  int code[10];
} KeyCodeStack;

KeyCodeStack keyCodeStack;

void process_input();

void key_push(int code) {
  if (keyCodeStack.length == 10) return;
  keyCodeStack.code[keyCodeStack.length++] = code;
}

int key_pop() {
  if (keyCodeStack.length == 0) return -1;
  return keyCodeStack.code[--keyCodeStack.length];
}

int is_key_pressed() {
  return keyCodeStack.length > 0;
}

unsigned int lcg_rand(unsigned int *seed) {
    // Coefficients for the LCG
    const unsigned int a = 1664525;
    const unsigned int c = 1013904223;

    // Modulus is implicitly the maximum value of unsigned int
    *seed = (a * (*seed) + c);
    return *seed;
}

int getRandom(int min, int max) {
  return lcg_rand(&seed) % (max - min + 1) + min;
}

int getScreenIdx(int x, int y) {
  return 2*(x*COLS + y);
}

void setGrid(int symbol, int x, int y) {
  char *vidmem = (char*)VIDEO_ADDRESS;
  grid[x][y] = symbol;
  vidmem[getScreenIdx(x, y)] = gridSymbols[symbol];
  vidmem[getScreenIdx(x, y)+1] = gridColors[symbol];
}

void snake_init() {
  dx = 1;
  dy = 0;
  head = 0;
  tail = 0;

  tailExtra = 2;

  keyCodeStack.length = 0;
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      setGrid(GRID_EMPTY, i, j);
    }
  }

  for (int i = 0; i <= head; i++) {
    snakeX[i] = ROWS / 2;
    snakeY[i] = COLS / 2;
  }

  setGrid(GRID_SNAKE, snakeX[0], snakeY[0]);
}

void snake_update() {
  apple_cd--;
  if (apple_cd <= 0) {
    apple_cd = getRandom(apple_cd_min, apple_cd_max);
    int appleX = getRandom(0, ROWS-1);
    int appleY = getRandom(0, COLS-1);
    if (grid[appleX][appleY] == GRID_EMPTY) {
      setGrid(GRID_APPLE, appleX, appleY);
    }
  }

  ticks++;
  while (ticks > tickRate) {
    ticks -= tickRate;

    process_input();

    int newX = snakeX[head] + dx;
    int newY = snakeY[head] + dy;

    if (newX < 0 || newX >= ROWS || newY < 0 || newY >= COLS) {
      snake_init();
      return;
    }

    if (grid[newX][newY] == GRID_SNAKE) {
      snake_init();
      return;
    }

    if (grid[newX][newY] == GRID_APPLE) {
      tailExtra++;
    }

    if (tailExtra > 0) {
      tailExtra--;
    } else {
      setGrid(GRID_EMPTY, snakeX[tail], snakeY[tail]);
      tail = (tail + 1) % (ROWS*COLS);
    }

    setGrid(GRID_SNAKE, newX, newY);
    head = (head + 1) % (ROWS*COLS);
    snakeX[head] = newX;
    snakeY[head] = newY;
  }
}

void snake_handleInput(uint8_t code) {
  // char code_str[10];
  // int_to_ascii(code, code_str);
  // append(code_str, '\0');
  // kprint(code_str);

  key_push(code);
}

void process_input() {
  while (is_key_pressed()) {
    int code = key_pop();
    if (code == 22) {
      // u
      if (dx == 1) return;
      dx = -1;
      dy = 0;
    } else if (code == 36) {
      // j
      if (dx == -1) return;
      dx = 1;
      dy = 0;
    } else if (code == 35) {
      // h
      if (dy == 1) return;
      dx = 0;
      dy = -1;
    } else if (code == 37) {
      // k
      if (dy == -1) return;
      dx = 0;
      dy = 1;
    } else {
      continue;
    }
    break;
  }
}
