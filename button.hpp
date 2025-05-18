#ifndef BUTTON_HPP_INCLUDED
#define BUTTON_HPP_INCLUDED

#include "widgets.hpp"
#include <functional>

class ButtonWidget : public Widget {
private:
    std::string _label;
    std::function<void()> _on_click;
    bool _pressed;
    bool _visible;
public:
    ButtonWidget(int x, int y, int sx, int sy, const std::string& label, std::function<void()> on_click);
    void draw() override;
    void handle(genv::event ev) override;
    void set_visible(bool visible);
    bool is_visible() const;
};
#endif // BUTTON_HPP_INCLUDED
