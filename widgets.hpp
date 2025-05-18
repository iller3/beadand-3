#ifndef WIDGETS_HPP_INCLUDED
#define WIDGETS_HPP_INCLUDED

#include "graphics.hpp"

class Widget {
protected:
    int _x, _y;
    int _size_x, _size_y;
public:
    Widget(int x, int y, int sx, int sy);
    virtual ~Widget() {}

    virtual void draw() = 0;
    virtual void handle(genv::event ev) = 0;

    bool is_selected(int mouse_x, int mouse_y) const;
};

#endif // WIDGETS_HPP_INCLUDED
