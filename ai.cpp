#include "ai.h"
#include "board.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

int connect4AI(const Board& board);
    //PieceType info[NUM_ROWS][NUM_COLS];
    
int check3InARowForWin(const Board& board);
int check3InARowForBlock(const Board& board);
int checkSpaceInMiddleForWin(const Board& board);
int checkSpaceInMiddleForBlock(const Board& board);
int anticipate2InARow(const Board& board);
int pieceCount(const Board& board);
int checkForDiagonalProblems(const Board& board);
int anticipate2inAColumnForAWin(const Board& board);
int anticipate2inAColumnForABlock(const Board& board);
string makeTempFenString(PieceType tempBoard[NUM_ROWS][NUM_COLS]);

int connect4AI(const Board& board) {
    clock_t t;
    t = clock();
    if (check3InARowForWin(board) != -1) {
        t = clock() - t;
        cout << "Took " << double(t) / CLOCKS_PER_SEC << " seconds" << endl;
        return check3InARowForWin(board);
    }
    if (checkSpaceInMiddleForWin(board) != -1) {
        t = clock() - t;
        cout << "Took " << double(t) / CLOCKS_PER_SEC << " seconds" << endl;
        return checkSpaceInMiddleForWin(board);
    }
    if (check3InARowForBlock(board) != -1) {
        t = clock() - t;
        cout << "Took " << double(t) / CLOCKS_PER_SEC << " seconds" << endl;
        return check3InARowForBlock(board);
    }
    if (checkSpaceInMiddleForBlock(board) != -1) {
        t = clock() - t;
        cout << "Took " << double(t) / CLOCKS_PER_SEC << " seconds" << endl;
        return checkSpaceInMiddleForBlock(board);
    }
    if (checkForDiagonalProblems(board) != -1) {
        t = clock() - t;
        cout << "Took " << double(t) / CLOCKS_PER_SEC << " seconds" << endl;
        return checkForDiagonalProblems(board);
    }
    if (anticipate2InARow(board) != -1) {
        t = clock() - t;
        cout << "Took " << double(t) / CLOCKS_PER_SEC << " seconds" << endl;
        return anticipate2InARow(board);
    }
    if (anticipate2inAColumnForAWin(board) != -1) {
        t = clock() - t;
        cout << "Took " << double(t) / CLOCKS_PER_SEC << " seconds" << endl;
        return anticipate2inAColumnForAWin(board);
    }
    if (anticipate2inAColumnForABlock(board) != -1) {
        t = clock() - t;
        cout << "Took " << double(t) / CLOCKS_PER_SEC << " seconds" << endl;
        return anticipate2inAColumnForABlock(board);
    }
    if (board.getFirstFreeRow(3) == 0){
        return 3;
    }
    else if (board.data[0][3] == Player1 && pieceCount(board) == 1){
        return 3;
    }
    else if (board.data[0][3] == Player2 && board.getFirstFreeRow(2) == 0 &&
             pieceCount(board) == 2) {
        return 2;
    }
    else if (board.data[0][3] == Player1 && board.getFirstFreeRow(4) == 0 &&
             pieceCount(board) == 2) {
        return 4;
    }
    for (int i = 0; i < NUM_COLS; i++) {
        if (board.getFirstFreeRow(i - 1) == 0 &&
            board.data[0][i] == Player1 && board.data[1][i] == Player2) {
            return i - 1;
        }
        if (board.getFirstFreeRow(i + 1) == 0 &&
            board.data[0][i] == Player1 && board.data[1][i] == Player2) {
            return i + 1;
        }
        if ((board.data[0][i - 1] == Player2 || board.data[0][i + 1] == Player2) &&
            board.data[0][i] == Player1 && board.data[1][i] == Player2 &&
            board.getFirstFreeRow(i) == 2)
            return i;
        if ((board.data[0][i - 1] == Player2) &&
            board.data[0][i] == Player1 && board.data[1][i] == Player2 &&
            board.data[2][i] == Player2 &&
            board.getFirstFreeRow(i - 1) == 2)
            return i - 1;
        if ((board.data[0][i + 1] == Player2) &&
            board.data[0][i] == Player1 && board.data[1][i] == Player2 &&
            board.data[2][i] == Player2 &&
            board.getFirstFreeRow(i + 1) == 2)
            return i + 1;
    }
    
    
    t = clock() - t;
    cout << "Took " << double(t) / CLOCKS_PER_SEC << " seconds" << endl;
    int potentialCol = 0;
    bool condition = true;
    while (condition) {
        potentialCol = rand() % NUM_COLS;
        PieceType tempBoard[NUM_ROWS][NUM_COLS];
        for (int row = 0; row < NUM_ROWS; row++) {
            for (int col = 0; col < NUM_COLS; col++) {
                tempBoard[row][col] = board.data[row][col];
            }
        }
        tempBoard[board.getFirstFreeRow(potentialCol)][potentialCol] = Player2;
        tempBoard[board.getFirstFreeRow(potentialCol) + 1][potentialCol] = Player1;
        Board tBoard(makeTempFenString(tempBoard));
        if (tBoard.isWin(board.getFirstFreeRow(potentialCol) + 1, potentialCol) == false)
            condition = false;
    }
    cout << "AI Choice is " << potentialCol << endl;
    return potentialCol;
}

int pieceCount(const Board& board) {
    int count = 0;
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            if (board.data[i][j] != Empty) {
                count++;
            }
        }
    }
    return count;
}

int check3InARowForWin(const Board& board) {
    int horizontal_consec_pieces = 0;
    int vertical_consec_pieces = 0;
    int diagonal1_consec_pieces = 0;
    int diagonal2_consec_pieces = 0;
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            if (board.data[row][col] == Player2) {
                vertical_consec_pieces = board.piecesInDirection(row, col, 1, 0);
                vertical_consec_pieces += board.piecesInDirection(row, col, -1, 0);
                diagonal1_consec_pieces = board.piecesInDirection(row, col, -1, 1);
                diagonal1_consec_pieces += board.piecesInDirection(row, col, 1, -1);
                diagonal2_consec_pieces = board.piecesInDirection(row, col, -1, -1);
                diagonal2_consec_pieces += board.piecesInDirection(row, col, 1, 1);
                horizontal_consec_pieces = board.piecesInDirection(row, col, 0, 1);
                horizontal_consec_pieces += board.piecesInDirection(row, col, 0, -1);
                //all the different "directional values"
                if (horizontal_consec_pieces >= 2) {
                    if (col >= 1 && board.data[row][col - 1] == Empty) {
                        if (board.getFirstFreeRow(col - 1) == row)
                            return col - 1;
                    }
                    else if ((board.data[row][col + 1] == board.data[row][col] &&
                              board.data[row][col + 2] == board.data[row][col]) &&
                             col <= 3 && board.data[row][col + 3] == Empty) {
                        if (board.getFirstFreeRow(col + 3) == row)
                            return col + 3;
                    }
                }
                if (vertical_consec_pieces >= 2) {
                    if (board.data[row - 1][col] != board.data[row][col] &&
                        row <= 2 && board.data[row + 3][col] == Empty)
                        return col;
                }
                if (diagonal1_consec_pieces >= 2) {
                    if ((row >= 1 && col <= 6) && board.data[row - 1][col + 1] == Empty) {
                        if (board.getFirstFreeRow(col + 1) == row - 1)
                            return col + 1;
                    }
                    else if ((board.data[row + 1][col - 1] == board.data[row][col] &&
                              board.data[row + 2][col - 2] == board.data[row][col]) &&
                             (row <= 2 && col >= 3) && board.data[row + 3][col - 3] == Empty) {
                        if (board.getFirstFreeRow(col - 3) == row + 3)
                            return col - 3;
                    }
                }
                if (diagonal2_consec_pieces >= 2) {
                    if ((row >= 1 && col >= 1) && board.data[row - 1][col - 1] == Empty) {
                        if (board.getFirstFreeRow(col - 1) == row - 1)
                            return col - 1;
                    }
                    else if ((board.data[row + 1][col + 1] == board.data[row][col] &&
                              board.data[row + 2][col + 2] == board.data[row][col]) &&
                             (row <= 2 && col <= 3) && board.data[row + 3][col + 3] == Empty) {
                        if (board.getFirstFreeRow(col + 3) == row + 3)
                            return col + 3;
                    }
                }
            }
        }
    }
    return -1;
}
int check3InARowForBlock(const Board& board) {
    int horizontal_consec_pieces = 0;
    int vertical_consec_pieces = 0;
    int diagonal1_consec_pieces = 0;
    int diagonal2_consec_pieces = 0;
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            if (board.data[row][col] == Player1) {
                vertical_consec_pieces = board.piecesInDirection(row, col, 1, 0);
                vertical_consec_pieces += board.piecesInDirection(row, col, -1, 0);
                diagonal1_consec_pieces = board.piecesInDirection(row, col, -1, 1);
                diagonal1_consec_pieces += board.piecesInDirection(row, col, 1, -1);
                diagonal2_consec_pieces = board.piecesInDirection(row, col, -1, -1);
                diagonal2_consec_pieces += board.piecesInDirection(row, col, 1, 1);
                horizontal_consec_pieces = board.piecesInDirection(row, col, 0, 1);
                horizontal_consec_pieces += board.piecesInDirection(row, col, 0, -1);
                //all the different "directional values"
                if (horizontal_consec_pieces >= 2) {
                    if (col >= 1 && board.data[row][col - 1] == Empty) {
                        if (board.getFirstFreeRow(col - 1) == row)
                            return col - 1;
                    }
                    else if ((board.data[row][col + 1] == board.data[row][col] &&
                              board.data[row][col + 2] == board.data[row][col]) &&
                             col <= 3 && board.data[row][col + 3] == Empty) {
                        if (board.getFirstFreeRow(col + 3) == row)
                            return col + 3;
                    }
                }
                if (vertical_consec_pieces >= 2) {
                    if ((row >= 1 && ((board.data[row - 1][col] != board.data[row][col]) || row == 0))
                        && row <= 2 && board.data[row + 3][col] == Empty)
                        return col;
                }
                if (diagonal1_consec_pieces >= 2) {
                    if ((row >= 1 && col <= 6) && board.data[row - 1][col + 1] == Empty) {
                        if (board.getFirstFreeRow(col + 1) == row - 1)
                            return col + 1;
                    }
                    else if ((board.data[row + 1][col - 1] == board.data[row][col] &&
                              board.data[row + 2][col - 2] == board.data[row][col]) &&
                             (row <= 2 && col >= 3) && board.data[row + 3][col - 3] == Empty) {
                        if (board.getFirstFreeRow(col - 3) == row + 3)
                            return col - 3;
                    }
                }
                if (diagonal2_consec_pieces >= 2) {
                    if ((row >= 1 && col >= 1) && board.data[row - 1][col - 1] == Empty) {
                        if (board.getFirstFreeRow(col - 1) == row - 1)
                            return col - 1;
                    }
                    else if ((board.data[row + 1][col + 1] == board.data[row][col] &&
                              board.data[row + 2][col + 2] == board.data[row][col]) &&
                             (row <= 2 && col <= 3) && board.data[row + 3][col + 3] == Empty) {
                        if (board.getFirstFreeRow(col + 3) == row + 3)
                            return col + 3;
                    }
                }
            }
        }
    }
// int x = rand() % NUM_COLS;
// cout << "AI Choice is " << x << endl;
// return x;
// }

    return -1;
}

int checkSpaceInMiddleForWin(const Board& board) {
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            if (board.data[row][col] == Player2) {
                if (col <= 3) {
                    //oo_o
                    if (board.data[row][col + 1] == board.data[row][col] &&
                        board.data[row][col + 2] == Empty &&
                        board.data[row][col + 3] == board.data[row][col]) {
                        if (board.getFirstFreeRow(col + 2) == row)
                            return col + 2;
                    }
                    //o_oo
                    if (board.data[row][col + 1] == Empty &&
                        board.data[row][col + 2] == board.data[row][col] &&
                        board.data[row][col + 3] == board.data[row][col])
                        if (board.getFirstFreeRow(col + 1) == row)
                            return col + 1;
                }
            }
        }
    }
    return -1;
}

int checkForDiagonalProblems(const Board& board) {
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            if (row <= 3 && col >= 2) {
                if (board.data[row][col] != Empty &&
                    board.data[row][col] == board.data[row + 2][col - 2] &&
                    board.data[row + 1][col - 1] == Empty) {
                    return col - 1;
                }
            }
            if (row <= 3 && col <= 4) {
                if (board.data[row][col] != Empty &&
                    board.data[row][col] == board.data[row + 2][col + 2] &&
                    board.data[row + 1][col + 1] == Empty) {
                    return col + 1;
                }
            }
        }
    }
    return -1;
}

int checkSpaceInMiddleForBlock(const Board& board) {
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            if (board.data[row][col] == Player1) {
                if (col <= 3) {
                    //xx_x
                    if (board.data[row][col + 1] == board.data[row][col] &&
                        board.data[row][col + 2] == Empty &&
                        board.data[row][col + 3] == board.data[row][col]) {
                        if (board.getFirstFreeRow(col + 2) == row)
                            return col + 2;
                    }
                    //x_xx
                    if (board.data[row][col + 1] == Empty &&
                        board.data[row][col + 2] == board.data[row][col] &&
                        board.data[row][col + 3] == board.data[row][col])
                        if (board.getFirstFreeRow(col + 1) == row)
                            return col + 1;
                }
            }
        }
    }
    return -1;
}
int anticipate2InARow(const Board& board) {
    int potentialCol = -1;
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            if (col <= 3) {
                if (board.data[row][col] == Empty) {
                    //_xx__
                    if (board.data[row][col + 1] != Empty &&
                        board.data[row][col + 1] ==
                        board.data[row][col + 2] &&
                        board.data[row][col + 3] == Empty &&
                        board.data[row][col + 4] == Empty) {
                        if (board.getFirstFreeRow(col) == row)
                            potentialCol = col;
                    }
                    //__xx_
                    if (board.data[row][col + 2] != Empty &&
                        board.data[row][col + 1] == Empty &&
                        board.data[row][col + 2] ==
                        board.data[row][col + 3] &&
                        board.data[row][col + 4] == Empty) {
                        if (board.getFirstFreeRow(col + 1) == row)
                            potentialCol = col + 1;
                    }
                }
            }
        }
    }
    if (potentialCol != -1) {
        PieceType tempBoard[NUM_ROWS][NUM_COLS];
        for (int row = 0; row < NUM_ROWS; row++) {
            for (int col = 0; col < NUM_COLS; col++) {
                tempBoard[row][col] = board.data[row][col];
            }
        }
        tempBoard[board.getFirstFreeRow(potentialCol)][potentialCol] = Player2;
        tempBoard[board.getFirstFreeRow(potentialCol) + 1][potentialCol] = Player1;
        cout << makeTempFenString(tempBoard) << endl;
        Board tBoard(makeTempFenString(tempBoard));
        if (tBoard.isWin(board.getFirstFreeRow(potentialCol) + 1, potentialCol) == true)
            return -1;
        return potentialCol;
    }
    return potentialCol;
}

int anticipate2inAColumnForAWin(const Board& board) {
    int potentialCol = -1;
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            if (board.data[row][col] == Player2) {
                if (row <= 2) {
                    if (board.data[row + 1][col] == Player2 &&
                        board.data[row + 2][col] == Empty) {
                        potentialCol = col;
                    }
                }
            }
        }
    }
    if (potentialCol != -1) {
        PieceType tempBoard[NUM_ROWS][NUM_COLS];
        for (int row = 0; row < NUM_ROWS; row++) {
            for (int col = 0; col < NUM_COLS; col++) {
                tempBoard[row][col] = board.data[row][col];
            }
        }
        tempBoard[board.getFirstFreeRow(potentialCol)][potentialCol] = Player2;
        tempBoard[board.getFirstFreeRow(potentialCol) + 1][potentialCol] = Player1;
        Board tBoard(makeTempFenString(tempBoard));
        if (tBoard.isWin(board.getFirstFreeRow(potentialCol) + 1, potentialCol) == true)
            return -1;
        return potentialCol;
    }
    return potentialCol;
}

string makeTempFenString(PieceType tempBoard[NUM_ROWS][NUM_COLS]) {
    int empties = 0;
    string fenString = "";
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            if (tempBoard[i][j] == Empty)
                empties++;
            else if (tempBoard[i][j] == Player1 || tempBoard[i][j] == Player2) {
                if (empties > 0) {
                    fenString += empties + 48;
                    empties = 0;
                }
                if (tempBoard[i][j] == Player1)
                    fenString += "x";
                else
                    fenString += "o";
            }
        }
        if (empties > 0) {
            fenString += empties + 48;
            empties = 0;
        }
        if (i < NUM_ROWS - 1)
            fenString += "/";
    }
    fenString += " o";
    return fenString;
}

int anticipate2inAColumnForABlock(const Board& board) {
    int potentialCol = -1;
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            if (board.data[row][col] == Player1) {
                if (row <= 2) {
                    if (board.data[row + 1][col] == Player1 &&
                        board.data[row + 2][col] == Empty) {
                        potentialCol = col;
                    }
                }
            }
        }
    }
    if (potentialCol != -1) {
        PieceType tempBoard[NUM_ROWS][NUM_COLS];
        for (int row = 0; row < NUM_ROWS; row++) {
            for (int col = 0; col < NUM_COLS; col++) {
                tempBoard[row][col] = board.data[row][col];
            }
        }
        tempBoard[board.getFirstFreeRow(potentialCol)][potentialCol] = Player2;
        tempBoard[board.getFirstFreeRow(potentialCol) + 1][potentialCol] = Player1;
        Board tBoard(makeTempFenString(tempBoard));
        if (tBoard.isWin(board.getFirstFreeRow(potentialCol) + 1, potentialCol) == true)
            return -1;
        return potentialCol;
    }
    return potentialCol;
}

