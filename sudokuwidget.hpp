#ifndef SUDOKUWIDGET_HPP_INCLUDED
#define SUDOKUWIDGET_HPP_INCLUDED

#include "widgets.hpp"
#include "sudoku.hpp"
#include "graphics.hpp"

enum class GameState {
    IN_PROGRESS,
    COMPLETED_SUCCESS,
    COMPLETED_FAILURE
};

class SudokuWidget : public Widget {
private:
    Sudoku _mester;
    int _kivalasztott_sor, _kivalasztott_oszlop;
    int _cell_meret;
    GameState _jatek_allapot;

public:
    SudokuWidget(int x, int y, int meret);
    void draw() override;
    void handle(genv::event ev) override;
    void uj_jatek(const std::string& fajlnev = "");
    void uj_jatek(bool nehez);

    GameState ellenoriz();
    GameState get_jatek_allapot() const { return _jatek_allapot; }
};

#endif // SUDOKUWIDGET_HPP_INCLUDED
