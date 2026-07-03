#pragma once
#include "Board.h"

// Simple chess AI using the minimax algorithm with alpha-beta pruning
// (classic DSA: recursion + tree search + pruning optimization).
class AI {
public:
    static Move findBestMove(Board& board, Color aiColor, int depth);

private:
    static int minimax(Board& board, int depth, int alpha, int beta, bool maximizing);
};
