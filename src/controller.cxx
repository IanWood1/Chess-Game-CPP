#include "controller.hxx"



Controller::Controller()
    : board_(),
      view_(board_)
{}

void
Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites);
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

void
Controller::on_key(ge211::Key key)
{
    if (key == ge211::Key::code('q'))
    {
        quit();
    }
    if (key == ge211::Key::code('u'))
    {
        board_.undo_move();
    }
    if (key == ge211::Key::code('t'))
    {
        view_.two_player = !view_.two_player;
    }
    if (key == ge211::Key::code('r'))
    {
        board_ = Model();
    }
}

void
Controller::on_mouse_move(ge211::Posn<int> loc)
{
    view_.set_mouse(loc);
}

void
Controller::on_mouse_down(ge211::Mouse_button, ge211::Posn<int> loc)
{
    view_.set_mouse_click(loc);
}

