#include "Board.h"
#include <iostream>
#include <cctype>

static int pieceValue(PieceType t) {
    switch (t) {
        case PieceType::PAWN:   return 100;
        case PieceType::KNIGHT: return 320;
        case PieceType::BISHOP: return 330;
        case PieceType::ROOK:   return 500;
        case PieceType::QUEEN:  return 900;
        case PieceType::KING:   return 20000;
        default: return 0;
    }
}

Board::Board() {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            grid[r][c] = Piece{};

    PieceType backRank[8] = {
        PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN,
        PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK
    };

    for (int c = 0; c < 8; c++) {
        grid[0][c] = { backRank[c], Color::BLACK };
        grid[1][c] = { PieceType::PAWN, Color::BLACK };
        grid[6][c] = { PieceType::PAWN, Color::WHITE };
        grid[7][c] = { backRank[c], Color::WHITE };
    }

    turn = Color::WHITE;
}

void Board::print() const {
    static const char* files = "  a b c d e f g h";
    std::cout << "\n" << files << "\n";
    for (int r = 0; r < 8; r++) {
        std::cout << (8 - r) << " ";
        for (int c = 0; c < 8; c++) {
            Piece p = grid[r][c];
            char ch = '.';
            switch (p.type) {
                case PieceType::PAWN:   ch = 'P'; break;
                case PieceType::KNIGHT: ch = 'N'; break;
                case PieceType::BISHOP: ch = 'B'; break;
                case PieceType::ROOK:   ch = 'R'; break;
                case PieceType::QUEEN:  ch = 'Q'; break;
                case PieceType::KING:   ch = 'K'; break;
                default: ch = '.'; break;
            }
            if (p.color == Color::BLACK) ch = (char)tolower(ch);
            std::cout << ch << " ";
        }
        std::cout << (8 - r) << "\n";
    }
    std::cout << files << "\n\n";
}

void Board::addPawnMoves(int r, int c, Color color, std::vector<Move>& moves) const {
    int dir = (color == Color::WHITE) ? -1 : 1;
    int startRow = (color == Color::WHITE) ? 6 : 1;
    int promotionRow = (color == Color::WHITE) ? 0 : 7;

    int nr = r + dir;
    if (inBounds(nr, c) && grid[nr][c].type == PieceType::EMPTY) {
        if (nr == promotionRow) moves.push_back({r, c, nr, c, PieceType::QUEEN});
        else moves.push_back({r, c, nr, c, PieceType::EMPTY});

        if (r == startRow) {
            int nr2 = r + 2 * dir;
            if (inBounds(nr2, c) && grid[nr2][c].type == PieceType::EMPTY) {
                moves.push_back({r, c, nr2, c, PieceType::EMPTY});
            }
        }
    }

    for (int dc : {-1, 1}) {
        int nc = c + dc;
        if (inBounds(nr, nc) && grid[nr][nc].type != PieceType::EMPTY && grid[nr][nc].color != color) {
            if (nr == promotionRow) moves.push_back({r, c, nr, nc, PieceType::QUEEN});
            else moves.push_back({r, c, nr, nc, PieceType::EMPTY});
        }
    }
}

void Board::addKnightMoves(int r, int c, Color color, std::vector<Move>& moves) const {
    static const std::vector<std::pair<int,int>> offs = {
        {-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}
    };
    for (auto& d : offs) {
        int nr = r + d.first, nc = c + d.second;
        if (!inBounds(nr, nc)) continue;
        if (grid[nr][nc].type == PieceType::EMPTY || grid[nr][nc].color != color) {
            moves.push_back({r, c, nr, nc, PieceType::EMPTY});
        }
    }
}

void Board::addSlidingMoves(int r, int c, Color color,
                             const std::vector<std::pair<int,int>>& dirs,
                             std::vector<Move>& moves) const {
    for (auto& d : dirs) {
        int nr = r + d.first, nc = c + d.second;
        while (inBounds(nr, nc)) {
            if (grid[nr][nc].type == PieceType::EMPTY) {
                moves.push_back({r, c, nr, nc, PieceType::EMPTY});
            } else {
                if (grid[nr][nc].color != color) moves.push_back({r, c, nr, nc, PieceType::EMPTY});
                break;
            }
            nr += d.first; nc += d.second;
        }
    }
}

void Board::addKingMoves(int r, int c, Color color, std::vector<Move>& moves) const {
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr, nc = c + dc;
            if (!inBounds(nr, nc)) continue;
            if (grid[nr][nc].type == PieceType::EMPTY || grid[nr][nc].color != color) {
                moves.push_back({r, c, nr, nc, PieceType::EMPTY});
            }
        }
    }
}

std::vector<Move> Board::generatePseudoMoves(Color color) const {
    std::vector<Move> moves;
    static const std::vector<std::pair<int,int>> bishopDirs = {{-1,-1},{-1,1},{1,-1},{1,1}};
    static const std::vector<std::pair<int,int>> rookDirs   = {{-1,0},{1,0},{0,-1},{0,1}};
    static const std::vector<std::pair<int,int>> queenDirs  = {
        {-1,-1},{-1,1},{1,-1},{1,1},{-1,0},{1,0},{0,-1},{0,1}
    };

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece p = grid[r][c];
            if (p.color != color) continue;
            switch (p.type) {
                case PieceType::PAWN:   addPawnMoves(r, c, color, moves); break;
                case PieceType::KNIGHT: addKnightMoves(r, c, color, moves); break;
                case PieceType::BISHOP: addSlidingMoves(r, c, color, bishopDirs, moves); break;
                case PieceType::ROOK:   addSlidingMoves(r, c, color, rookDirs, moves); break;
                case PieceType::QUEEN:  addSlidingMoves(r, c, color, queenDirs, moves); break;
                case PieceType::KING:   addKingMoves(r, c, color, moves); break;
                default: break;
            }
        }
    }
    return moves;
}

bool Board::isSquareAttacked(int r, int c, Color byColor) const {
    // Pawn attacks: a pawn of byColor sits diagonally "behind" the target
    // square relative to its own advance direction.
    int pawnDir = (byColor == Color::WHITE) ? 1 : -1;
    for (int dc : {-1, 1}) {
        int pr = r + pawnDir, pc = c + dc;
        if (inBounds(pr, pc) && grid[pr][pc].type == PieceType::PAWN && grid[pr][pc].color == byColor)
            return true;
    }

    static const std::vector<std::pair<int,int>> kn = {
        {-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}
    };
    for (auto& d : kn) {
        int nr = r + d.first, nc = c + d.second;
        if (inBounds(nr, nc) && grid[nr][nc].type == PieceType::KNIGHT && grid[nr][nc].color == byColor)
            return true;
    }

    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr, nc = c + dc;
            if (inBounds(nr, nc) && grid[nr][nc].type == PieceType::KING && grid[nr][nc].color == byColor)
                return true;
        }
    }

    static const std::vector<std::pair<int,int>> rookDirs = {{-1,0},{1,0},{0,-1},{0,1}};
    for (auto& d : rookDirs) {
        int nr = r + d.first, nc = c + d.second;
        while (inBounds(nr, nc)) {
            if (grid[nr][nc].type != PieceType::EMPTY) {
                if (grid[nr][nc].color == byColor &&
                    (grid[nr][nc].type == PieceType::ROOK || grid[nr][nc].type == PieceType::QUEEN))
                    return true;
                break;
            }
            nr += d.first; nc += d.second;
        }
    }

    static const std::vector<std::pair<int,int>> bishopDirs = {{-1,-1},{-1,1},{1,-1},{1,1}};
    for (auto& d : bishopDirs) {
        int nr = r + d.first, nc = c + d.second;
        while (inBounds(nr, nc)) {
            if (grid[nr][nc].type != PieceType::EMPTY) {
                if (grid[nr][nc].color == byColor &&
                    (grid[nr][nc].type == PieceType::BISHOP || grid[nr][nc].type == PieceType::QUEEN))
                    return true;
                break;
            }
            nr += d.first; nc += d.second;
        }
    }

    return false;
}

void Board::applyMoveInternal(const Move& m) {
    UndoInfo info;
    info.move = m;
    info.captured = grid[m.toRow][m.toCol];
    info.movedOriginalType = grid[m.fromRow][m.fromCol].type;

    Piece moving = grid[m.fromRow][m.fromCol];
    if (m.promotion != PieceType::EMPTY) moving.type = m.promotion;

    grid[m.toRow][m.toCol] = moving;
    grid[m.fromRow][m.fromCol] = Piece{};

    history.push(info);
}

void Board::undoMoveInternal() {
    UndoInfo info = history.top();
    history.pop();

    Piece moved = grid[info.move.toRow][info.move.toCol];
    moved.type = info.movedOriginalType; // revert any promotion

    grid[info.move.fromRow][info.move.fromCol] = moved;
    grid[info.move.toRow][info.move.toCol] = info.captured;
}

std::vector<Move> Board::generateLegalMoves(Color color) {
    std::vector<Move> pseudo = generatePseudoMoves(color);
    std::vector<Move> legal;
    legal.reserve(pseudo.size());

    for (auto& m : pseudo) {
        applyMoveInternal(m);
        bool leavesKingInCheck = isInCheck(color);
        undoMoveInternal();
        if (!leavesKingInCheck) legal.push_back(m);
    }
    return legal;
}

bool Board::makeMove(const Move& m) {
    std::vector<Move> legal = generateLegalMoves(turn);
    for (auto& lm : legal) {
        if (lm == m) {
            applyMoveInternal(m);
            turn = opposite(turn);
            return true;
        }
    }
    return false;
}

void Board::undoMove() {
    if (history.empty()) return;
    undoMoveInternal();
    turn = opposite(turn);
}

bool Board::isInCheck(Color color) const {
    int kr = -1, kc = -1;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (grid[r][c].type == PieceType::KING && grid[r][c].color == color) {
                kr = r; kc = c;
            }
        }
    }
    if (kr == -1) return false;
    return isSquareAttacked(kr, kc, opposite(color));
}

bool Board::isCheckmate(Color color) {
    if (!isInCheck(color)) return false;
    return generateLegalMoves(color).empty();
}

bool Board::isStalemate(Color color) {
    if (isInCheck(color)) return false;
    return generateLegalMoves(color).empty();
}

int Board::evaluate() const {
    int score = 0;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece p = grid[r][c];
            if (p.type == PieceType::EMPTY) continue;
            int val = pieceValue(p.type);
            score += (p.color == Color::WHITE) ? val : -val;
        }
    }
    return score;
}
