
#include "board_view.h"
#include "board_model.h"

#include <vector>
#include <strstream>
#include <ncurses.h>


void showUpperBorderSection () {
  addch(ACS_ULCORNER);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_TTEE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_TTEE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_URCORNER);
}

void showUpperBorder3Sections () {
  showUpperBorderSection();
  showUpperBorderSection();
  showUpperBorderSection();
}

void showFieldsSection () {
  addch(ACS_VLINE);
  addch(' ');
  addch(' ');
  addch(' ');
  addch(ACS_VLINE);
  addch(' ');
  addch(' ');
  addch(' ');
  addch(ACS_VLINE);
  addch(' ');
  addch(' ');
  addch(' ');
  addch(ACS_VLINE);
}

void showFields3Sections () {
  showFieldsSection();
  showFieldsSection();
  showFieldsSection();
}

void showInnerBorderSection () {
  addch(ACS_LTEE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_PLUS);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_PLUS);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_RTEE);
}

void showInnerBorder3Sections () {
  showInnerBorderSection();
  showInnerBorderSection();
  showInnerBorderSection();
}

void showLowerBorderSection () {
  addch(ACS_LLCORNER);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_BTEE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_BTEE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_HLINE);
  addch(ACS_LRCORNER);
}

void showLowerBorder3Sections () {
  showLowerBorderSection();
  showLowerBorderSection();
  showLowerBorderSection();
}

void show3Sections () {
  showUpperBorder3Sections();
  addch('\n');

  showFields3Sections();
  addch('\n');
  showInnerBorder3Sections();
  addch('\n');
  showFields3Sections();
  addch('\n');
  showInnerBorder3Sections();
  addch('\n');
  showFields3Sections();
  addch('\n');

  showLowerBorder3Sections();
  addch('\n');
}

void showGrid () {
  show3Sections();
  show3Sections();
  show3Sections();
}

void showNumber(int r, int c, int n) {
   mvaddch(1 + r*2 + r/3, 2 + c*4 + c/3, '0' + n);
}

void showDefinitions(BoardModel const& board) {
  attron(COLOR_PAIR(COLOR_YELLOW));
  BoardModel::forEachInBoard([&] (uint32_t r, uint32_t c) {
    int n = board.getCell(r, c).getDefinition();
    if (n > 0)
      showNumber(r, c, n);
  });
  attroff(COLOR_PAIR(COLOR_YELLOW));
}

void showDecisions(BoardModel const& board) {
  BoardModel::forEachInBoard([&] (uint32_t r, uint32_t c) {
    int n = board.getCell(r, c).getDecision();
    if (n > 0)
      showNumber(r, c, n);
  });
}

void showStatistics(BoardModel const& board) {
  uint32_t numUndecided = 0;
  uint32_t numDecided = 0;
  uint32_t numDefined = 0;

  BoardModel::forEachInBoard([&] (uint32_t r, uint32_t c) {
    CellModel cell = board.getCell(r, c);
    if (cell.isUndecided())
      ++numUndecided;
    if (cell.isDecided())
      ++numDecided;
    if (cell.isDefined())
      ++numDefined;
  });

  std::strstream ss;
  ss << "Undesided : " << numUndecided << "\n";
  ss << "Desided   : " << numDecided << "\n";
  ss << "Defined   : " << numDefined << "\n";

  mvprintw(22, 0, ss.str());
}

void BoardView::showDefined(BoardModel const& board) const {
  showGrid();
  showDefinitions(board);
  showStatistics(board);
}

void BoardView::showDecided(BoardModel const& board) const {
  showGrid();
  showDecisions(board);
  showDefinitions(board);
  showStatistics(board);
}
