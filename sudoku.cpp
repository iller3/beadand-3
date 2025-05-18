#include "sudoku.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>

Sudoku::Sudoku() : _tabla(9, std::vector<int>(9, 0)),
                   _hibak(9, std::vector<bool>(9, false)),
                   _fix_szamok(9, std::vector<bool>(9, false)) {}

void Sudoku::befaj(const std::string& fajlnev) {
    std::ifstream fajl(fajlnev);
    if (!fajl.is_open()) {
        throw std::runtime_error("Nem sikerült megnyitni a fájlt: " + fajlnev);
    }

    for (int sor = 0; sor < 9; ++sor) {
        for (int oszlop = 0; oszlop < 9; ++oszlop) {
            int szam;
            if (!(fajl >> szam)) {
                throw std::runtime_error("Hibás fájlformátum: " + fajlnev);
            }
            _tabla[sor][oszlop] = szam;
            _fix_szamok[sor][oszlop] = (szam != 0);
            _hibak[sor][oszlop] = false;
        }
    }
    fajl.close();
}

void Sudoku::uj_jatek(bool nehez) {
    // Tábla törlése
    for (auto& sor : _tabla) {
        std::fill(sor.begin(), sor.end(), 0);
    }
    for (auto& sor : _hibak) {
        std::fill(sor.begin(), sor.end(), false);
    }
    for (auto& sor : _fix_szamok) {
        std::fill(sor.begin(), sor.end(), false);
    }

    // Átlós blokkok feltöltése
    fill_diagonal_boxes();

    // Megoldás keresése
    solve_sudoku();

    // Számok eltávolítása
    int cells_to_remove = nehez ? 55 : 40;
    remove_numbers(cells_to_remove);

    // Fix számok beállítása
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            _fix_szamok[i][j] = (_tabla[i][j] != 0);
        }
    }
}

void Sudoku::fill_diagonal_boxes() {
    for (int i = 0; i < 9; i += 3) {
        fill_box(i, i);
    }
}

void Sudoku::fill_box(int row, int col) {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::shuffle(numbers.begin(), numbers.end(), std::default_random_engine(std::time(0)));

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            _tabla[row+i][col+j] = numbers[i*3 + j];
        }
    }
}

bool Sudoku::solve_sudoku() {
    for (int sor = 0; sor < 9; sor++) {
        for (int oszlop = 0; oszlop < 9; oszlop++) {
            if (_tabla[sor][oszlop] == 0) {
                for (int szam = 1; szam <= 9; szam++) {
                    if (is_valid(sor, oszlop, szam)) {
                        _tabla[sor][oszlop] = szam;

                        if (solve_sudoku()) {
                            return true;
                        }

                        _tabla[sor][oszlop] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::is_valid(int sor, int oszlop, int szam) const {
    //sor check
    for (int i = 0; i < 9; i++) {
        if (_tabla[sor][i] == szam) return false;
    }

    //oszlop check
    for (int i = 0; i < 9; i++) {
        if (_tabla[i][oszlop] == szam) return false;
    }

    // 3x3-as blokk check
    int blokk_sor = sor - sor % 3;
    int blokk_oszlop = oszlop - oszlop % 3;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (_tabla[blokk_sor + i][blokk_oszlop + j] == szam) return false;
        }
    }

    return true;
}

void Sudoku::remove_numbers(int cells_to_remove) {
    std::vector<std::pair<int, int>> positions;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            positions.emplace_back(i, j);
        }
    }

    std::shuffle(positions.begin(), positions.end(), std::default_random_engine(std::time(0)));

    for (size_t i = 0; i < static_cast<size_t>(cells_to_remove) && i < positions.size(); i++) {
        _tabla[positions[i].first][positions[i].second] = 0;
    }
}

bool Sudoku::ervenyes_lepes(int sor, int oszlop, int szam) {
    if (szam < 1 || szam > 9) {
        return false;
    }

    //sor check
    for (int i = 0; i < 9; ++i) {
        if (i != oszlop && _tabla[sor][i] == szam) {
            return false;
        }
    }

    //oszlop check
    for (int i = 0; i < 9; ++i) {
        if (i != sor && _tabla[i][oszlop] == szam) {
            return false;
        }
    }

    // 3x3-as blokk check
    int blokk_sor = sor / 3;
    int blokk_oszlop = oszlop / 3;
    for (int i = blokk_sor * 3; i < blokk_sor * 3 + 3; ++i) {
        for (int j = blokk_oszlop * 3; j < blokk_oszlop * 3 + 3; ++j) {
            if ((i != sor || j != oszlop) && _tabla[i][j] == szam) {
                return false;
            }
        }
    }

    return true;
}

void Sudoku::ellentmondast(int sor, int oszlop) {
    _hibak[sor][oszlop] = true;
}

int Sudoku::get_mezo(int sor, int oszlop) const {
    return _tabla[sor][oszlop];
}

bool Sudoku::megoldva() const {
    for (int sor = 0; sor < 9; ++sor) {
        for (int oszlop = 0; oszlop < 9; ++oszlop) {
            if (_tabla[sor][oszlop] == 0 || _hibak[sor][oszlop]) {
                return false;
            }
        }
    }
    return true;
}

void Sudoku::set_mezo(int sor, int oszlop, int szam) {
    _tabla[sor][oszlop] = szam;
    _hibak[sor][oszlop] = false;
}

bool Sudoku::is_fix(int sor, int oszlop) const {
    return _fix_szamok[sor][oszlop];
}

bool Sudoku::is_hibas(int sor, int oszlop) const {
    return _hibak[sor][oszlop];
}
