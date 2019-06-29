#include "board.h"
#include <iostream>

Board::Board() {
    nextPlayer_to_move = Player1;
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            data[i][j] = Empty;
        }
    }
}

Board::Board(const string &fen) {
    for (int i = 0; i <= fen.length(); i++) {
        if (fen[i] == ' ') {
            if (fen[i + 1] == 'x') {
                nextPlayer_to_move = Player1;
            }
            else {
                nextPlayer_to_move = Player2;
            }
        }
    }
    
    int spot = 0;
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            if (fen[spot] == 'x') {
                data[i][j] = Player1;
                spot++;
            }
            else if (fen[spot] == 'o') {
                data[i][j] = Player2;
                spot++;
            }
            else if (fen[spot] != '/') {
                int num = fen[spot] - 48;
                for (int k = 0; k < num; k++) {
                    data[i][j] = Empty;
                    j++;
                }
                j--;
                spot++;
            }
        }
        spot++;
    }
}

void Board::printBoard_as_FENstring(ostream &os) const {
    int empties = 0;
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            if (data[i][j] == Empty)
                empties++;
            else if (data[i][j] == Player1 || data[i][j] == Player2) {
                if (empties > 0) {
                    os << empties;
                    empties = 0;
                }
                if (data[i][j] == Player1)
                    os << "x";
                else
                    os << "o";
            }
        }
        if (empties > 0) {
            os << empties;
            empties = 0;
        }
        if (i < NUM_ROWS - 1)
            os << "/";
    }
    os << " ";
    if (nextPlayer_to_move == Player1) {
        os << 'x';
    }
    else {
        os << 'o';
    }
}


PieceType Board::atLocation(int row, int col) {
    return data[row][col];
}

void Board::prettyPrintBoard(ostream &os) const {
    os << endl;
    for (int row = NUM_ROWS - 1; row >= 0; row--)  {
        os << "     +---+---+---+---+---+---+---+"
        << endl
        << "    ";
        
        for (int col = 0; col < NUM_COLS; col++)   {
            os << " | " ;
            if ( data[row][col] == Player1)
                os <<  PLAYER1_TOKEN;
            else if (data[row][col] == Player2)
                os << PLAYER2_TOKEN;
            else
                os << EMPTY_TOKEN;
        }
        os << " |" << endl;
    }
    os << "     +---+---+---+---+---+---+---+" << endl
    <<"  col  1   2   3   4   5   6   7" << endl;
    return;
}

int Board::toMove() const {
    return nextPlayer_to_move;
}

Result Board::makeMove(int col) {
    
    //if move is out of bound or on top of another, it is an illegal move
    if ((!inBounds(getFirstFreeRow(col), col) || getFirstFreeRow(col) == NUM_ROWS)) {
        return IllegalMove;
    }
    // next, make the move
    data[getFirstFreeRow(col)][col] = nextPlayer_to_move;
    // if that move was a win, return Win
    if (isWin(getFirstFreeRow(col) - 1, col)) {
        updateToMove();
        return Win;
    }
    // if the board was full and no one won, return draw
    else if (isBoardFull()) {
        updateToMove();
        return Draw;
    }
    // if no result occurs, update to move
    else {
        updateToMove();
        return NoResult;
    }
}

int Board::getFirstFreeRow(int col) const {
    int smallest = 0;
    for (int row = 0; row < NUM_ROWS; row++) {
        if (data[row][col] == Empty) {
            smallest = row;
            return smallest;
        }
    }
    return NUM_ROWS;
}



PieceType Board::updateToMove() {
    // Sam edited this one
    if (nextPlayer_to_move == Player1) {
        nextPlayer_to_move = Player2;
        return Player1;
    }
    else {
        nextPlayer_to_move = Player1;
        return Player2;
    }
}

bool Board::isBoardFull() const {
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            if (data[i][j] == Empty)
                return false;
        }
    }
    return true;
}


bool Board::inBounds(int row, int col) const {
    //Hope edited this one
    // if row is less than or equal to 6 and col is less than or equal to 7 -- in bounds
    if ((row >= 0) && (col >= 0) && (row < NUM_ROWS) && (col < NUM_COLS)) {
        return true;
        
    }
    // if both not true, return false
    return false;
}

int Board::piecesInDirection(int row, int col, int dRow, int dCol) const {
    int consec_count = 0;
    int updater = 0;
    bool value = true; // setting necessary variable to check for matches
    int changeRow = dRow;
    int changeCol = dCol;
    while (value){
        if (data[row][col] == data[row + changeRow][col + changeCol] && inBounds(row + changeRow, col + changeCol)){
            // checks for consecutve pieces based on the values given
            updater++;
            consec_count++;
            //sets up the next check
            changeRow = (updater + 1) * dRow;
            changeCol = (updater + 1) * dCol;
        }
        else{
            //if next piece is not a match, this will end the loop
            return consec_count;
        }
    }
    return consec_count;
}

bool Board::isWin(int row, int col) const {
    int consec_pieces = 0;
    int count[8] = {};
    count[0] = piecesInDirection(row, col, 1, 0);
    count[1] = piecesInDirection(row, col, -1, 0);
    count[2] = piecesInDirection(row, col, -1, 1);
    count[3] = piecesInDirection(row, col, 1, -1);
    count[4] = piecesInDirection(row, col, -1, -1);
    count[5] = piecesInDirection(row, col, 1, 1);
    count[6] = piecesInDirection(row, col, 0, 1);
    count[7] = piecesInDirection(row, col, 0, -1);
    //all the different "directional values"
    for (int k = 0; k < 8; k = k + 2){
        consec_pieces = (count[k] + count[k + 1]);
        //adds up the two "directions" 1 pos/ 1 neg
        if (consec_pieces >= 3){
            return true;
        }
    }
    return false;
}
