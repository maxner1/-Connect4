#include "connect4.h"
#include "ai.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

void terminalPlay() {
    string p1_name, p2_name;
    Board board = getBoard(p1_name, p2_name);
    board.prettyPrintBoard(cout);
    
    if (p2_name != "AI") {
        string choice;
        bool condition = true;
        while (condition) {
            cout << "\nEnter: the column to move, q to quit, s to save" << endl;
            if (board.toMove() == 1) {
                cout << p1_name << " enter your move: ";
                cin >> choice;
            }
            else {
                cout << p2_name << " enter your move: ";
                cin >> choice;
            }
            
            if (choice == "q" || choice == "Q") {
                cout << "Thanks for playing!" << endl;
                condition = false;
            }
            else if (choice == "s" || choice == "S") {
                saveBoard(p1_name, p2_name, board);
                board.prettyPrintBoard(cout);
            }
            else if ((choice.length() == 1) &&
                     (choice[0] - 48) >= 1 && (choice[0] - 48) <= NUM_COLS) {
                Result result;
                result = board.makeMove(choice[0] - 49);
                if (result == Win) {
                    if (board.toMove() == 1)
                        cout << "Congratulations " << p2_name
                        << "! You won!" << endl;
                    else
                        cout << "Congratulations " << p1_name
                        << "! You won!" << endl;
                    condition = false;
                    board.prettyPrintBoard(cout);
                }
                else if (result == Draw) {
                    cout << "Draw!" << endl;
                    condition = false;
                    board.prettyPrintBoard(cout);
                }
                else if (result == IllegalMove) {
                    cout << "ILLEGAL MOVE: Try again" << endl;
                    board.prettyPrintBoard(cout);
                }
                else {
                    board.prettyPrintBoard(cout);
                }
            }
            else {
                cout << "ILLEGAL MOVE: Try again" << endl;
                board.prettyPrintBoard(cout);
            }
        }
    }
    else {
        string choice = "1";
        int ai_choice = 0;
        bool condition = true;
        while (condition) {
            cout << "\nEnter: the column to move, q to quit, s to save" << endl;
            if (board.toMove() == 1) {
                cout << p1_name << " enter your move: ";
                cin >> choice;
            }
            else {
                ai_choice = connect4AI(board);
                //cout << ai_choice;
            }
            
            if (choice == "q" || choice == "Q") {
                cout << "Thanks for playing!" << endl;
                condition = false;
            }
            else if (choice == "s" || choice == "S") {
                saveBoard(p1_name, p2_name, board);
                board.prettyPrintBoard(cout);
            }
            else if ((board.toMove() == 1 && ((choice.length() == 1) &&
                     (choice[0] - 48) >= 1 && (choice[0] - 48) <= NUM_COLS)) ||
                     (board.toMove() == 2)){
                Result result;
                if (board.toMove() == 1)
                    result = board.makeMove(choice[0] - 49);
                else
                    result = board.makeMove(ai_choice);
                if (result == Win) {
                    if (board.toMove() == 2)
                        cout << "Congratulations " << p1_name
                        << "! You won!" << endl;
                    else
                        cout << "You lost. Thanks for playing!" << endl;
                    condition = false;
                    board.prettyPrintBoard(cout);
                }
                else if (result == Draw) {
                    cout << "Draw!" << endl;
                    condition = false;
                    board.prettyPrintBoard(cout);
                }
                else if (result == IllegalMove) {
                    cout << "ILLEGAL MOVE: Try again" << endl;
                    board.prettyPrintBoard(cout);
                }
                else {
                    board.prettyPrintBoard(cout);
                }
            }
            else {
                cout << "ILLEGAL MOVE: Try again" << endl;
                board.prettyPrintBoard(cout);
            }
        }
    }
}



Board getBoard(string &p1, string &p2) {
    cout << "Artificial Intelligence Mode" << endl;
    cout << "Enter L to load a game or P to play: ";
    string choice;
    string garbage;
    getline(cin, garbage);
    getline(cin, choice);
//    if (choice == "P" || choice == "p") {
//        cout << "Player 1, enter your name: " << flush;
//        getline(cin, p1);
//        cout << "Player 2, enter your name: " << flush;
//        getline(cin, p2);
//        Board board;
//        return board;
//    }
    if (choice == "P" || choice == "p") {
        cout << "Enter your name: " << flush;
        getline(cin, p1);
        p2 = "AI";
        Board board;
        return board;
    }
    else {
        cout << "Enter the filename: " << flush;
        ifstream ins;
        string filename;
        cin >> filename;
        ins.open(filename);
        if (ins.good()) {
            getline(ins, p1);
            getline(ins, p2);
            string fenString;
            getline(ins, fenString);
            Board board = Board(fenString);
            return board;
        }
        else {
            Board board;
            return board;
        }
    }
}

void saveBoard(string p1, string p2, const Board &board) {
    cout << "Enter the filename: " << flush;
    ofstream outs;
    string filename;
    cin >> filename;
    outs.open(filename);
    outs << p1 << "\n" << p2 << "\n";
    board.printBoard_as_FENstring(outs);
    outs.close();
}
