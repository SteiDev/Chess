#include "../include/pieceMoves.h"
#include "../include/chessboardlogic.h"
#include "../include/gentools.h"
#include "../include/typedefs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

void Piece_ToggleSpecialMove(playerPiece_t*** board_ptr, const u16Vect2 from, const u16Vect2 to) {
    (*board_ptr)[from.y][from.x].usedSpecialMove = false;
    (*board_ptr)[to.y][to.x].usedSpecialMove = true;
}

bool Piece_CheckOwnerDestination(playerPiece_t** restrict board, const u16Vect2 from, const u16Vect2 to) {
    if(board[from.y][from.x].pieceColor != board[to.y][to.x].pieceColor) {
        return true;
    } else {
        return false;
    }
}

bool Piece_CheckCastling(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to) {
    u16 startIndex = 0;
    u16 stopIndex = 0;

    if((mainBoard[from.y][from.x].pieceType == king && mainBoard[to.y][to.x].pieceType == rook) || (mainBoard[from.y][from.x].pieceType == rook && mainBoard[to.y][to.x].pieceType == king))
    if(mainBoard[from.y][from.x].usedSpecialMove == false || mainBoard[to.y][to.x].usedSpecialMove == false) {
        if(from.y == to.y) {
            startIndex = Util_u16Min(from.x, to.x) + 1;
            stopIndex = Util_u16Max(from.x, to.x);

            if(startIndex >= stopIndex) {
                return false;
            }

            for(u16 searchIndex = startIndex; searchIndex < stopIndex; ++searchIndex) {
                if(mainBoard[from.y][searchIndex].pieceType != piece_NONE) {
                    return false;
                }
            }

            return true;
        }
    }

    return false;
}

bool Piece_CheckPawnPromotion(playerPiece_t** restrict board, const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize) {
    if(board[from.y][from.x].usedSpecialMove != false) {
        if(to.y == 0 || to.y == boardSize.y - 1) {
            return true;
        }
    }

    return false;
}

static void __MovePiece__(playerPiece_t*** restrict board_ptr, const u16Vect2 from, const u16Vect2 to) {
    (*board_ptr)[to.y][to.x].pieceType = (*board_ptr)[from.y][from.x].pieceType;
    (*board_ptr)[to.y][to.x].pieceColor = (*board_ptr)[from.y][from.x].pieceColor;

    (*board_ptr)[from.y][from.x].pieceType = piece_NONE;
    (*board_ptr)[from.y][from.x].pieceColor = color_NONE;

    Piece_ToggleSpecialMove(board_ptr, from, to);
}


void Piece_MoveCastling(playerPiece_t*** restrict board_ptr, const u16Vect2 from, const u16Vect2 to) {
    u16 kingX = 0;
    u16 rookX = 0;
    u16 newKingX = 0;
    u16 newRookX = 0;
    
    // identify king and rook
    if((*board_ptr)[from.y][from.x].pieceType == king) {
        kingX = from.x;
        rookX = to.x;
    } else {
        kingX = to.x;
        rookX = from.x;
    }

    if(rookX > kingX) {
        // kingside castling
        newKingX = kingX + 2;
        newRookX = kingX + 1;
    } else {
        // queenside castling
        newKingX = kingX - 2;
        newRookX = kingX - 1;
    }

    __MovePiece__(board_ptr, (u16Vect2){.x = kingX, .y = from.y}, (u16Vect2){.x = newKingX, .y = from.y});
    __MovePiece__(board_ptr, (u16Vect2){.x = rookX, .y = from.y}, (u16Vect2){.x = newRookX, .y = from.y});
}

void Piece_Move(playerPiece_t*** restrict board_ptr, const u16Vect2 from, const u16Vect2 to, bool* restrict doCastling) {
    if((*doCastling) == false) {
        __MovePiece__(board_ptr, from, to);
    } else {
        *doCastling = false;
        Piece_MoveCastling(board_ptr, from, to);
    }
}

bool Piece_ISOwnerValid(playerPiece_t** restrict mainBoard, const u16Vect2 from, const __color_t currentTurnColor) {
    if(mainBoard[from.y][from.x].pieceColor == currentTurnColor){
        return true;
    }else{
        return false;
    }
}

bool __CheckValidMove_Pawn__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to) {
    playerPiece_t fromPiece = mainBoard[from.y][from.x];
    playerPiece_t toPiece = mainBoard[to.y][to.x];

    switch(fromPiece.pieceColor) {
        case white:
            // Normal move forward 1
            if (from.x == to.x && to.y == from.y + 1 && toPiece.pieceType == piece_NONE) {
                return true;
            }
            // Double move forward from initial position (assuming rank 1 for white pawns, y=1)
            if (from.x == to.x && to.y == from.y + 2 && !fromPiece.usedSpecialMove &&
                mainBoard[from.y + 1][from.x].pieceType == piece_NONE && toPiece.pieceType == piece_NONE) {
                return true;
            }
            // Capture diagonally
            if ((to.x == from.x + 1 || to.x == from.x - 1) && to.y == from.y + 1 && toPiece.pieceColor == black) {
                return true;
            }
            break;

        case black:
            // Normal move forward 1
            if (from.x == to.x && to.y == from.y - 1 && toPiece.pieceType == piece_NONE) {
                return true;
            }
            // Double move forward from initial position (assuming rank 6 for black pawns, y=6)
            if (from.x == to.x && to.y == from.y - 2 && !fromPiece.usedSpecialMove &&
                mainBoard[from.y - 1][from.x].pieceType == piece_NONE && toPiece.pieceType == piece_NONE) {
                return true;
            }
            // Capture diagonally
            if ((to.x == from.x + 1 || to.x == from.x - 1) && to.y == from.y - 1 && toPiece.pieceColor == white) {
                return true;
            }
            break;

        default:
            // Invalid color or no piece
            break;
    }

    return false;
}

bool __CheckValidMove_Knight__(const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize) {
    int dx = abs((int)from.x - (int)to.x);
    int dy = abs((int)from.y - (int)to.y);

    if (to.x >= boardSize.x || to.y >= boardSize.y)
        return false;

    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

bool __CheckValidMove_Bishop__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize) {
    // Check both positions are within bounds
    if (from.x >= boardSize.x || from.y >= boardSize.y ||
        to.x >= boardSize.x || to.y >= boardSize.y) {
        return false;
    }

    // Check if the move is diagonal
    if (abs((int)from.x - (int)to.x) != abs((int)from.y - (int)to.y)) {
        return false;
    }

    // Determine direction of steps
    int stepX = (to.x > from.x) ? 1 : -1;
    int stepY = (to.y > from.y) ? 1 : -1;

    u16 x = from.x + stepX;
    u16 y = from.y + stepY;

    // Check each square along the diagonal (excluding the destination)
    while (x != to.x && y != to.y) {
        // Bounds check (defensive; should be fine due to diagonal match)
        if (x >= boardSize.x || y >= boardSize.y) {
            return false;
        }

        if (mainBoard[y][x].pieceColor != color_NONE) {
            return false; // Path is blocked
        }

        x += stepX;
        y += stepY;
    }

    // Final destination check
    __color_t fromColor = mainBoard[from.y][from.x].pieceColor;
    __color_t toColor = mainBoard[to.y][to.x].pieceColor;

    return (toColor == color_NONE || toColor != fromColor);
}

bool __CheckValidMove_Rook__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize) {
    i32 stepX = 0;
    i32 stepY = 0;

    // Ensure both 'from' and 'to' are within bounds
    if (from.x >= boardSize.x || from.y >= boardSize.y || to.x >= boardSize.x || to.y >= boardSize.y) {
        return false;
    }

    // Rook must move either in a straight line: horizontally or vertically
    if (from.x != to.x && from.y != to.y) {
        return false;
    }

    // Horizontal move
    if (from.y == to.y) {
        stepX = (to.x > from.x) ? 1 : -1;
        for (u16 x = from.x + stepX; x != to.x; x += stepX) {
            if (x >= boardSize.x) return false;  // Prevent OOB access
            if (mainBoard[from.y][x].pieceColor != color_NONE) {
                return false; // Blocked
            }
        }
    }

    // Vertical move
    else if (from.x == to.x) {
        stepY = (to.y > from.y) ? 1 : -1;
        for (u16 y = from.y + stepY; y != to.y; y += stepY) {
            if (y >= boardSize.y) return false;  // Prevent OOB access
            if (mainBoard[y][from.x].pieceColor != color_NONE) {
                return false; // Blocked
            }
        }
    }

    // Final square: must be empty or contain opponent's piece
    __color_t fromColor = mainBoard[from.y][from.x].pieceColor;
    __color_t toColor = mainBoard[to.y][to.x].pieceColor;

    return (toColor == color_NONE || toColor != fromColor);
}

bool __CheckValidMove_Queen__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize) {
    if((__CheckValidMove_Bishop__(mainBoard, from, to, boardSize) == true) || (__CheckValidMove_Rook__(mainBoard, from, to, boardSize) == true)){
        return true;
    }else{
        return false;
    }
}
/*
bool __CheckValidMove_King__(const u16Vect2 from, const u16Vect2 to) {
    if((to.x == from.x + 1) || (to.x == from.x - 1)){
        if((to.y == from.y + 1) || (to.y == from.y - 1)){
            return true;
        }    
    }
    return false;
}*/

bool __CheckValidMove_King__(const u16Vect2 from, const u16Vect2 to) {
    int dx = abs((int)to.x - (int)from.x);
    int dy = abs((int)to.y - (int)from.y);

    return (dx <= 1 && dy <= 1) && (dx + dy > 0);
}



void Board_ApplyMove(playerPiece_t** board, Move move) {
    // Move piece to target square
    board[move.to.y][move.to.x] = board[move.from.y][move.from.x];

    // Clear the source square
    board[move.from.y][move.from.x].pieceType = piece_NONE;
    board[move.from.y][move.from.x].pieceColor = color_NONE;

    // Handle castling
    if (move.isCastling) {
        if (move.to.x == 6) {
            // Kingside castling
            board[move.to.y][5] = board[move.to.y][7]; // Move rook
            board[move.to.y][7].pieceType = piece_NONE;
            board[move.to.y][7].pieceColor = color_NONE;
        } else if (move.to.x == 2) {
            // Queenside castling
            board[move.to.y][3] = board[move.to.y][0]; // Move rook
            board[move.to.y][0].pieceType = piece_NONE;
            board[move.to.y][0].pieceColor = color_NONE;
        }
    }

    // Handle promotion
    if (move.isPromotion) {
        board[move.to.y][move.to.x].pieceType = queen; // Promote to queen
    }
}

playerPiece_t** Board_Clone(playerPiece_t** board, u16Vect2 boardSize) {
    playerPiece_t** clone = malloc(boardSize.y * sizeof(playerPiece_t*));
    if (!clone) {
        perror("malloc failed for clone rows");
        exit(1);
    }

    for (int y = 0; y < boardSize.y; y++) {
        clone[y] = malloc(boardSize.x * sizeof(playerPiece_t));
        if (!clone[y]) {
            perror("malloc failed for clone row");
            exit(1);
        }

        for (int x = 0; x < boardSize.x; x++) {
            clone[y][x] = board[y][x];
        }
    }

    return clone;
}

int GenerateLegalMoves(playerPiece_t** restrict board, const u16Vect2 boardSize, __color_t currentColor, Move* outMoves) {
    int moveCount = 0;

    for (u16 y = 0; y < boardSize.y; y++) {
        for (u16 x = 0; x < boardSize.x; x++) {
            playerPiece_t piece = board[y][x];

            if (piece.pieceColor != currentColor || piece.pieceType == piece_NONE) {
                continue; // skip empty or opponent pieces
            }
        
            u16Vect2 from = {x, y};

            for (u16 ty = 0; ty < boardSize.y; ty++) {
                for (u16 tx = 0; tx < boardSize.x; tx++) {
                    u16Vect2 to = {tx, ty};
                    bool isValid = false;

                    switch (piece.pieceType) {
                        case pawn:
                            isValid = __CheckValidMove_Pawn__(board, from, to);
                            break;
                        case knight:
                            isValid = __CheckValidMove_Knight__(from, to, boardSize);
                            break;
                        case bishop:
                            isValid = __CheckValidMove_Bishop__(board, from, to, boardSize);
                            break;
                        case rook:
                            isValid = __CheckValidMove_Rook__(board, from, to, boardSize);
                            break;
                        case queen:
                            isValid = __CheckValidMove_Queen__(board, from, to, boardSize);
                            break;
                        case king:
                            isValid = __CheckValidMove_King__(from, to);
                            break;
                        default:
                            break;
                    }

                    if (!isValid) continue;

                    // Skip moves that capture own piece
                    playerPiece_t target = board[to.y][to.x];
                    if (target.pieceColor == currentColor) continue;

                    Move newMove = {
                        .from = from,
                        .to = to,
                        .isCastling = (piece.pieceType == king && Piece_CheckCastling(board, from, to)),
                        .isPromotion = (piece.pieceType == pawn && Piece_CheckPawnPromotion(board, from, to, boardSize)),
                    };

                    outMoves[moveCount++] = newMove;
                }
            }
        }
    }

    return moveCount;
}

int EvaluateBoard(playerPiece_t** board, u16Vect2 boardSize, __color_t maximizingColor) {
    int scoreMaximizing = 0;
    int scoreOpponent = 0;

    for (u16 y = 0; y < boardSize.y; y++) {
        for (u16 x = 0; x < boardSize.x; x++) {
            playerPiece_t piece = board[y][x];
            int pieceValue = 0;

            switch (piece.pieceType) {
                case pawn:
                    pieceValue = 1;
                    break;
                case knight:
                    pieceValue = 3;
                    break;
                case bishop:
                    pieceValue = 3;
                    break;
                case rook:
                    pieceValue = 5;
                    break;
                case queen:
                    pieceValue = 9;
                    break;
                case king:
                    pieceValue = 0; // King usually not counted for material evaluation
                    break;
                default:
                    pieceValue = 0;
            }

            if (piece.pieceColor == maximizingColor) {
                scoreMaximizing += pieceValue;  
            } else if (piece.pieceColor != color_NONE) {
                scoreOpponent += pieceValue;
            }
        }
    }

    return scoreMaximizing - scoreOpponent;
}

#define MIN_EVAL -1000000
#define MAX_EVAL  1000000

int Minimax(playerPiece_t** board, u16Vect2 boardSize, __color_t currentColor, int depth, int alpha, int beta, bool maximizingPlayer, __color_t rootColor) {
    if (depth == 0) {
        printf("[Eval] Leaf node reached — Evaluating board for color '%c'\n", rootColor);
        return EvaluateBoard(board, boardSize, rootColor);
    }

    Move legalMoves[256];
    int legalMoveCount = GenerateLegalMoves(board, boardSize, currentColor, legalMoves);
    printf("[Minimax] Depth %d — Color '%c' has %d legal move(s)\n", depth, currentColor, legalMoveCount);

    if (legalMoveCount == 0) {
        // No moves — could be checkmate or stalemate
        return EvaluateBoard(board, boardSize, rootColor);
    }

    int bestScore = maximizingPlayer ? MIN_EVAL : MAX_EVAL;

    for (int i = 0; i < legalMoveCount; ++i) {
        playerPiece_t** boardCopy = Board_Clone(board, boardSize);
        if (!boardCopy) continue;

        // Defensive bounds check
        if (legalMoves[i].to.x >= boardSize.x || legalMoves[i].to.y >= boardSize.y ||
            legalMoves[i].from.x >= boardSize.x || legalMoves[i].from.y >= boardSize.y) {
            printf("[Warn] Skipping invalid move: (%d,%d) -> (%d,%d)\n",
                legalMoves[i].from.x, legalMoves[i].from.y,
                legalMoves[i].to.x, legalMoves[i].to.y);
            Board_Free(&boardCopy, boardSize.y);
            continue;
        }

        Board_ApplyMove(boardCopy, legalMoves[i]);

        printf("[Minimax] Trying move: (%d,%d) -> (%d,%d) at depth %d\n",
            legalMoves[i].from.x, legalMoves[i].from.y,
            legalMoves[i].to.x, legalMoves[i].to.y, depth);

        int score = Minimax(
            boardCopy,
            boardSize,
            (currentColor == white ? black : white),
            depth - 1,
            alpha,
            beta,
            !maximizingPlayer,
            rootColor 
        );

        Board_Free(&boardCopy, boardSize.y);

        if (maximizingPlayer) {
            if (score > bestScore) bestScore = score;
            if (bestScore > alpha) alpha = bestScore;
        } else {
            if (score < bestScore) bestScore = score;
            if (bestScore < beta) beta = bestScore;
        }

        if (beta <= alpha) {
            printf("[Prune] Alpha-beta pruning at depth %d — alpha: %d, beta: %d\n", depth, alpha, beta);
            break;
        }
    }

    return bestScore;
}

Move FindBestMove(playerPiece_t** board, u16Vect2 boardSize, __color_t aiColor, int depth) {
    Move legalMoves[256];
    int legalMoveCount = GenerateLegalMoves(board, boardSize, aiColor, legalMoves);

    if (legalMoveCount == 0) {
        Move nullMove = { .from = {0, 0}, .to = {0, 0} };
        return nullMove;
    }

    int bestScore = MIN_EVAL;
    int bestMoveIndex = -1;

    for (int i = 0; i < legalMoveCount; i++) {
        playerPiece_t** boardCopy = Board_Clone(board, boardSize);
        if (!boardCopy) continue;
        Board_ApplyMove(boardCopy, legalMoves[i]);

        int score = Minimax(
            boardCopy,
            boardSize,
            (aiColor == white ? black : white),
            depth - 1,
            MIN_EVAL,
            MAX_EVAL,
            false,
            aiColor
        );

        Board_Free(&boardCopy, boardSize.y);
        
        printf("[AI] Evaluating move %d: (%d,%d) -> (%d,%d) | Score = %d\n",
            i, legalMoves[i].from.x, legalMoves[i].from.y,
            legalMoves[i].to.x, legalMoves[i].to.y, score);

        if (score > bestScore) {
            printf("[AI] New best move found! Move %d: (%d,%d) -> (%d,%d) | Score = %d\n",
                i, legalMoves[i].from.x, legalMoves[i].from.y,
                legalMoves[i].to.x, legalMoves[i].to.y, score);
            bestScore = score;
            bestMoveIndex = i;
        }
    }

    if (bestMoveIndex != -1) {
        printf("[AI] Best move selected: (%d,%d) -> (%d,%d) [Index %d] | Final score = %d\n",
            legalMoves[bestMoveIndex].from.x, legalMoves[bestMoveIndex].from.y,
            legalMoves[bestMoveIndex].to.x, legalMoves[bestMoveIndex].to.y,
            bestMoveIndex, bestScore);
        return legalMoves[bestMoveIndex];
    }

    // No valid moves (shouldn't happen unless in checkmate/stalemate)
    Move nullMove = { .from = {0, 0}, .to = {0, 0} };
    return nullMove;
}

bool IsValidMove(Move move) {
    return !(move.from.x == 0 && move.from.y == 0 && move.to.x == 0 && move.to.y == 0);
}