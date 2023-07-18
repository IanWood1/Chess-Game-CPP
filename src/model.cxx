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
    auto pins_and_checks = get_pins_and_checks();
    pins_ = pins_and_checks.first;
    checks_ = pins_and_checks.second;
    in_check_ = !checks_.empty();

    std::vector<Move> moves;
    const loc& king_loc = turn_ == 'w' ? white_king_ : black_king_;
    if (in_check_) {
        if (checks_.size() == 1) {
            moves = get_all_moves();
            const loc& check_loc = checks_[0].first;
            const dir& check_dir = checks_[0].second;
            const std::string& check_piece = board_[check_loc.row][check_loc.col];
            std::vector<loc> valid_squares;
            if (check_piece[1] == 'N') {
                valid_squares.push_back(check_loc);
            }
            else {
                for (int i = 0; i < 8; i++) {
                    valid_squares.push_back({
                        king_loc.row + check_dir.y * i,
                        king_loc.col + check_dir.x * i });
                    if (valid_squares.back().row == check_loc.row && valid_squares.back().col == check_loc.row) {
                        break;
                    }
                }		
            }
            for (auto it = moves.begin(); it != moves.end();) {
                const Move& move = *it;
                if (move.piece_moved[1] != 'K') {
                    bool found = false;
                    for (loc& square : valid_squares) {
                        if (square.eq(move.end.row, move.end.col)) {
                            found = true;
                        }
                    }
                    if (!found)
                        it = moves.erase(it);
                    else
                        it++;
                }
                else {
                    it++;
                }
            }
        }
        else {
            // double check -> can only move king
            king_moves(king_loc.row, king_loc.col, moves);
        }
    }
    else {
		// no checks; all moves work!
        moves = get_all_moves();
        get_castling_moves(king_loc.row, king_loc.col, moves);	
    }

    if (moves.empty()) {
        if (in_check()) {
            checkmate_ = true;
        }
        else {
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

std::pair<std::vector<std::pair<loc, dir>>, std::vector<std::pair<loc, dir>>> 
Model::get_pins_and_checks()
{
    using piece_dir_t = std::pair<loc, dir>;
    std::vector<piece_dir_t> checks;
    std::vector<piece_dir_t> pins;
	
    bool in_check = false;
	
    const char enemy_color = turn_ == 'w' ? 'b' : 'w';
    const char my_color = turn_;
    const loc king_loc = turn_ == 'w' ? white_king_ : black_king_;

	// check for pins/checks outwards from king loc
    const std::vector<dir> directions = { {-1, 0}, {0, -1}, {1, 0}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
    for (int i = 0; i < directions.size(); i++) {
        dir curr_dir = directions[i];
        piece_dir_t possible_pin;
        bool found_possible_pin = false;

        for (int dist = 1; dist < 8; dist++) {
            loc curr_square = { king_loc.row + curr_dir.y * dist, king_loc.col + curr_dir.x * dist };
            if (curr_square.on_board()) {
                std::string& end_piece = board_[curr_square.row][curr_square.col];
                if (end_piece[0] == my_color && end_piece[1] != 'K') {
                    if (!found_possible_pin) {
                        possible_pin = { curr_square, curr_dir };
                        found_possible_pin = true;
                    }
                    else {
                        break; // no further pins/checks from this direction
                    }
                }
                else if (end_piece[0] == enemy_color) {
                    char enemy_type = end_piece[1];
                    if (
                        (i >= 0 && i <= 3 && enemy_type == 'R') ||
                        (i >= 4 && i <= 7 && enemy_type == 'B') ||
                        ((dist == 1 && enemy_type == 'p') && 
                            (
                                (enemy_color == 'w'  && (i == 5 || i == 7)) ||
                                (enemy_color == 'b' && (i == 6 || i == 4))
                                )
                            ) ||
                        (enemy_type == 'Q') ||
                        (dist == 1 && enemy_type == 'K')
                        ) {
                        if (!found_possible_pin) {
                            in_check = true;
                            checks.emplace_back(curr_square, curr_dir);
                            break;
                        }
                        else {
                            pins.push_back(std::move(possible_pin));
                            break;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
    }
    const std::vector<dir> knight_moves = { {-2, -1}, {-2, 1}, {-1, 2}, {1, 2}, {2, -1}, {2, 1}, {-1, -2}, {1, -2} };
    for (dir curr_dir : knight_moves) {
        loc curr_square = { king_loc.row + curr_dir.y, king_loc.col + curr_dir.x };
        if (curr_square.on_board()) {
            std::string& end_piece = board_[curr_square.row][curr_square.col];
            if (end_piece[0] == enemy_color && end_piece[1] == 'N') {
                in_check = true;
                checks.emplace_back(curr_square, curr_dir);
            }
        }
    }

    return { pins, checks };
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
    if (in_check_)
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
    bool is_pinned = false;
    dir pin_dir;
    for (auto it = pins_.begin(); it != pins_.end(); it++) {
        loc& square = (*it).first;
        if (square.row == row && square.col == col) {
            is_pinned = true;
            pin_dir = (*it).second;
            pins_.erase(it);
            break;
        }
    }

    int move_amount;
    int start_row;
    char enemy_color;
    loc king_loc = white_king_;
    if (turn_ == 'w') {
        move_amount = -1;
        start_row = 6;
        enemy_color = 'b';
        king_loc = white_king_;
    }
    else {
        move_amount = 1;
        start_row = 1;
        enemy_color = 'w';
        king_loc = black_king_;
    }

    if (board_[row + move_amount][col] == "--") {
        if (!is_pinned || pin_dir.eq(move_amount, 0)) {
            moves.emplace_back(loc{ row,col }, loc{ row + move_amount, col }, board_);
            if (row == start_row && board_[row + 2 * move_amount][col] == "--") {
                moves.emplace_back(loc{ row,col }, loc{ row + 2*move_amount, col }, board_);
            }
        }
    }
    if (col - 1 >= 0) {
        if (!is_pinned || pin_dir.eq(move_amount, -1)) {
            if (board_[row + move_amount][col - 1][0] == enemy_color) {
                moves.emplace_back(loc{ row,col }, loc{ row + move_amount, col - 1}, board_);
            }
            if (enpassant_.eq(row + move_amount, col - 1)) {
				bool attacking = false, blocking = false;
                if (king_loc.row == row) {
                    std::tuple<int, int, int> inside, outside;
                    if (king_loc.col < col) { // king is on the left of the pawn
                        inside = std::make_tuple(king_loc.col + 1, col - 1, 1);
                        outside = std::make_tuple( col + 1, 8, 1);
                    }
                    else {
                        inside = std::make_tuple(king_loc.col - 1, col - 1, -1);
                        outside = std::make_tuple(col - 2, -1, -1);
                    }
                    for (int i = std::get<0>(inside); i != std::get<1>(inside); i += std::get<2>(inside)) {
                        if (board_[row][i] != "--") {
                            blocking = true;
                        }
                    }
                    for (int i = std::get<0>(outside); i != std::get<1>(outside); i += std::get<2>(outside)) {
                        const std::string& piece = board_[row][i];
                        if (piece[0] == enemy_color && (piece[1] == 'R' || piece[1] == 'Q')) {
                            attacking = true;
                        }
                        else if (piece == "--") {
                            blocking = true;
                        }
                    }
                }
                if (!attacking || blocking) {
                    moves.emplace_back(loc{ row,col }, loc{ row + move_amount, col - 1}, board_, true, false);
                }
				
            }
        }
    }
    if (col + 1 <= 7) {
        if (!is_pinned || pin_dir.eq(move_amount, 1)) {
            if (board_[row + move_amount][col + 1][0] == enemy_color) {
                moves.push_back(Move(loc{ row,col }, loc{ row + move_amount, col + 1 }, board_));
            }
            if (enpassant_.eq(row + move_amount, col + 1)) {
                bool attacking = false, blocking = false;
                if (king_loc.row == row) {
                    std::tuple<int, int, int> inside, outside;
                    if (king_loc.col < col) { // king is on the left of the pawn
                        inside = std::make_tuple(king_loc.col + 1, col, 1);
                        outside = std::make_tuple(col + 2, 8, 1);
                    }
                    else {
                        inside = std::make_tuple(king_loc.col - 1, col + 1, -1);
                        outside = std::make_tuple(col - 1, -1, -1);
                    }
                    for (int i = std::get<0>(inside); i != std::get<1>(inside); i += std::get<2>(inside)) {
                        if (board_[row][i] != "--") {
                            blocking = true;
                        }
                    }
                    for (int i = std::get<0>(outside); i != std::get<1>(outside); i += std::get<2>(outside)) {
                        const std::string& piece = board_[row][i];
                        if (piece[0] == enemy_color && (piece[1] == 'R' || piece[1] == 'Q')) {
                            attacking = true;
                        }
                        else if (piece == "--") {
                            blocking = true;
                        }
                    }
                }
                if (!attacking || blocking) {
                    moves.push_back(Move(loc{ row,col }, loc{ row + move_amount, col + 1 }, board_, true, false));
                }

            }
        }
    }
}


void
Model::knight_moves(int row, int col, std::vector<Move>& moves)
{
    bool is_pinned = false;
    dir pin_dir;
    for (auto it = pins_.begin(); it != pins_.end(); it++) {
        loc& square = (*it).first;
        if (square.row == row && square.col == col) {
            is_pinned = true;
            pin_dir = (*it).second;
            pins_.erase(it);
            break;
        }
    }
    if (is_pinned) {
        return; // cant make a move if knight is pinned
    }
	
    std::vector<loc> directions 
        = {loc{-2,-1},loc{-2,1},loc{-1,-2}, loc{-1,2},loc{1,-2},loc{1,2}, loc{2,-1},loc{2,1}};
	
	
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
    bool is_pinned = false;
    dir pin_dir;
    for (auto it = pins_.begin(); it != pins_.end(); it++) {
        loc& square = (*it).first;
        if (square.row == row && square.col == col) {
            is_pinned = true;
            pin_dir = (*it).second;
            pins_.erase(it);
            break;
        }
    }
	
    std::vector<dir> directions = { {1,1},{-1,1},{1,-1},{-1,-1} };
    char other_color = 'w';
    if (turn_ == 'w'){
        other_color = 'b';
    }
    for (auto& direction : directions)
    {
        for (int i = 1; i < 8; i++)
        {
            int end_row = row + direction.y * i;
            int end_col = col + direction.x * i;
            if (
                (0 <= end_col  && end_col < 8 && 0 <= end_row && end_row < 8) &&
				(!is_pinned || pin_dir.eq(direction.x, direction.y) 
                    || pin_dir.eq(-1*direction.x, -1*direction.y))
                )
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
    bool is_pinned = false;
    dir pin_dir;
    for (auto it = pins_.begin(); it != pins_.end(); it++) {
        loc& square = (*it).first;
        if (square.row == row && square.col == col) {
            is_pinned = true;
            pin_dir = (*it).second;
			if(board_[row][col][1] != 'Q')
                pins_.erase(it); // only remove on bishop moves if queen
				
            break;
        }
    }

	
    std::vector<dir> directions = { {1,0}, {-1,0}, {0,-1}, {0,1} };
    char other_color = 'w';
    if (turn_ == 'w'){
        other_color = 'b';
    }
    for (auto& direction : directions)
    {
        for (int i = 1; i < 8; i++)
        {
            int end_row = row + direction.y * i;
            int end_col = col + direction.x * i;
            if ((0 <= end_col  && end_col < 8&& 0 <= end_row && end_row < 8) 
                && (!is_pinned || pin_dir.eq(direction.x, direction.y)
                    || pin_dir.eq(-1 * direction.x, -1 * direction.y))
                )
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
    directions = { loc{1,1},loc{-1,1},loc{1,-1},loc{-1,-1},
                  loc{1,0},loc{-1,0},loc{0,-1},loc{0,1} };
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
            if (curr_color == 'w') {
                white_king_ = { end_row, end_col };
            }
            else {
                black_king_ = { end_row, end_col };
            }

            auto pins_and_checks = get_pins_and_checks();
            if (pins_and_checks.second.empty()) {
                moves.push_back(Move(
                    loc{ row,col },
                    loc{ end_row,end_col },
                    board()));
            }

            if (curr_color == 'w') {
                white_king_ = { row,col };
            }
            else {
                black_king_ = { row, col };
            }
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
            const std::string& piece = board_[r][c];
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

int
square_score(char piece)
{
    if (piece == 'K') {
        return 0;
    }
    else if (piece == 'Q') {
        return 90;
    }
    else if (piece == 'R') {
        return 50;
    }
    else if (piece == 'B') {
        return 30;
    }
    else if (piece == 'N') {
        return 30;
    }
    else if (piece == 'p') {
        return 10;
    }
}

inline bool compare (const Move& first, const Move& other) {
    // used to determine the "strength" of a given move
    const int myCaptureScore = square_score(first.piece_captured[2]);
    const int otherCaptureScore = square_score(other.piece_captured[2]);
    if (myCaptureScore != otherCaptureScore) {
        return myCaptureScore > otherCaptureScore;
    }
    const int myForwardMovement = first.end.row - first.start.row;
    const int otherForwardMovement = other.end.row - other.start.row;
    if (myForwardMovement != otherForwardMovement) {
        return myForwardMovement > otherForwardMovement;
    }

    // deterministic else case
    return &first < &other;
}



void
Model::best_move()
{
    //https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
    count_ = 0;
    // leave depth on three a weird bug arises when its on an even number
    depth_ = 5;
    int turn_multi;

    // allows best_move to be used for white too
    if (turn_ == 'w')
    {
        turn_multi = 1;
    }else{
        turn_multi = -1;
    }
    std::vector<Move> valid_moves = get_valid_moves();
    std::sort(valid_moves.begin(), valid_moves.end(), compare);
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









