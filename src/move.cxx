//
// Created by imwhi on 11/30/2021.
//
#include "move.hxx"

// define "move" class
Move::Move(loc start, loc end, board_array_t& board)
        :start(start),
         end(end),
         piece_moved(board[start.row][start.col]),
         piece_captured(board[end.row][end.col]),
         is_enpassant(false),
         is_castle(false)
{
    is_promote = (piece_moved == "wp" && end.row == 0) ||
                 (piece_moved == "bp" && end.row == 7);
}
Move::Move(loc start, loc end, board_array_t& board, bool enpassant, bool castle)
        :start(start),
         end(end),
         piece_moved(board[start.row][start.col]),
         piece_captured(board[end.row][end.col]),
         is_enpassant(enpassant),
         is_castle(castle)
{
    is_promote = (piece_moved == "wp" && end.row == 0) ||
                 (piece_moved == "bp" && end.row == 7);

    if (is_enpassant
        && piece_moved[0] == 'w')
    {
        piece_captured = "bp";

    }else if (is_enpassant)
    {
        piece_captured = "wp";
    }
}

void
Move::print_move() const
{
    std::cout << "move: {" << " (" << start.row << ", " << start.col << ")";
    std::cout << " -> " << "(" << end.row << ", " << end.col << ")";
    std::cout << " moved: " << piece_moved << " captured: " << piece_captured;
    std::cout << " }";
}
