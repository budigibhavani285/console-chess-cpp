#include "AI.h"
#include <limits>
#include <algorithm>

int AI::minimax(Board& board, int depth, int alpha, int beta, bool maximizing) {
    Color side = maximizing ? Color::WHITE : Color::BLACK;
    std::vector<Move> moves = board.generateLegalMoves(side);

    if (moves.empty()) {
        if (board.isInCheck(side)) {
            // Checkmate: strongly favor/penalize, prefer faster mates via depth bonus.
            return maximizing ? (-100000 - depth) : (100000 + depth);
        }
        return 0; // stalemate
    }

    if (depth == 0) {
        return board.evaluate();
    }

    if (maximizing) {
        int best = std::numeric_limits<int>::min();
        for (auto& m : moves) {
            board.makeMove(m);
            int val = minimax(board, depth - 1, alpha, beta, false);
            board.undoMove();
            best = std::max(best, val);
            alpha = std::max(alpha, best);
            if (beta <= alpha) break; // prune
        }
        return best;
    } else {
        int best = std::numeric_limits<int>::max();
        for (auto& m : moves) {
            board.makeMove(m);
            int val = minimax(board, depth - 1, alpha, beta, true);
            board.undoMove();
            best = std::min(best, val);
            beta = std::min(beta, best);
            if (beta <= alpha) break; // prune
        }
        return best;
    }
}

Move AI::findBestMove(Board& board, Color aiColor, int depth) {
    std::vector<Move> moves = board.generateLegalMoves(aiColor);
    if (moves.empty()) return Move{-1, -1, -1, -1};

    bool maximizing = (aiColor == Color::WHITE);
    Move bestMove = moves[0];
    int bestVal = maximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

    for (auto& m : moves) {
        board.makeMove(m);
        int val = minimax(board, depth - 1,
                           std::numeric_limits<int>::min(),
                           std::numeric_limits<int>::max(),
                           !maximizing);
        board.undoMove();

        if (maximizing && val > bestVal) { bestVal = val; bestMove = m; }
        if (!maximizing && val < bestVal) { bestVal = val; bestMove = m; }
    }
    return bestMove;
}
