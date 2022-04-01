#include <vector>
#include <string>
#include <array>
#include <iostream>
struct loc
{
    int row;
    int col;

    loc add(int r, int c)
    {
        return loc {row + r, col + c};
    }

    bool eql(loc other)
    {
        return row == other.row && col == other.col;
    }

};
using board_array_t = std::array<std::array<std::string,8>,8>;


class Model;

class Move
{
public:
    Move(loc start, loc end, board_array_t board);

    Move(loc start, loc end, board_array_t board, bool enpassant, bool castle);

    loc start;
    loc end;
    std::string piece_moved;
    std::string piece_captured;
    bool is_enpassant;
    bool is_castle;
    bool is_promote;
    // TODO: is castle move & promotion choice & enpassant

    void print_move() const;
};
