#include <vector>
#include <string>
#include <array>
#include <iostream>
struct loc
{
    int row;
    int col;

    constexpr bool eq(int otherr, int otherc) {
        return otherr == row && otherc == col;
    }

    constexpr bool on_board() {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }
};

struct dir
{
    int x = 0;
    int y = 0;
	
    constexpr bool eq(int otherx, int othery) {
        return x == otherx && y == othery;
    }
};
using board_array_t = std::array<std::array<std::string,8>,8>;


class Model;

class Move
{
public:
    Move(loc start, loc end, board_array_t& board);

    Move(loc start, loc end, board_array_t& board, bool enpassant, bool castle);

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
