#ifndef BOARD_VIEW_H
#define BOARD_VIEW_H

class BoardModel;


class BoardView {
public:
  void showDefined(BoardModel const& model) const;
  void showDecided(BoardModel const& model) const;
};

#endif // BOARD_VIEW_H