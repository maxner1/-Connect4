#include<iostream>
#include<string>
#include "board.h"
#include "connect4.h"
#include "ai.h"

using namespace std;

int main() {
    cout << "Press 1 for terminal and 2 for graphics: ";
    int choice;
    while (!(cin >> choice)) {
        cin.clear();
        string garbage;
        getline(cin, garbage);
        cout << "Press 1 for terminal and 2 for graphics: ";
    }
    if (choice == 1) {
        terminalPlay();
    } else {
        cout << "Under construction..." << endl;
        //graphicsPlay(argc, argv);
    }
    return 0;
}
