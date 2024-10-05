#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DISPLAY_HEIGHT 15
#define DISPLAY_WIDTH 25
#define FPS 10
#define BOLD "\033[1m"
#define RESET "\033[0m"

typedef int Cells[DISPLAY_HEIGHT][DISPLAY_WIDTH];

void InitCells(Cells cells, int **seed) {
  for (int i = 0; i < DISPLAY_HEIGHT; ++i) {
    for (int j = 0; j < DISPLAY_WIDTH; ++j) {
      if (seed[i][j] == 1) {
        cells[i][j] = 1;
      } else {
        cells[i][j] = 0;
      }
    }
  }
}

void CheckCells(Cells cells) {
  int neighbours = 0;
  Cells newCells = {0};
  int right = 0, left = 0, up = 0, down = 0, left_up = 0, right_up = 0,
      left_down = 0, right_down = 0;
  /*
    RULES:
    - Any live cell with fewer than two live neighbours dies, as if by
    underpopulation.
    - Any live cell with two or three live neighbours lives on to
    the next generation.
    - Any live cell with more than three live neighbours
    dies, as if by overpopulation.
    - Any dead cell with exactly three live
    neighbours becomes a live cell, as if by reproduction.
  */
  for (int i = 0; i < DISPLAY_HEIGHT; ++i) {
    for (int j = 0; j < DISPLAY_WIDTH; ++j) {
      if (i > 0)
        up = cells[i - 1][j];

      if (j > 0)
        left = cells[i][j - 1];

      if (i != (DISPLAY_HEIGHT - 1))
        down = cells[i + 1][j];

      if (j != (DISPLAY_WIDTH - 1))
        right = cells[i][j + 1];

      if (j > 0 && i > 0)
        left_up = cells[i - 1][j - 1];

      if (j != (DISPLAY_WIDTH - 1) && i > 0)
        right_up = cells[i - 1][j + 1];

      if (j > 0 && i != (DISPLAY_HEIGHT - 1))
        left_down = cells[i + 1][j - 1];

      if (j != (DISPLAY_WIDTH - 1) && i != (DISPLAY_HEIGHT - 1))
        right_down = cells[i + 1][j + 1];

      neighbours = up + down + left + right + left_up + right_up + left_down +
                   right_down;

      if (cells[i][j] == 1) {
        if (neighbours == 2 || neighbours == 3) {
          newCells[i][j] = 1;
        } else if (neighbours < 2 || neighbours > 3) {
          newCells[i][j] = 0;
        }
      } else {
        if (neighbours == 3) {
          newCells[i][j] = 1;
        }
      }
    }
  }

  for (int i = 0; i < DISPLAY_HEIGHT; ++i) {
    for (int j = 0; j < DISPLAY_WIDTH; ++j) {
      cells[i][j] = newCells[i][j];
    }
  }
}

void DrawCells(Cells cells) {
  printf("\033[2J\033[H");
  for (int i = 0; i < DISPLAY_HEIGHT; ++i) {
    printf("\033[0K%2d ", i);
    for (int j = 0; j < DISPLAY_WIDTH; ++j) {
      if (cells[i][j] == 1)
        printf(BOLD "\033[0K0" RESET);
      else
#ifdef EMPTY_LINE_DOT
        printf("\033[0K");
#else
        printf("\033[0K.");
#endif
    }
    printf("\033[0K\n");
  }
}

int main() {
  Cells cells = {0};

  int **seed = malloc(sizeof(int *) * DISPLAY_HEIGHT);

  for (int i = 0; i < DISPLAY_HEIGHT; ++i) {
    seed[i] = malloc(sizeof(int) * DISPLAY_WIDTH);
  }

  seed[3][5] = 1;
  seed[4][5] = 1;
  seed[5][5] = 1;
  seed[4][4] = 1;
  seed[5][6] = 1;
  seed[6][9] = 1;
  seed[7][9] = 1;
  seed[8][9] = 1;
  seed[7][8] = 1;
  seed[8][10] = 1;

  InitCells(cells, seed);

  for (;;) {
    DrawCells(cells);
    float dt = (1.0 / FPS) * 1000000;
    usleep((int)dt);
    CheckCells(cells);
  }

  for (int i = 0; i < DISPLAY_HEIGHT; ++i) {
    free(seed[i]);
  }

  free(seed);

  return 0;
}
