#pragma once
#include <string>

enum class PieceType { EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
enum class Color { NONE, WHITE, BLACK };

struct Piece {
    PieceType type = PieceType::EMPTY;
    Color color = Color::NONE;
};

struct Move {
    int fromRow, fromCol, toRow, toCol;
    PieceType promotion = PieceType::EMPTY; // used only for pawn promotion

    bool operator==(const Move& other) const {
        return fromRow == other.fromRow && fromCol == other.fromCol &&
               toRow == other.toRow && toCol == other.toCol &&
               promotion == other.promotion;
    }
};

inline Color opposite(Color c) {
    if (c == Color::WHITE) return Color::BLACK;
    if (c == Color::BLACK) return Color::WHITE;
    return Color::NONE;
}
