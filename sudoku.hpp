#ifndef SUDOKU_HPP_INCLUDED
#define SUDOKU_HPP_INCLUDED

#include <vector>
#include <string>
#include <random>
#include <ctime>

class Sudoku {
private:
    std::vector<std::vector<int>> _tabla;
    std::vector<std::vector<bool>> _hibak;
    std::vector<std::vector<bool>> _fix_szamok;

public:
    Sudoku();
    void befaj(const std::string& fajlnev);
    void uj_jatek(bool nehez = false);
    bool ervenyes_lepes(int sor, int oszlop, int szam);
    void ellentmondast(int sor, int oszlop);
    int get_mezo(int sor, int oszlop) const;
    bool megoldva() const;
    void set_mezo(int sor, int oszlop, int szam);
    bool is_fix(int sor, int oszlop) const;
    bool is_hibas(int sor, int oszlop) const;

private:
    bool solve_sudoku();
    bool is_valid(int sor, int oszlop, int szam) const;
    void remove_numbers(int cells_to_remove);
    void fill_diagonal_boxes();
    void fill_box(int row, int col);
};

#endif // SUDOKU_HPP_INCLUDED
