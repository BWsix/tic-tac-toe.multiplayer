#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H

#include "dbg.h"
#include <stdbool.h>

typedef enum Cell {
  Cell_Empty = ' ',
  Cell_O = 'O',
  Cell_X = 'X',
} Cell;

bool Wins(Cell cells[9], Cell who);
bool IsBoardFull(Cell cells[9]);

#endif // !TIC_TAC_TOE_H

#ifdef TIC_TAC_TOE_IMPLEMENTATION
static int lines[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6},
    {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6},
};

bool Wins(Cell cells[9], Cell who) {
  for (int i = 0; i < 8; i++) {
    Cell cell1 = cells[lines[i][0]];
    Cell cell2 = cells[lines[i][1]];
    Cell cell3 = cells[lines[i][2]];
    if (cell1 == who && cell2 == who && cell3 == who) {
      return true;
    }
  }
  return false;
}

bool Loses(Cell cells[9], Cell who) {
  if (who == Cell_O) {
    who = Cell_X;
  } else {
    who = Cell_O;
  }

  dbg("opponent: %c", who);

  return Wins(cells, who);
}

bool IsBoardFull(Cell cells[9]) {
  for (int i = 0; i < 9; i++) {
    if (cells[i] == Cell_Empty) {
      return false;
    }
  }
  return true;
}

#endif // !TIC_TAC_TOE_IMPLEMENTATION
