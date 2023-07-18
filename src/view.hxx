#pragma once


#include "model.hxx"


class View
{
public:
    using Dimensions = ge211::Dims<int>;
    explicit View(Model&);


    void draw(ge211::Sprite_set& set);
    Dimensions initial_window_dimensions() const;
    std::string initial_window_title() const;

    void set_mouse(ge211::Posn<int> pos);
    void set_mouse_click(ge211::Posn<int> pos);
    bool two_player;

private:




    Model& board_;
    ge211::Posn<int> mouse_loc_;
    ge211::Posn<int> mouse_click_loc_;
    Dimensions dims_;

    // sprites
    ge211::Rectangle_sprite const tile_;
    ge211::Rectangle_sprite const dark_tile_;
    ge211::Rectangle_sprite const highlight_;
    ge211::Rectangle_sprite const options_;


    ge211::Image_sprite const black_pawn;
    ge211::Image_sprite const black_knight;
    ge211::Image_sprite const black_bishop;
    ge211::Image_sprite const black_rook;
    ge211::Image_sprite const black_queen;
    ge211::Image_sprite const black_king;

    ge211::Image_sprite const white_pawn;
    ge211::Image_sprite const white_knight;
    ge211::Image_sprite const white_bishop;
    ge211::Image_sprite const white_rook;
    ge211::Image_sprite const white_queen;
    ge211::Image_sprite const white_king;
    int time_;
    ge211::Text_sprite const black_wins;
    ge211::Text_sprite const white_wins;
    ge211::Text_sprite const tie;



    ge211::Posn<int> screen_to_board(ge211::Posn<int>);
    ge211::Posn<int> board_to_screen(ge211::Posn<int>);


    std::thread aiThread_;
    Model aiBoard_;
    bool aiThreadReady_;
	



};



