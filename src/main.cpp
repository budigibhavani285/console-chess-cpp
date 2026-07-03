#include <iostream>
#include <string>
#include <cctype>
#include "Board.h"
#include "AI.h"

static bool parseMove(const std::string& s, Move& m) {
    if (s.size() < 4) return false;

    int fc = s[0] - 'a';
    int fr = 8 - (s[1] - '0');
    int tc = s[2] - 'a';
    int tr = 8 - (s[3] - '0');

    if (fc < 0 || fc > 7 || tc < 0 || tc > 7 || fr < 0 || fr > 7 || tr < 0 || tr > 7)
        return false;

    m = {fr, fc, tr, tc, PieceType::EMPTY};

    if (s.size() >= 5) {
        char p = (char)tolower(s[4]);
        switch (p) {
            case 'q': m.promotion = PieceType::QUEEN;  break;
            case 'r': m.promotion = PieceType::ROOK;   break;
            case 'b': m.promotion = PieceType::BISHOP; break;
            case 'n': m.promotion = PieceType::KNIGHT; break;
            default: break;
        }
    }
    return true;
}

int main() {
    Board board;

    std::cout << "=====================================\n";
    std::cout << "   Console Chess  (C++ / DSA demo)\n";
    std::cout << "=====================================\n";
    std::cout << "Moves are entered like: e2e4   (from-square to-square)\n";
    std::cout << "Promotion example:      e7e8q  (promote to queen)\n\n";

    std::cout << "1. Human vs Human\n2. Human vs AI\nChoose: ";
    int mode = 1;
    std::cin >> mode;

    bool vsAI = (mode == 2);
    Color humanColor = Color::WHITE;
    int aiDepth = 3;

    if (vsAI) {
        std::cout << "Play as (1) White or (2) Black? ";
        int choice = 1;
        std::cin >> choice;
        humanColor = (choice == 1) ? Color::WHITE : Color::BLACK;

        std::cout << "AI search depth (2-4 recommended, higher = slower/stronger): ";
        std::cin >> aiDepth;
        if (aiDepth < 1) aiDepth = 1;
    }

    while (true) {
        board.print();
        Color side = board.sideToMove();
        std::string sideName = (side == Color::WHITE) ? "White" : "Black";

        if (board.isCheckmate(side)) {
            std::cout << sideName << " is checkmated. "
                      << (side == Color::WHITE ? "Black" : "White") << " wins!\n";
            break;
        }
        if (board.isStalemate(side)) {
            std::cout << "Stalemate. It's a draw.\n";
            break;
        }
        if (board.isInCheck(side)) {
            std::cout << sideName << " is in check!\n";
        }

        if (vsAI && side != humanColor) {
            std::cout << "AI (" << sideName << ") is thinking...\n";
            Move aiMove = AI::findBestMove(board, side, aiDepth);
            if (aiMove.fromRow == -1) break; // no legal moves (shouldn't happen, safety net)
            board.makeMove(aiMove);
            continue;
        }

        std::cout << sideName << " to move (e.g. e2e4, 'undo', or 'quit'): ";
        std::string input;
        std::cin >> input;

        if (input == "quit") break;
        if (input == "undo") {
            board.undoMove();
            if (vsAI) board.undoMove(); // undo AI's reply too
            continue;
        }

        Move m;
        if (!parseMove(input, m) || !board.makeMove(m)) {
            std::cout << "Illegal move, try again.\n";
        }
    }

    std::cout << "\nGame over. Thanks for playing!\n";
    return 0;
}
