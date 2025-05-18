#include "App.hpp"
#include "graphics.hpp"
#include <iostream>

using namespace genv;
using namespace std;

SudokuApplication::SudokuApplication() : eredmeny_megjelenitve(false) {
    sudoku = new SudokuWidget(50, 50, 450);

    // gombor
    ButtonWidget* ujjatekKonnyu = new ButtonWidget(550, 100, 180, 40, "Új könnyû játék", [this]() {
        sudoku->uj_jatek(false);
        this->hideStartButtons();
        eredmeny_megjelenitve = false;
    });
    buttons.push_back(ujjatekKonnyu);

    ButtonWidget* ujjatekNehez = new ButtonWidget(550, 160, 180, 40, "Új nehéz játék", [this]() {
        sudoku->uj_jatek(true);
        this->hideStartButtons();
        eredmeny_megjelenitve = false;
    });
    buttons.push_back(ujjatekNehez);

    ButtonWidget* ujJatek = new ButtonWidget(550, 220, 180, 40, "Új játék kezdése", [this]() {
        for (size_t i = 0; i < 2; i++) {
            buttons[i]->set_visible(true);
        }
        eredmeny_megjelenitve = false;
    });
    buttons.push_back(ujJatek);
}

SudokuApplication::~SudokuApplication() {
    delete sudoku;
    for (ButtonWidget* btn : buttons) {
        delete btn;
    }
}

void SudokuApplication::hideStartButtons() {
    for (size_t i = 0; i < 2; i++) {
        buttons[i]->set_visible(false);
    }
}

void SudokuApplication::eredmeny_kijelzese() {
    GameState allapot = sudoku->get_jatek_allapot();
    if (allapot != GameState::IN_PROGRESS && !eredmeny_megjelenitve) {
        eredmeny_megjelenitve = true;

        std::string uzenet;
        genv::color szoveg_szin(0, 0, 0);

        if (allapot == GameState::COMPLETED_SUCCESS) {
            uzenet = "Gratulálunk, sikeresen megoldottad a Sudokut! Jár a Bambi!";
            szoveg_szin = genv::color(0, 100, 0);
        } else {
            uzenet = "Hát ez ma nem jött be, majd legközebb se!";
            szoveg_szin = genv::color(150, 0, 0);
        }

        int szelesseg = gout.twidth(uzenet);
        gout << szoveg_szin;
        gout << move_to(400 - szelesseg/2, 20) << text(uzenet);
        gout << refresh;
    }
}

void SudokuApplication::run() {
    gout.open(800, 550);
    event ev;

    while(gin >> ev && ev.keycode != key_escape) {
        gout << move_to(0, 0) << color(220, 220, 220) << box(800, 550);
        sudoku->handle(ev);
        for (ButtonWidget* btn : buttons) {
            btn->handle(ev);
        }
        sudoku->draw();
        for (ButtonWidget* btn : buttons) {
            btn->draw();
        }
        eredmeny_kijelzese();

        gout << refresh;
    }
}
