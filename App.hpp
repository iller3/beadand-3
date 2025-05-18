#ifndef APP_HPP_INCLUDED
#define APP_HPP_INCLUDED
#include "sudokuwidget.hpp"
#include "button.hpp"
#include <vector>

class SudokuApplication {
private:
    SudokuWidget* sudoku;
    std::vector<ButtonWidget*> buttons;
    bool eredmeny_megjelenitve;

public:
    SudokuApplication();
    ~SudokuApplication();

    void hideStartButtons();
    void run();
    void eredmeny_kijelzese();
};



#endif // APP_HPP_INCLUDED
