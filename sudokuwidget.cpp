#include "sudokuwidget.hpp"
#include "graphics.hpp"
#include <iostream>
#include <sstream>

using namespace genv;

SudokuWidget::SudokuWidget(int x, int y, int meret) :
    Widget(x, y, meret, meret),
    _kivalasztott_sor(-1), _kivalasztott_oszlop(-1),
    _cell_meret(meret / 9), _jatek_allapot(GameState::IN_PROGRESS) {}

void SudokuWidget::draw() {
    if (_jatek_allapot == GameState::COMPLETED_SUCCESS) {
        gout << color(200, 255, 200);
    } else if (_jatek_allapot == GameState::COMPLETED_FAILURE) {
        gout << color(255, 200, 200);
    } else {
        gout << color(255, 255, 255);
    }
    gout << move_to(_x, _y) << box(_size_x, _size_y);

    for (int i = 0; i <= 9; ++i) {
        if (i % 3 == 0) {
            gout << color(0, 0, 0);
            gout << move_to(_x, _y + i * _cell_meret) << box(_size_x, 3);
            gout << move_to(_x + i * _cell_meret, _y) << box(3, _size_y);
        } else {
            gout << color(200, 200, 200);
            gout << move_to(_x, _y + i * _cell_meret) << line(_size_x, 0);
            gout << move_to(_x + i * _cell_meret, _y) << line(0, _size_y);
        }
    }

    for (int sor = 0; sor < 9; ++sor) {
        for (int oszlop = 0; oszlop < 9; ++oszlop) {
            int szam = _mester.get_mezo(sor, oszlop);
            if (szam != 0) {
                int x_pos = _x + oszlop * _cell_meret + _cell_meret / 2 - 5;
                int y_pos = _y + sor * _cell_meret + _cell_meret / 2 + 5;

                if(_mester.is_fix(sor, oszlop)){
                    gout << color(0, 0, 255);
                }
                else if (_mester.is_hibas(sor, oszlop)) {
                    gout << color(255, 0, 0);
                }
                else {
                    gout << color(0, 0, 0);
                }

                std::stringstream ss;
                ss << szam;
                gout << move_to(x_pos, y_pos) << text(ss.str());
            }
        }
    }

    if (_kivalasztott_sor != -1 && _kivalasztott_oszlop != -1) {
        gout << color(255, 255, 0);
        int vastagsag = 3;
        int x1 = _x + _kivalasztott_oszlop * _cell_meret;
        int y1 = _y + _kivalasztott_sor * _cell_meret;
        gout << move_to(x1, y1) << box(_cell_meret, vastagsag);
        gout << move_to(x1, y1 + _cell_meret - vastagsag) << box(_cell_meret, vastagsag);
        gout << move_to(x1, y1) << box(vastagsag, _cell_meret);
        gout << move_to(x1 + _cell_meret - vastagsag, y1) << box(vastagsag, _cell_meret);
    }
}

void SudokuWidget::handle(genv::event ev) {
    if (_jatek_allapot != GameState::IN_PROGRESS) {
        return;
    }

    if (ev.type == ev_mouse && ev.button == btn_left) {
        if (ev.pos_x > _x && ev.pos_x < _x + _size_x &&
            ev.pos_y > _y && ev.pos_y < _y + _size_y) {
            _kivalasztott_sor = (ev.pos_y - _y) / _cell_meret;
            _kivalasztott_oszlop = (ev.pos_x - _x) / _cell_meret;
        }
        else {
            _kivalasztott_sor = -1;
            _kivalasztott_oszlop = -1;
        }
    }

    if (ev.type == ev_key && _kivalasztott_sor != -1 && _kivalasztott_oszlop != -1) {
        int szam = ev.keycode - '0';

        if (szam >= 1 && szam <= 9) {
            if (!_mester.is_fix(_kivalasztott_sor, _kivalasztott_oszlop)) {
                _mester.set_mezo(_kivalasztott_sor, _kivalasztott_oszlop, szam);
                if (!_mester.ervenyes_lepes(_kivalasztott_sor, _kivalasztott_oszlop, szam)) {
                    _mester.ellentmondast(_kivalasztott_sor, _kivalasztott_oszlop);
                }
                ellenoriz();
            }
        } else if (ev.keycode == '0' || ev.keycode == key_backspace || ev.keycode == key_delete) {
            if (!_mester.is_fix(_kivalasztott_sor, _kivalasztott_oszlop)) {
                _mester.set_mezo(_kivalasztott_sor, _kivalasztott_oszlop, 0);
                _jatek_allapot = GameState::IN_PROGRESS;
            }
        }
    }
}

void SudokuWidget::uj_jatek(const std::string& fajlnev) {
    if (fajlnev.empty()) {
        _mester.uj_jatek(false);
    } else {
        _mester.befaj(fajlnev);
    }
    _kivalasztott_sor = -1;
    _kivalasztott_oszlop = -1;
    _jatek_allapot = GameState::IN_PROGRESS;
}

void SudokuWidget::uj_jatek(bool nehez) {
    _mester.uj_jatek(nehez);
    _kivalasztott_sor = -1;
    _kivalasztott_oszlop = -1;
    _jatek_allapot = GameState::IN_PROGRESS;
}

GameState SudokuWidget::ellenoriz() {
    bool tele_van = true;

    for (int sor = 0; sor < 9; ++sor) {
        for (int oszlop = 0; oszlop < 9; ++oszlop) {
            if (_mester.get_mezo(sor, oszlop) == 0) {
                tele_van = false;
                break;
            }
        }
        if (!tele_van) break;
    }

    if (!tele_van) {
        _jatek_allapot = GameState::IN_PROGRESS;
        return _jatek_allapot;
    }

    bool van_hiba = false;

    for (int sor = 0; sor < 9; ++sor) {
        for (int oszlop = 0; oszlop < 9; ++oszlop) {
            if (_mester.is_hibas(sor, oszlop)) {
                van_hiba = true;
                break;
            }
        }
        if (van_hiba) break;
    }

    if (van_hiba) {
        _jatek_allapot = GameState::COMPLETED_FAILURE;
    } else {
        _jatek_allapot = GameState::COMPLETED_SUCCESS;
    }

    return _jatek_allapot;
}
