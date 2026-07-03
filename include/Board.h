#pragma once
#include "Types.h"
#include <vector>
#include <stack>

// Board owns the 8x8 grid (DSA: 2D array), a move-history stack (DSA: stack,
// used for undo/backtracking), and all legal-move generation logic.
class Board {
public:
    Board();

    void print() const;

    // Returns only fully legal moves (pseudo-moves filtered so the mover's
    // own king is never left in check).
    std::vector<Move> generateLegalMoves(Color color);

    // Validates m against generateLegalMoves(sideToMove()), applies it and
    // switches the turn. Returns false (no-op) if m is not legal.
    bool makeMove(const Move& m);

    // Reverts the last move applied via makeMove.
    void undoMove();

    bool isInCheck(Color color) const;
    bool isCheckmate(Color color);
    bool isStalemate(Color color);

    Color sideToMove() const { return turn; }
    Piece getPiece(int r, int c) const { return grid[r][c]; }

    // Simple material evaluation, positive favors White.
    int evaluate() const;

private:
    Piece grid[8][8];
    Color turn;

    struct UndoInfo {
        Move move;
        Piece captured;
        PieceType movedOriginalType; // to correctly revert promotions
    };
    std::stack<UndoInfo> history;

    std::vector<Move> generatePseudoMoves(Color color) const;
    void addPawnMoves(int r, int c, Color color, std::vector<Move>& moves) const;
    void addKnightMoves(int r, int c, Color color, std::vector<Move>& moves) const;
    void addSlidingMoves(int r, int c, Color color,
                          const std::vector<std::pair<int,int>>& dirs,
                          std::vector<Move>& moves) const;
    void addKingMoves(int r, int c, Color color, std::vector<Move>& moves) const;

    bool isSquareAttacked(int r, int c, Color byColor) const;

    void applyMoveInternal(const Move& m);
    void undoMoveInternal();

    static bool inBounds(int r, int c) { return r >= 0 && r < 8 && c >= 0 && c < 8; }
};
