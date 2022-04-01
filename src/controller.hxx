#pragma once

#include "model.hxx"
#include "view.hxx"


class Controller : public ge211::Abstract_game
{
public:
    Controller();

protected:
    void draw(ge211::Sprite_set&) override;
    View::Dimensions initial_window_dimensions() const override;
    std::string initial_window_title() const override;

    void on_mouse_move(ge211::Posn<int>) override;
    void on_mouse_down(ge211::Mouse_button, ge211::Posn<int>) override;
    void on_key(ge211::Key) override;


private:
    Model board_;
    View view_;



};