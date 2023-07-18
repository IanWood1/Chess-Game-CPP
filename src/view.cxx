
#include "view.hxx"
#include <thread>
#include <map>

static ge211::Color const grey {140,140,140};
static ge211::Color const dark_grey {80,100,80};
static ge211::Color const red {100,10,10};
static ge211::Color const green {10,120,10};


View::View(Model& board)
    :two_player(false),
    board_(board),
    mouse_loc_(ge211::Posn<int>{-100,-100}),
    mouse_click_loc_(ge211::Posn<int>{-100,-100}),
    dims_({800,800}),
    tile_({90,90},grey),
    dark_tile_({90,90},dark_grey),
    highlight_({90,90},red),
    options_({90,90},green),

    // 60x60 chess pieces found at:
    // https://commons.wikimedia.org/wiki/Category:PNG_chess_pieces/Standard_transparent
    black_pawn("bp.png"),
    black_knight("bN.png"),
    black_bishop("bB.png"),
    black_rook("bR.png"),
    black_queen("bQ.png"),
    black_king("bK.png"),
    white_pawn("wp.png"),
    white_knight("wN.png"),
    white_bishop("wB.png"),
    white_rook("wR.png"),
    white_queen("wQ.png"),
    white_king("wK.png"),
    time_(0),
    black_wins("Black Wins", {"font.ttf",100}),
    white_wins("White Wins", {"font.ttf",100}),
    tie("Tie", {"font.ttf",100})
{

}

View::Dimensions
View::initial_window_dimensions() const
{
    return dims_;
}

std::string
View::initial_window_title() const
{
    return "♔ Chess ♔";
}

void
View::draw(ge211::Sprite_set& set)
{
    // highlight last made move
    if (!board_.move_log_.empty()) {
        Move last_move = board_.move_log_.back();

        set.add_sprite(highlight_,
                       board_to_screen(ge211::Posn<int>(last_move.end.col,
                                                        last_move.end.row)),
                                                        -1);
        set.add_sprite(highlight_,
                       board_to_screen(ge211::Posn<int>(last_move.start.col,
                                                        last_move.start.row)),
                       -1);
    }

    // add pieces
    // map didnt work so now there are 12 if statements :|
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (board_.board()[row][col] == "bp") {
                set.add_sprite(black_pawn,
                               ge211::Posn<int> {col * 100, row * 100},
                               0,
                               ge211::Transform::scale(1.5));
            }
            if (board_.board()[row][col] == "bN") {
                set.add_sprite(black_knight,
                               ge211::Posn<int> {col * 100, row * 100},
                               0,
                               ge211::Transform::scale(1.5));
            }
            if (board_.board()[row][col] == "bR") {
                set.add_sprite(black_rook,
                               ge211::Posn<int> {col * 100, row * 100},
                               0,
                               ge211::Transform::scale(1.5));
            }
            if (board_.board()[row][col] == "bB") {
                set.add_sprite(black_bishop,
                               ge211::Posn<int> {col * 100, row * 100},
                               0,
                               ge211::Transform::scale(1.5));
            }
            if (board_.board()[row][col] == "bQ") {
                set.add_sprite(black_queen,
                               ge211::Posn<int> {col * 100, row * 100},
                               0,
                               ge211::Transform::scale(1.5));
            }
            if (board_.board()[row][col] == "bK") {
                set.add_sprite(black_king,
                               ge211::Posn<int> {col * 100, row * 100},
                               0,
                               ge211::Transform::scale(1.5));
            }
            if (board_.board()[row][col] == "wp") {
                set.add_sprite(white_pawn,
                               ge211::Posn<int> {col * 100, row * 100},
                               0,
                               ge211::Transform::scale(1.5));
            }
            if (board_.board()[row][col] == "wB") {
                set.add_sprite(white_bishop,
                               ge211::Posn<int> {col * 100, row * 100},
                               0,
                               ge211::Transform::scale(1.5));
            }
            if (board_.board()[row][col] == "wN") {
                set.add_sprite(white_knight,
                               ge211::Posn<int> {col * 100, row * 100},
                               0,
                               ge211::Transform::scale(1.5));
            }
            if (board_.board()[row][col] == "wR") {
                set.add_sprite(white_rook,
                               ge211::Posn<int> {col * 100, row * 100},
                               0,
                               ge211::Transform::scale(1.5));
            }
            if (board_.board()[row][col] == "wQ") {
                set.add_sprite(white_queen,
                               ge211::Posn<int> {col * 100, row * 100},
                               0,
                               ge211::Transform::scale(1.5));
            }
            if (board_.board()[row][col] == "wK") {
                set.add_sprite(white_king,
                               ge211::Posn<int> {col * 100, row * 100},
                               0,
                               ge211::Transform::scale(1.5));
            }

        }

    }

    // draw board
    for (int x = 0; x < 800; x = x + 100){
        for (int y = 0; y < 800; y = y + 100){
            if ((x/100+y/100) % 2 == 1)
            {
                set.add_sprite(dark_tile_, ge211::Posn<int>{x+5,y+5},-2);
            }
            else
            {
                set.add_sprite(tile_, ge211::Posn<int>{x+5,y+5},-2);

            }
        }
    }

    if (board_.checkmate() || board_.stalemate())
    {
        if (board_.stalemate())
        {
            set.add_sprite(tie,
                           ge211::Posn<int>{330,325},
                           10);
        }
        else if (board_.turn() == 'w'){
            set.add_sprite(black_wins,
                           ge211::Posn<int>{170,325},
                           10);
        } else
        {
            set.add_sprite(white_wins,
                           ge211::Posn<int>{170,325},
                           10);
        }

        return;
    }




    // highlight mouse location
    set.add_sprite(options_, board_to_screen(screen_to_board(mouse_click_loc_)),
                   -1);


    // highlight possible moves
    std::vector<Move> moves = board_.get_valid_moves();
    for (auto move : moves)
    {
        if (move.start.row == screen_to_board(mouse_click_loc_).y
            && move.start.col == screen_to_board(mouse_click_loc_).x)
        {
            ge211::Posn<int> possible_move(move.end.col,move.end.row);
            set.add_sprite(options_,
                           board_to_screen(possible_move),
                           -1);
        }
    }

    // "ai" makes a move
    if (aiThreadReady_ || aiThread_.joinable()) {
        if (aiThread_.joinable())
        {
            aiThread_.join();
            board_ = aiBoard_;
            aiBoard_ = board_;
            aiThreadReady_ = true;
            board_.make_move(board_.best_move_);
        }
        else {
            // init aiBoard_
            aiBoard_ = board_;
        }

    }
	
    if (board_.turn() == 'b' && !two_player)
    {
        if (!aiThreadReady_)
            return;
        aiThread_ = std::thread([this] {aiBoard_.best_move();});
    }



}

void
View::set_mouse(ge211::Posn<int> pos)
{
    mouse_loc_ = pos;
}

ge211::Posn<int>
View::board_to_screen(ge211::Posn<int> pos)
{
    return ge211::Posn<int>(pos.x*100+5, pos.y*100+5);
}

ge211::Posn<int>
View::screen_to_board(ge211::Posn<int> pos)
{
    return ge211::Posn<int>((pos.x-5)/100, (pos.y-5)/100);
}

void
View::set_mouse_click(ge211::Posn<int> pos)
{
    std::vector<Move> moves = board_.get_valid_moves();
    for (auto move : moves) {
        if (move.start.row == screen_to_board(mouse_click_loc_).y
            && move.start.col == screen_to_board(mouse_click_loc_).x
            && move.end.col == screen_to_board(pos).x
            && move.end.row == screen_to_board(pos).y)
        {
            board_.make_move(move);
            mouse_click_loc_ = {-100,-100};
            return;
        }
    }
    if (pos == mouse_click_loc_)
    {
        mouse_click_loc_ = {-100,-100};
    }else{
        mouse_click_loc_ = pos;
    }
};