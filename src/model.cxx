#include "model.hxx"

using board_array_t = std::array<std::array<std::string,8>,8>;



Model::Model()
{
    board_ = board_array_t {
            std::array<std::string,8>{"bR", "bN","bB","bQ","bK","bB","bN","bR"},
            std::array<std::string,8>{"bp","bp","bp","bp","bp","bp","bp","bp"},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--",},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--",},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--",},
            std::array<std::string,8>{"--","--","--","--","--","--","--","--",},
            std::array<std::string,8>{"wp","wp","wp","wp","wp","wp","wp","wp",},
            std::array<std::string,8>{"wR", "wN","wB","wQ","wK","wB","wN","wR"}};
}

Model::Model(std::array<std::array<std::string, 8>, 8> input_board)
{
    board_ = input_board;
}

void
Model::print_board() const
{
    for (auto& row : board_)
    {
        for (auto& piece : row)
        {
            std::cout << piece << " ";
        }
        std::cout << "\n";
    }
}

void
Model::print_board_array() const
{
    std::cout << "std::array<std::array<std::string,8>,8>{\n";
    for (auto& row : board_)
    {
        std::cout << "std::array<std::string,8>{";
        for (auto& piece : row)
        {
            std::cout << "\"" << piece << "\", ";
        }
        std::cout << "},\n";
    }
    std::cout << "}";
}

void
Model::make_move(const Move& move)
{
    board_[move.start.row][move.start.col] = "--";
    board_[move.end.row][move.end.col] = move.piece_moved;
    move_log_.push_back(move);
    if (turn_ == 'w'){
        turn_ = 'b';
    }else{
        turn_ = 'w';
    }

    if (move.piece_moved == "wK")
    {
        white_king_ = loc{move.end};
    }else if (move.piece_moved == "bK")
    {
        black_king_ = loc{move.end};
    }

    // TODO: add promotion
    if (move.is_promote)
    {
        if (turn_ == 'w'){
            board_[move.end.row][move.end.col] = "bQ";
        }else{
            board_[move.end.row][move.end.col] = "wQ";
        }
    }

    // TODO: add enpassant
    if (move.is_enpassant)
    {
        board_[move.start.row][move.end.col] = "--";
    }
    if (move.piece_moved[1] == 'p'
        && std::abs(move.start.row - move.end.row) == 2)
    {
        enpassant_ = loc{(move.start.row + move.end.row)/2, move.end.col};
    }
    else
    {
        enpassant_ = loc{-1,-1};
    }

    // TODO: add castling
    if (move.is_castle)
    {
        if (move.end.col - move.start.col == 2)
        {
            board_[move.end.row][move.end.col-1] =
                    board_[move.end.row][move.end.col+1];
            board_[move.end.row][move.end.col+1] = "--";
        }
        else
        {
            board_[move.end.row][move.end.col+1] =
                    board_[move.end.row][move.end.col-2];
            board_[move.end.row][move.end.col-2] = "--";
        }
    }
    // update castling rights
    if (move.piece_moved == "wK")
    {
        castling_rights_.wk = false;
        castling_rights_.wq = false;
    }
    else if (move.piece_moved == "bK")
    {
        castling_rights_.bk = false;
        castling_rights_.bq = false;
    }
    else if (move.piece_moved == "wR")
    {
        if (move.start.row == 7)
        {
            if (move.start.col == 0)
            {
                castling_rights_.wq = false;
            }
            else if (move.start.col == 7)
            {
                castling_rights_.wk = false;
            }
        }
    }
    else if (move.piece_moved == "bR")
    {
        if (move.start.row == 0)
        {
            if (move.start.col == 0)
            {
                castling_rights_.bq = false;
            }
            else if (move.start.col == 7)
            {
                castling_rights_.bk = false;
            }
        }
    }
    castling_log_.push_back(castling(castling_rights_));

}

void
Model::undo_move()
{
    if (move_log_.size() != 0)
    {
        Move last_move = Move(*(move_log_.end() - 1));
        board_[last_move.end.row][last_move.end.col] = last_move.piece_captured;
        board_[last_move.start.row][last_move.start.col] = last_move.piece_moved;

        if (turn_ == 'w'){
            turn_ = 'b';
        }else{
            turn_ = 'w';
        }

        if (last_move.piece_moved == "wK")
        {
            white_king_ = last_move.start;
        }else if (last_move.piece_moved == "bK")
        {
            black_king_ = last_move.start;
        }


        if (last_move.piece_moved[1] == 'p'
            && 2 == std::abs(last_move.start.row - last_move.end.row))
        {
            enpassant_ = loc{-1,-1};
        }

        // TODO: enpassant moves
        if (last_move.is_enpassant)
        {
            board_[last_move.end.row][last_move.end.col] = "--";
            board_[last_move.start.row][last_move.end.col] = last_move
                    .piece_captured;
            enpassant_ = last_move.end;
        }

        // TODO: castle moves
        if (last_move.is_castle)
        {
            if (last_move.end.col - last_move.start.col == 2)
            {
                board_[last_move.end.row][last_move.end.col+1] =
                        board_[last_move.end.row][last_move.end.col-1];
                board_[last_move.end.row][last_move.end.col-1] = "--";
            }
            else
            {
                board_[last_move.end.row][last_move.end.col-2] =
                        board_[last_move.end.row][last_move.end.col+1];
                board_[last_move.end.row][last_move.end.col+1] = "--";
            }
        }
        castling_log_.pop_back();
        castling_rights_ = castling(castling_log_.back());
        move_log_.pop_back();
        checkmate_ = false;
        stalemate_ = false;
    }


}

std::vector<Move>
Model::get_valid_moves()
{
    loc save_enpassant = enpassant_;
    castling save_castling = castling(castling_rights_);
    std::vector<Move> moves = get_all_moves();
    // TODO: implement castling here
    if(turn_ == 'w')
    {
        get_castling_moves(white_king_.row,white_king_.col,moves);
    }
    else
    {
        get_castling_moves(black_king_.row,black_king_.col,moves);

    }
    if (moves.empty())
    {
        return moves;
    }
    for (std::vector<Move>::iterator i = moves.begin();
        i != moves.end();)
    {
        make_move(*i);
        if (turn_ == 'w'){
            turn_ = 'b';
        }else{
            turn_ = 'w';
        }

        if (in_check())
        {
            i = moves.erase(i);
        }else {
            i++;
        }

        if (turn_ == 'w'){
            turn_ = 'b';
        }else{
            turn_ = 'w';
        }
        undo_move();

        if (moves.size() == 0)
        {
            if (in_check())
            {
                checkmate_ = true;
            }
            else
            {
                stalemate_ = true;
            }
        }
        else
        {
            checkmate_ = false;
            stalemate_ = true;
            for (auto& row : board_)
            {
                for (auto& piece : row)
                {
                    if (piece[1] != '-'
                        && piece[1] != 'K')
                    {
                        stalemate_ = false;
                    }
                }
            }
        }

    }



    enpassant_ = save_enpassant;
    castling_rights_ = save_castling;
    return moves;
}

std::vector<Move>
Model::get_all_moves()
{
    std::vector<Move> moves;

    // branching factor of 35 seems reasonable
    // https://chess.stackexchange.com/questions/23135/what-is-the-average-number-of-legal-moves-per-turn
    moves.reserve(35);

    for (int row = 0; row < 8; row ++)
    {
        for (int col = 0; col < 8; col ++)
        {
            char piece_color = board_[row][col][0];
            if (piece_color == turn_)
            {
                char piece = board_[row][col][1];
                if (piece == 'p')
                {
                    pawn_moves(row,col,moves);
                }
                else if (piece == 'B')
                {
                    bishop_moves(row,col,moves);
                }
                else if (piece == 'N')
                {
                    knight_moves(row,col,moves);
                }
                else if (piece == 'R')
                {
                    rook_moves(row,col,moves);
                }
                else if (piece == 'Q')
                {
                    queen_moves(row,col,moves);
                }
                else if (piece == 'K')
                {
                    king_moves(row,col,moves);
                }
            }
        }
    }
    return moves;
}

bool
Model::square_attacked(loc square)
{
    if (turn_ == 'w')
    {
        turn_ = 'b';
    } else
    {
        turn_ = 'w';
    }

    std::vector<Move> opponent_moves = get_all_moves();

    if (turn_ == 'w')
    {
        turn_ = 'b';
    } else
    {
        turn_ = 'w';
    }

    for (auto move : opponent_moves)
    {
        if (move.end.row == square.row && move.end.col == square.col)
        {
            return true;
        }
    }
    return false;
}


bool
Model::in_check()
{
    if (turn_ == 'w')
    {
        return square_attacked(white_king_);
    }else
    {
        return square_attacked(black_king_);
    }
}





//  ---------------------------------------------------------
// Helper functions for generating move vectors
// Invariant? -> function is called with the appropriate piece
// at the location
//  ---------------------------------------------------------

void
Model::get_castling_moves(int Krow, int Kcol, std::vector<Move>& moves)
{

    // TODO: add castling ot other side and add castling move to Move
    if (square_attacked(loc{Krow,Kcol}))
    {
        return;
    }
    if ((turn_ == 'w' && castling_rights_.wk)
        || (turn_ == 'b' && castling_rights_.bk))
    {
        if (board_[Krow][Kcol+1] == "--" && board_[Krow][Kcol+2] == "--"
            && !square_attacked(loc{Krow,Kcol+1})
            && !square_attacked (loc{Krow,Kcol+2}))
        {
            moves.push_back(Move(
                    loc{Krow,Kcol},
                    loc{Krow,Kcol+2},
                    board_,
                    false,
                    true));
        }
    }
    if((turn_ == 'w' && castling_rights_.wq)
       || (turn_ == 'b' && castling_rights_.bq))
    {
        if (board_[Krow][Kcol-1] == "--" && board_[Krow][Kcol-2] == "--"
            && board_[Krow][Kcol-3] == "--"
            && !square_attacked(loc{Krow,Kcol-1})
            && !square_attacked (loc{Krow,Kcol-2}))
        {
            moves.push_back(Move(
                    loc {Krow, Kcol},
                    loc {Krow, Kcol - 2},
                    board_,
                    false,
                    true));
        }
    }
}



void
Model::pawn_moves(int row, int col, std::vector<Move>& moves)
{
    //TODO: doesnt handle enpassant by adding it to the move
    if (turn_ == 'w'){
        if (row > 0 && board_[row - 1][col] == "--"){
            // one square move
            moves.push_back(Move(
                    loc{row,col},
                    loc{row-1,col},
                    board()));
            if (row == 6 && board_[row-2][col] == "--"){
                // two square move
                moves.push_back(Move(
                        loc{row,col},
                        loc{row-2,col},
                        board()));
            }
        }
        if (col - 1 >= 0 && row > 0){
            if (board_[row-1][col-1][0] == 'b') {
                moves.push_back(Move(
                        loc{row,col},
                        loc{row-1,col-1},
                        board()));
            }else if (row - 1 == enpassant_.row && col - 1 == enpassant_.col) {
                moves.push_back(Move(
                        loc {row, col},
                        loc {row-1, col-1},
                        board(),
                        true,
                        false));
            }
        }
        if (col + 1 <=7 && row > 0){
            if(board_[row-1][col+1][0] == 'b'){
                moves.push_back(Move(
                        loc {row, col},
                        loc {row - 1, col + 1},
                        board()));
            }else if (row - 1 == enpassant_.row && col + 1 == enpassant_.col){
                moves.push_back(Move(
                        loc {row, col},
                        loc {row-1, col+1},
                        board(),
                        true,
                        false));
            }
        }
    } else {
        if (row + 1 < 8 && board_[row+1][col] == "--")
        {
            moves.push_back(Move(
                    loc{row,col},
                    loc{row+1,col},
                    board()));
            if (row == 1 && board_[row+2][col] == "--")
            {
                moves.push_back(Move(
                        loc{row,col},
                        loc{row+2,col},
                        board()));
            }
        }
        if (col - 1 >= 0 && row + 1 < 8)
        {
            if (board_[row+1][col-1][0] == 'w')
            {
                moves.push_back(Move(
                        loc{row,col},
                        loc{row+1,col-1},
                        board()));
            }
            else if (row + 1 == enpassant_.row && col - 1 == enpassant_.col)
            {
                moves.push_back(Move(
                        loc{row,col},
                        loc{row+1,col-1},
                        board(),
                        true,
                        false));
            }
        }
        if (col + 1 <= 7 && row + 1 < 8) {
            if (board_[row + 1][col + 1][0] == 'w') {
                moves.push_back(Move(
                        loc {row, col},
                        loc {row + 1, col + 1},
                        board()));
            } else if (row + 1 == enpassant_.row && col + 1 == enpassant_.col) {
                moves.push_back(Move(
                        loc {row, col},
                        loc {row + 1, col + 1},
                        board(),
                        true,
                        false));
            }
        }
    }
}


void
Model::knight_moves(int row, int col, std::vector<Move>& moves)
{
    std::vector<loc> directions;
    directions = {loc{-2,-1},loc{-2,1},loc{-1,-2},
                  loc{-1,2},loc{1,-2},loc{1,2},
                  loc{2,-1},loc{2,1}};
    char curr_color = 'w';
    if (turn_ == 'b'){
        curr_color = 'b';
    }
    for (auto& direction : directions)
    {
        int end_row = row + direction.row;
        int end_col = col + direction.col;
        if (0 <= end_col  && end_col < 8
            && 0 <= end_row && end_row < 8
            && board_[end_row][end_col][0] != curr_color)
        {
            moves.push_back(Move(
                    loc{row,col},
                    loc{end_row,end_col},
                    board()
                    ));
        }

    }

}


void
Model::bishop_moves(int row, int col, std::vector<Move>& moves)
{
    std::vector<loc> directions;
    directions = {loc{1,1},loc{-1,1},loc{1,-1},loc{-1,-1}};
    char other_color = 'w';
    if (turn_ == 'w'){
        other_color = 'b';
    }
    for (auto& direction : directions)
    {
        for (int i = 1; i < 8; i++)
        {
            int end_row = row + direction.row * i;
            int end_col = col + direction.col * i;
            if (0 <= end_col  && end_col < 8
                && 0 <= end_row && end_row < 8)
            {
                char prefix_piece_to = board_[end_row][end_col][0];
                if (prefix_piece_to == '-')
                {
                    moves.push_back(Move(
                            loc{row,col},
                            loc{end_row,end_col},
                            board()
                    ));
                }
                else if (prefix_piece_to == other_color)
                {
                    moves.push_back(Move(
                            loc{row,col},
                            loc{end_row,end_col},
                            board()
                    ));
                    break;
                }
                else{break;}
            }
            else{break;}
        }
    }
}


void
Model::rook_moves(int row, int col, std::vector<Move>& moves)
{
    std::vector<loc> directions;
    directions = {loc{1,0},loc{-1,0},loc{0,-1},loc{0,1}};
    char other_color = 'w';
    if (turn_ == 'w'){
        other_color = 'b';
    }
    for (auto& direction : directions)
    {
        for (int i = 1; i < 8; i++)
        {
            int end_row = row + direction.row * i;
            int end_col = col + direction.col * i;
            if (0 <= end_col  && end_col < 8
                && 0 <= end_row && end_row < 8)
            {
                char prefix_piece_to = board_[end_row][end_col][0];
                if (prefix_piece_to == '-')
                {
                    moves.push_back(Move(
                            loc{row,col},
                            loc{end_row,end_col},
                            board()
                    ));
                }
                else if (prefix_piece_to == other_color)
                {
                    moves.push_back(Move(
                            loc{row,col},
                            loc{end_row,end_col},
                            board()));
                    break;
                }
                else{break;}
            }
            else{break;}
        }
    }
}


void
Model::queen_moves(int row, int col, std::vector<Move>& moves)
{
    bishop_moves(row,col,moves);
    rook_moves(row,col,moves);
}


void
Model::king_moves(int row, int col, std::vector<Move>& moves)
{
    std::vector<loc> directions;
    directions = {loc{1,1},loc{-1,1},loc{1,-1},loc{-1,-1},
                  loc{1,0},loc{-1,0},loc{0,-1},loc{0,1}};
    char curr_color = 'w';
    if (turn_ == 'b'){
        curr_color = 'b';
    }
    for (auto& direction : directions)
    {
        int end_row = row + direction.row;
        int end_col = col + direction.col;
        if (0 <= end_col  && end_col < 8
            && 0 <= end_row && end_row < 8
            && board_[end_row][end_col][0] != curr_color)
        {
            moves.push_back(Move(
                    loc{row,col},
                    loc{end_row,end_col},
                    board()));
        }
    }
}



// -----------------------------------
// Chess "ai"
// -----------------------------------
// positive score is good for white; negative is good for black
// checkmate is 10000; stalemate is 0
// note: scoring is done by "deci" pawn scoring -> a pawn is worth 1


// this tells the pawns to advance up the board
// also encourages king side pawns to stay back and shelter king
std::array<std::array<int,8>,8> pawn_loc_scores =
        {std::array<int, 8>{8, 8, 8, 9, 9, 8, 8, 8},
         std::array<int, 8>{4, 4, 5, 9, 9, 5, 4, 4},
         std::array<int, 8>{4, 4, 4, 8, 8, 4, 5, 4},
         std::array<int, 8>{4, 4, 3, 7, 7, 3, 4, 4},
         std::array<int, 8>{1, 1, 2, 6, 6, 2, 1, 1},
         std::array<int, 8>{1, 1, 1, 2, 2, 1, 1, 1},
         std::array<int, 8>{0, 0, 0, 0, 0, 5, 5, 5},
         std::array<int, 8>{0, 0, 0, 0, 0, 0, 0, 0},};
// this tells the knights to stick to the center of the board
// where they are stronger
std::array<std::array<int,8>,8> knight_loc_scores =
        {std::array<int, 8>{0, 0, 0, 0, 0, 0, 0, 0},
         std::array<int, 8>{0, 0, 0, 0, 0, 0, 0, 0},
         std::array<int, 8>{0, 0, 5, 4, 4, 5, 0, 0},
         std::array<int, 8>{0, 0, 4, 5, 5, 4, 0, 0},
         std::array<int, 8>{0, 0, 4, 5, 5, 4, 0, 0},
         std::array<int, 8>{0, 0, 5, 4, 4, 5, 0, 0},
         std::array<int, 8>{0, 0, 0, 0, 0, 0, 0, 0},
         std::array<int, 8>{0, 0, 0, 0, 0, 0, 0, 0},};

// encourages king to stay on back rank
std::array<std::array<int,8>,8> king_loc_scores =
        {std::array<int, 8>{0, 0, 0, 0, 0, 0, 0, 0},
         std::array<int, 8>{0, 0, 0, 0, 0, 0, 0, 0},
         std::array<int, 8>{0, 0, 0, 0, 0, 0, 0, 0},
         std::array<int, 8>{0, 0, 0, 0, 0, 0, 0, 0},
         std::array<int, 8>{0, 0, 0, 0, 0, 0, 0, 0},
         std::array<int, 8>{0, 0, 0, 0, 0, 0, 0, 0},
         std::array<int, 8>{1, 1, 1, 1, 1, 1, 1, 1},
         std::array<int, 8>{3, 6, 5, 4, 4, 6, 8, 3},};




int
Model::piece_score(char piece) const
{
    if (piece == 'K'){
        return 0;
    }else if (piece == 'Q'){
        return 90;
    }else if (piece == 'R'){
        return 50;
    }else if (piece == 'B'){
        return 30;
    }else if (piece == 'N'){
        return 30;
    }else{
        return 10;
    }
}

int
Model::score_the_board() const
{
    if (checkmate_)
    {
        if (turn_ == 'w')
        {
            return 10000;
        }
        else
        {
            return -10000;
        }
    }
    else if (stalemate_)
    {
        return 0;
    }

    int score = 0;
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            std::string piece = board_[r][c];
            if (piece[0] == 'w')
            {
                // value of piece
                score += piece_score(piece[1]);

                // value of position of the piece
                if (piece[1] == 'p')
                {
                    score += pawn_loc_scores[r][c];
                }
                else if (piece[1] == 'N')
                {
                    score += knight_loc_scores[r][c];
                }
                else if (piece[1] == 'K')
                {
                    score += king_loc_scores[r][c];
                }

            }
            else if (piece[0] == 'b')
            {
                score -= piece_score(piece[1]);

                if (piece[1] == 'p')
                {
                    score -= pawn_loc_scores[7-r][c];
                }
                else if (piece[1] == 'N')
                {
                    score -= knight_loc_scores[7-r][c];
                }
                else if (piece[1] == 'K')
                {
                    score -= king_loc_scores[7-r][c];
                }

            }

        }

    }
    return score;
}


void
Model::best_move()
{
    //https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
    count_ = 0;
    // leave depth on three a weird bug arises when its on an even number
    depth_ = 3;
    int turn_multi;

    // allows best_move to be used for white too
    if (turn_ == 'w')
    {
        turn_multi = 1;
    }else{
        turn_multi = -1;
    }
    std::vector<Move> valid_moves = get_valid_moves();
    best_move_ = valid_moves.at(0);

    best_move_helper(valid_moves,
                           depth_,
                           -10000,
                           10000,
                           turn_multi);
}

int
Model::best_move_helper(const std::vector<Move>& valid_moves,
                        int depth,
                        int alpha,
                        int beta,
                        int turn_multi)
{
    count_ = count_ + 1;
    if (depth == 0)
    {
        return turn_multi * score_the_board();
    }
    if (stalemate_)
    {
        return 0;
    }
    int max_score = -10000;
    for (auto& move : valid_moves)
    {
        make_move(move);
        std::vector<Move> next_moves = get_valid_moves();
        int score = -best_move_helper(next_moves,
                                      depth - 1,
                                      -beta,
                                      -alpha,
                                      -turn_multi);
        if (score > max_score)
        {
            max_score = score;
            if (depth == depth_)
            {
                best_move_ = move;
            }
        }
        undo_move();
        if (max_score > alpha)
        {
            alpha = max_score;
        }
        if (alpha >= beta)
        {
            break;
        }
    }
    return max_score;


}









