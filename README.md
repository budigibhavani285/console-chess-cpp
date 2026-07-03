<<<<<<< HEAD
# Console Chess (C++)

A complete, playable console chess game in C++17 — Human vs Human or Human vs AI —
built as a Data Structures & Algorithms showcase project.

## Features
- Full legal move generation for all pieces (pawns incl. double-step and
  promotion, knights, bishops, rooks, queen, king)
- Check, checkmate, and stalemate detection
- Move undo
- AI opponent using **minimax with alpha-beta pruning**
- Simple text UI (algebraic-style input, e.g. `e2e4`, `e7e8q` for promotion)

> Note: castling and en-passant are intentionally left out to keep the code
> approachable — see "Ideas to extend" below if you want to add them.

## DSA concepts used
| Concept | Where |
|---|---|
| 2D array | `Piece grid[8][8]` board representation |
| Stack | `std::stack<UndoInfo>` for move history / undo (backtracking) |
| Vectors / dynamic arrays | move lists |
| Recursion | `AI::minimax` |
| Tree search + pruning | alpha-beta pruning in minimax |
| Backtracking | legal-move filtering: apply a pseudo-move, test check, undo |
| Enums / structs | `PieceType`, `Color`, `Piece`, `Move` |

## Project structure
```
chess_project/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── Types.h     # Piece, Color, Move, PieceType
│   ├── Board.h      # Board class declaration
│   └── AI.h         # Minimax AI declaration
└── src/
    ├── Board.cpp     # Move generation, check/mate detection, undo
    ├── AI.cpp        # Minimax + alpha-beta pruning
    └── main.cpp       # Console UI / game loop
```

## Build & run

### Option A: CMake (recommended)
```bash
mkdir build && cd build
cmake ..
cmake --build .
./chess          # or chess.exe on Windows
```

### Option B: Direct g++ (no CMake needed)
```bash
g++ -std=c++17 -O2 -Iinclude src/main.cpp src/Board.cpp src/AI.cpp -o chess
./chess
```

Works on Linux, macOS, and Windows (via MinGW/g++ or MSVC — adjust the
compile command for `cl.exe` if using Visual Studio directly).

## How to play
- On launch, choose **1** for Human vs Human or **2** for Human vs AI.
- Moves are entered as `<from><to>`, e.g. `e2e4` moves the piece on e2 to e4.
- Pawn promotion: append the piece letter, e.g. `e7e8q` promotes to queen
  (`q`, `r`, `b`, `n` supported).
- Type `undo` to take back the last move (and the AI's reply, if playing vs AI).
- Type `quit` to exit.

## Ideas to extend
- Add castling and en passant
- Add a proper GUI (SFML / Qt) instead of console text
- Improve AI evaluation (piece-square tables, mobility, king safety)
- Add iterative deepening / a transposition table (hash map) for a stronger AI
- Save/load games (PGN-style notation) using file I/O
=======
# console-chess-cpp
Chess app project
>>>>>>> cb52a92963a90c144b210958a8928ca6bdee04e8
