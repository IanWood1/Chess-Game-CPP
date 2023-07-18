#pragma once

#include <ge211.hxx>
#include <vector>
#include <string>
#include <iostream>
#include <optional>
#include "move.hxx"
using board_array_t = std::array<std::array<std::string,8>,8>;

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

    Model(std::array<std::array<std::string, 8>, 8>);

    void print_board() const;
    void print_board_array() const;

    std::array<std::array<std::string,8>,8> board_;
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

    using board_array_t = std::array<std::array<std::string,8>,8>;

    int count_ = 0;

    /*
    board_array_t board_ = board_array_t {
            std::array<std::string,8>{"bR", "bN","bB","bQ","bK","bB","bN","bR"},
            std::array<std::string,8>{"bp","bp","bp","bp","bp","bp","bp","bp"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--",},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--",},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--",},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--",},
            std::array<std::string,8>{"wp","wp","wp","wp","wp","wp","wp","wp",},
            std::array<std::string,8>{"wR", "wN","wB","wQ","wK","wB","wN","wR"}};
    */
    /*
    board_array_t board_ = board_array_t {
            std::array<std::string,8>{"bR", "bN","bB","bQ","bK","bB","bN","bR"},
            std::array<std::string,8>{"bp","bp","bp","bp","bp","bp","bp","bp"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--",},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--",},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--",},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--",},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--",},
            std::array<std::string,8>{"wR", "wN","wB","wQ","wK","wB","wN","wR"}};
    */
    /*
    board_array_t board_ = board_array_t {
            std::array<std::string,8>{"--", "--","--","--","bK","--","--","--"},
            std::array<std::string,8>{"bp","--","--","--","bR","bR","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","bB","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"bQ","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"wp","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","wK","--","--","--"}};
    */
/*
    board_array_t board_ = board_array_t {
            std::array<std::string,8>{"bR", "--","--","--","bK","--","--","bR"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"wR","--","--","--","wK","--","--","wR"}};
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
