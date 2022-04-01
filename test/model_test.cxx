#include "model.hxx"
#include <catch.hxx>


bool move_in_vec(Move move, std::vector<Move> moves)
{
    for (auto move_in_moves : moves)
    {
        if (move_in_moves.is_enpassant == move.is_enpassant
            && move_in_moves.is_castle == move.is_castle
            && move_in_moves.is_promote == move.is_promote
            && move_in_moves.start.row == move.start.row
            && move_in_moves.start.col == move.start.col
            && move_in_moves.end.row == move.end.row
            && move_in_moves.end.col == move.end.col
            && move_in_moves.piece_captured == move.piece_captured
            && move_in_moves.piece_moved == move.piece_moved)
        {
            return true;
        }
    }
    return false;
}

// Tests that a 8x8 starting chess board is constructed and that a basic move
// can be made
TEST_CASE("test that Model() initializes a board")
{

    Model b = Model();
    CHECK(b.board() ==
    std::array<std::array<std::string,8>,8>{
    std::array<std::string,8>{"bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR", },
    std::array<std::string,8>{"bp", "bp", "bp", "bp", "bp", "bp", "bp", "bp", },
    std::array<std::string,8>{"--", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "--", "--", "--","--", "--", "--", },
    std::array<std::string,8>{"wp", "wp", "wp", "wp", "wp","wp", "wp", "wp", },
    std::array<std::string,8>{"wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR",},
    });

    // check that pawn can be moved
    Move mv = Move(loc{6,2},loc{5,2},b.board());
    CHECK(move_in_vec(mv, b.get_valid_moves()));
    b.make_move(mv);
    CHECK(b.board() ==
    std::array<std::array<std::string,8>,8>{
    std::array<std::string,8>{"bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR", },
    std::array<std::string,8>{"bp", "bp", "bp", "bp", "bp", "bp", "bp", "bp", },
    std::array<std::string,8>{"--", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "wp", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"wp", "wp", "--", "wp", "wp", "wp", "wp", "wp", },
    std::array<std::string,8>{"wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR",
                              },});

    mv = Move(loc{0,1},loc{2,2},b.board());
    CHECK(move_in_vec(mv, b.get_valid_moves()));
    b.make_move(mv);
    CHECK(b.board() ==
    std::array<std::array<std::string,8>,8>{
    std::array<std::string,8>{"bR", "--", "bB", "bQ", "bK","bB", "bN", "bR", },
    std::array<std::string,8>{"bp", "bp", "bp", "bp", "bp", "bp", "bp", "bp", },
    std::array<std::string,8>{"--", "--", "bN", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "wp", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"wp", "wp", "--", "wp", "wp", "wp", "wp", "wp", },
    std::array<std::string,8>{"wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR",
    },});

    mv = Move(loc{7,3},loc{4,0},b.board());
    CHECK(move_in_vec(mv, b.get_valid_moves()));
    b.make_move(mv);
    CHECK(b.board() ==
    std::array<std::array<std::string,8>,8>{
    std::array<std::string,8>{"bR", "--", "bB", "bQ", "bK","bB", "bN", "bR", },
    std::array<std::string,8>{"bp", "bp", "bp", "bp", "bp", "bp", "bp", "bp", },
    std::array<std::string,8>{"--", "--", "bN", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"wQ", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "wp", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"wp", "wp", "--", "wp", "wp", "wp", "wp", "wp", },
    std::array<std::string,8>{"wR", "wN", "wB", "--", "wK", "wB", "wN", "wR",
    },});

}
// test enpassant, castling, and promotion moves
TEST_CASE("Test weird moves")
{
    // test castling moves
    Model b = Model(std::array<std::array<std::string, 8>, 8> {
    std::array<std::string,8>{"bR", "--","--","--","bK","--","--","bR"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"wR","--","--","--","wK","--","--","wR"}});
    Move mv = Move(loc{7,4},loc{7,2},b.board(),false,true);
    CHECK(move_in_vec(mv, b.get_valid_moves()));
    mv = Move(loc{7,4},loc{7,6},b.board(),false,true);
    CHECK(move_in_vec(mv, b.get_valid_moves()));

    b.make_move(mv);
    CHECK(b.board() ==
    std::array<std::array<std::string,8>,8>{
    std::array<std::string,8>{"bR", "--", "--", "--", "bK", "--", "--", "bR", },
    std::array<std::string,8>{"--", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"--", "--", "--", "--", "--", "--", "--", "--", },
    std::array<std::string,8>{"wR", "--", "--", "--", "--", "wR", "wK", "--",
                              },});


    // test promotion
    b = Model(std::array<std::array<std::string, 8>, 8> {
            std::array<std::string,8>{"--", "--","--","--","bK","--","--","--"},
            std::array<std::string,8>{"--","wp","--","--","--","bp","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","wK","--","--","--"}
    });
    mv = Move(loc{1,1},loc{0,1},b.board(),false,false);
    b.make_move(mv);
    CHECK(b.board() ==
    std::array<std::array<std::string,8>,8> {
    std::array<std::string,8>{"--", "wQ","--","--","bK","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","bp","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","wK","--","--","--"}
    });

    // TODO: check enpassant
    b = Model(std::array<std::array<std::string, 8>, 8> {
            std::array<std::string,8>{"--", "--","--","--","bK","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","bp","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","wp","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","wK","--","--","--"}
    });
    mv = Move(loc{6,2},loc{4,2},b.board(),false,false);
    b.make_move(mv);
    mv = Move(loc{4,3},loc{5,2},b.board(),true,false);
    b.make_move(mv);
    CHECK(b.board() ==
          std::array<std::array<std::string,8>,8> {
          std::array<std::string,8>{"--", "--","--","--","bK","--","--","--"},
          std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
          std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
          std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
          std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
          std::array<std::string,8>{"--","--","bp","--","--","--","--","--"},
          std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
          std::array<std::string,8>{"--","--","--","--","wK","--","--","--"}
          });

}

// show that the board "knows" when there is a checkmate
TEST_CASE("Test win/lose")
{
    // check white wins
    Model b = Model(std::array<std::array<std::string, 8>, 8> {
            std::array<std::string,8>{"--", "--","--","--","bK","--","--","--"},
            std::array<std::string,8>{"--","wQ","--","--","--","--","--","wQ"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"wR","--","--","--","wK","--","--","wR"}
    });
    Move mv = Move(loc{1,1},loc{0,1},b.board());
    b.make_move(mv);
    b.get_valid_moves();
    CHECK(b.checkmate());
    // check black wins
    b = Model(std::array<std::array<std::string, 8>, 8> {
            std::array<std::string,8>{"--", "--","--","--","bK","--","--","bQ"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"bQ","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","wK","--","--","--"}
    });
    mv = Move(loc{7,4},loc{7,3},b.board());
    b.make_move(mv);
    mv = Move(loc{0,7},loc{7,7},b.board());
    b.make_move(mv);
    b.get_valid_moves();
    CHECK(b.checkmate());
}

// show that the board "knows" when there is a tie
TEST_CASE("tie")
{
    // check draw by material
    Model b = Model(std::array<std::array<std::string, 8>, 8> {
            std::array<std::string,8>{"--", "--","--","--","bK","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","wK","--","--","--"}
    });
    Move mv = Move(loc{7,4},loc{7,3},b.board());
    b.make_move(mv);
    b.get_valid_moves();
    CHECK(b.stalemate());


    // check stalemate
    b = Model(std::array<std::array<std::string, 8>, 8> {
            std::array<std::string,8>{"--", "--","--","--","bK","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","wR","--","wQ","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","wK","--","--","--"}
    });
    mv = Move(loc{7,4},loc{7,3},b.board());
    b.make_move(mv);
    b.get_valid_moves();
    CHECK(b.stalemate());
}

// show limited functionality of the ai
TEST_CASE("show that ai can find an obvious move")
{
    Model b = Model(std::array<std::array<std::string, 8>, 8> {
            std::array<std::string,8>{"bR", "--","--","--","bK","--","--","bQ"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"bQ","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","wK","--","--","--"}
    });
    Move mv = Move(loc{7,4},loc{7,3},b.board());
    b.make_move(mv);

    // best move because checkmates

    b.best_move();
    Move ai_move = b.best_move_;
    b.make_move(ai_move);
    b.get_valid_moves();
    CHECK(b.checkmate());



    b = Model(std::array<std::array<std::string, 8>, 8> {
            std::array<std::string,8>{"--", "--","--","--","bK","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
            std::array<std::string,8>{"bR","--","--","--","--","--","--","wQ"},
            std::array<std::string,8>{"--","--","--","--","wK","--","--","--"}
    });
    mv = Move(loc{7,4},loc{7,3},b.board());
    b.make_move(mv);

    // best move becasue takes queen

    b.best_move();
    ai_move = b.best_move_;
    b.make_move(ai_move);
    CHECK(b.board() ==
    std::array<std::array<std::string,8>,8> {
    std::array<std::string,8>{"--", "--","--","--","bK","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","--"},
    std::array<std::string,8>{"--","--","--","--","--","--","--","bR"},
    std::array<std::string,8>{"--","--","--","wK","--","--","--","--"}
                  });



}

