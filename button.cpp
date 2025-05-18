#include "button.hpp"
#include "graphics.hpp"

using namespace genv;

ButtonWidget::ButtonWidget(int x, int y, int sx, int sy, const std::string& label, std::function<void()> on_click)
    : Widget(x, y, sx, sy), _label(label), _on_click(on_click), _pressed(false), _visible(true) {}

void ButtonWidget::draw() {
    if (!_visible) return;

    //háttér
    gout << (_pressed ? color(150, 150, 150) : color(200, 200, 200));
    gout << move_to(_x, _y) << box(_size_x, _size_y);

    //keret
    gout << color(0, 0, 0);
    gout << move_to(_x, _y) << line(_size_x, 0) << line(0, _size_y)
         << line(-_size_x, 0) << line(0, -_size_y);

    //szöveg
    gout << color(0, 0, 0);
    gout << move_to(_x + _size_x/2 - gout.twidth(_label)/2,
                   _y + _size_y/2 + gout.cascent()/2)
         << text(_label);
}

void ButtonWidget::handle(genv::event ev) {
    if (!_visible) return;

    if (ev.type == ev_mouse) {
        bool selected = is_selected(ev.pos_x, ev.pos_y);

        if (ev.button == btn_left) {
            _pressed = selected;
        } else if (ev.button == -btn_left && _pressed && selected) {
            _on_click();
            _pressed = false;
        }
    }
}

void ButtonWidget::set_visible(bool visible) {
    _visible = visible;
}

bool ButtonWidget::is_visible() const {
    return _visible;
}
