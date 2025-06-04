#ifndef __PIECEMOVES_H__
#define __PIECEMOVES_H__

#include "../include/typedefs.h"
#include "../include/chessboardlogic.h"

// [Piece movement related macros]

bool Piece_CheckOwnerDestination(playerPiece_t** restrict board, const u16Vect2 from, const u16Vect2 to);

/**
 * @brief #### Move a piece to the desired location
 * @brief Moves a piece to the desired location with checking and validation of the location.
 * @param board_ptr Pointer to the main board.
 * @param fromX The X coordinates (number of lines) of the piece to be moved.
 * @param fromY The Y coordinates (number of columns) of the piece to be moved.
 * @param toX The X coordinates (number of lines) of the location where the piece should be moved.
 * @param toY The Y coordinates (number of columns) of the location where the piece should be moved.
 */
void Piece_Move(playerPiece_t*** restrict board_ptr, const u16Vect2 from, const u16Vect2 to, bool* restrict doCastling);

/**
 * @brief #### Check the color of the piece
 * @brief Checks if the color is the same as the current player.
 * @param fromX The X coordinates (number of lines) of the piece.
 * @param fromY The Y coordinates (number of columns) of the piece.
 * @param currentTurnColor The color of the current player.
 * @return false if the colors don't match, true otherwise.
 */
bool Piece_ISOwnerValid(playerPiece_t** restrict mainBoard, const u16Vect2 from, const __color_t currentTurnColor);

/**
 * @brief #### Check if a pawn can move
 * @brief Checks if a pawn can move.
 * @note This function can throw exceptions
 * @param mainBoard The main board.
 * @param fromX The X coordinates (number of lines) of the pawn.
 * @param fromY The Y coordinates (number of columns) of the pawn.
 * @param toX The X coordinates (number of lines) of the location where the pawn should be moved.
 * @param toY The Y coordinates (number of columns) of the location where the pawn should be moved.
 * @return INVALID_MOVE if the move isn't possible, VALID_MOVE otherwise.
 */
bool __CheckValidMove_Pawn__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to);

/**
 * @brief #### Check if a knight can move
 * @brief Checks if a knight can move.
 * @param fromX The X coordinates (number of lines) of the knight.
 * @param fromY The Y coordinates (number of columns) of the knight.
 * @param toX The X coordinates (number of lines) of the location where the knight should be moved.
 * @param toY The Y coordinates (number of columns) of the location where the knight should be moved.
 * @param boardX The X coordinates (number of lines) of the main board.
 * @param boardY The Y coordinates (number of columns) of the main board.
 * @return INVALID_MOVE if the move isn't possible, VALID_MOVE otherwise.
 */
bool __CheckValidMove_Knight__(const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize);

/**
 * @brief #### Check if a bishop can move
 * @brief Checks if a bishop can move.
 * @param mainBoard The main board.
 * @param fromX The X coordinates (number of lines) of the bishop.
 * @param fromY The Y coordinates (number of columns) of the bishop.
 * @param toX The X coordinates (number of lines) of the location where the bishop should be moved.
 * @param toY The Y coordinates (number of columns) of the location where the bishop should be moved.
 * @param boardX The X coordinates (number of lines) of the main board.
 * @param boardY The Y coordinates (number of columns) of the main board.
 * @return INVALID_MOVE if the move isn't possible, VALID_MOVE otherwise.
 */
bool __CheckValidMove_Bishop__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize);

/**
 * @brief #### Check if a rook can move
 * @brief Checks if a rook can move.
 * @param mainBoard The main board.
 * @param fromX The X coordinates (number of lines) of the rook.
 * @param fromY The Y coordinates (number of columns) of the rook.
 * @param toX The X coordinates (number of lines) of the location where the rook should be moved.
 * @param toY The Y coordinates (number of columns) of the location where the rook should be moved.
 * @param boardX The X coordinates (number of lines) of the main board.
 * @param boardY The Y coordinates (number of columns) of the main board.
 * @return INVALID_MOVE if the move isn't possible, VALID_MOVE otherwise.
 */
bool __CheckValidMove_Rook__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize);

/**
 * @brief #### Check if a queen can move
 * @brief Checks if a queen can move.
 * @param mainBoard The main board.
 * @param fromX The X coordinates (number of lines) of the queen.
 * @param fromY The Y coordinates (number of columns) of the queen.
 * @param toX The X coordinates (number of lines) of the location where the queen should be moved.
 * @param toY The Y coordinates (number of columns) of the location where the queen should be moved.
 * @param boardX The X coordinates (number of lines) of the main board.
 * @param boardY The Y coordinates (number of columns) of the main board.
 * @return INVALID_MOVE if the move isn't possible, VALID_MOVE otherwise.
 */
bool __CheckValidMove_Queen__(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize);

/**
 * @brief #### Check if a king can move
 * @brief Checks if a king can move or capture.
 * @param fromX The X coordinates (number of lines) of the knight.
 * @param fromY The Y coordinates (number of columns) of the knight.
 * @param toX The X coordinates (number of lines) of the location where the knight should be moved.
 * @param toY The Y coordinates (number of columns) of the location where the knight should be moved.
 * @return INVALID_MOVE if the move isn't possible, VALID_MOVE otherwise.
 */
bool __CheckValidMove_King__(const u16Vect2 from, const u16Vect2 to);

bool Piece_CheckCastling(playerPiece_t** restrict mainBoard, const u16Vect2 from, const u16Vect2 to);
void Piece_ToggleSpecialMove(playerPiece_t*** board_ptr, const u16Vect2 from, const u16Vect2 to);
void Piece_MoveCastling(playerPiece_t*** restrict board_ptr, const u16Vect2 from, const u16Vect2 to);
bool Piece_CheckPawnPromotion(playerPiece_t** restrict board, const u16Vect2 from, const u16Vect2 to, const u16Vect2 boardSize);
int GenerateLegalMoves(playerPiece_t** restrict board, const u16Vect2 boardSize, __color_t currentColor, Move* outMoves);
int EvaluateBoard(playerPiece_t** board, u16Vect2 boardSize, __color_t maximizingColor);
int Minimax(playerPiece_t** board, u16Vect2 boardSize, __color_t currentColor, int depth, int alpha, int beta, bool maximizingPlayer, __color_t rootColor);
void Board_ApplyMove(playerPiece_t** board, Move move);
playerPiece_t** Board_Clone(playerPiece_t** src, u16Vect2 boardSize);
Move FindBestMove(playerPiece_t** board, u16Vect2 boardSize, __color_t currentColor, int depth);
bool IsValidMove(Move move);

#endif // __PIECEMOVES_H__