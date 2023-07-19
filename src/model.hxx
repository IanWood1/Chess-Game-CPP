#pragma once

#include <ge211.hxx>
#include <vector>
#include <string>
#include <iostream>
#include <optional>
#include "move.hxx"


using board_elem_t = std::string;
using board_row_t = std::array<board_elem_t, 8>;
using board_array_t = std::array<board_row_t, 8>;


// structs for important parts about the game

struct castling {
    bool wk;
    bool wq;
    bool bk;
    bool bq;
};

// note: model contains both the model and the "board"
class Model
{
public:

    Model();

    Model(board_array_t);

    void print_board() const;
    void print_board_array() const;

    board_array_t board_;
    int score_the_board() const;
    int piece_score(char) const;
    void best_move();


    char turn() const
    {return turn_;}

    bool checkmate() const
    {return checkmate_;}
    bool stalemate() const
    {return stalemate_;}

    char winner() const
    {return winner_;}

    board_array_t& board() { return board_; }
    const board_array_t& board() const { return board_; }


    void make_move(const Move& mv);

    std::vector<Move> get_valid_moves();



    void undo_move();


    int count_ = 0;

    /*
    board_array_t board_ = board_array_t {
            board_row_t{"bR", "bN","bB","bQ","bK","bB","bN","bR"},
            board_row_t{"bp","bp","bp","bp","bp","bp","bp","bp"},
            board_row_t{"--","--","--","--","--","--","--","--",},
            board_row_t{"--","--","--","--","--","--","--","--",},
            board_row_t{"--","--","--","--","--","--","--","--",},
            board_row_t{"--","--","--","--","--","--","--","--",},
            board_row_t{"wp","wp","wp","wp","wp","wp","wp","wp",},
            board_row_t{"wR", "wN","wB","wQ","wK","wB","wN","wR"}};
    */
    /*
    board_array_t board_ = board_array_t {
            board_row_t{"bR", "bN","bB","bQ","bK","bB","bN","bR"},
            board_row_t{"bp","bp","bp","bp","bp","bp","bp","bp"},
            board_row_t{"--","--","--","--","--","--","--","--",},
            board_row_t{"--","--","--","--","--","--","--","--",},
            board_row_t{"--","--","--","--","--","--","--","--",},
            board_row_t{"--","--","--","--","--","--","--","--",},
            board_row_t{"--","--","--","--","--","--","--","--",},
            board_row_t{"wR", "wN","wB","wQ","wK","wB","wN","wR"}};
    */
    /*
    board_array_t board_ = board_array_t {
            board_row_t{"--", "--","--","--","bK","--","--","--"},
            board_row_t{"bp","--","--","--","bR","bR","--","--"},
            board_row_t{"--","--","--","--","--","--","bB","--"},
            board_row_t{"--","--","--","--","--","--","--","--"},
            board_row_t{"bQ","--","--","--","--","--","--","--"},
            board_row_t{"--","--","--","--","--","--","--","--"},
            board_row_t{"wp","--","--","--","--","--","--","--"},
            board_row_t{"--","--","--","--","wK","--","--","--"}};
    */
/*
    board_array_t board_ = board_array_t {
            board_row_t{"bR", "--","--","--","bK","--","--","bR"},
            board_row_t{"--","--","--","--","--","--","--","--"},
            board_row_t{"--","--","--","--","--","--","--","--"},
            board_row_t{"--","--","--","--","--","--","--","--"},
            board_row_t{"--","--","--","--","--","--","--","--"},
            board_row_t{"--","--","--","--","--","--","--","--"},
            board_row_t{"--","--","--","--","--","--","--","--"},
            board_row_t{"wR","--","--","--","wK","--","--","wR"}};
*/
     Move best_move_ = Move(loc{},loc{},board_);
    std::vector<Move> move_log_ = std::vector<Move>{};
private:

    // private methods




    std::vector<Move> get_all_moves();

    std::pair<std::vector<std::pair<loc, dir>>, std::vector<std::pair<loc, dir>>>
        get_pins_and_checks();



    bool in_check();

    bool square_attacked(loc square);



    void get_castling_moves(int Krow, int Kcol, std::vector<Move>& moves);


    // private methods for moved pieces

    void pawn_moves(int row, int col, std::vector<Move>& moves);

    void knight_moves(int row, int col, std::vector<Move>& moves);

    void bishop_moves(int row, int col, std::vector<Move>& moves);

    void rook_moves(int row, int col, std::vector<Move>& moves);

    void queen_moves(int row, int col, std::vector<Move>& moves);

    void king_moves(int row, int col, std::vector<Move>& moves);



    // private data members



    loc white_king_ = loc{7,4};
    loc black_king_ = loc{0,4};
    castling castling_rights_ = castling{true,true,true,true};
    std::vector<castling> castling_log_ =
            std::vector<castling>{castling{true,true,true,true}};
    loc enpassant_ = loc{-1,-1};
    bool checkmate_ = false;
    bool stalemate_ = false;
    bool in_check_ = false;
    std::vector<std::pair<loc, dir>> pins_;
    std::vector<std::pair<loc, dir>> checks_;
    char turn_ = 'w';
    char winner_ = 0;
    int depth_ = 0;

    int
    best_move_helper(
            const std::vector<Move>& valid_moves,
            int depth,
            int alpha,
            int beta,
            int turn_multi);
};
