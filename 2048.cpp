#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SIZE = 4;
int board[SIZE][SIZE];

void initBoard() {
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            board[i][j] = 0;
        }
    }
}

void printBoard() {
    //print upside the board if it is needed
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            if (board[i][j] == 0)
            cout << ".\t";
            else
            cout << board[i][j] << "\t";
        }
        cout << endl;
    }
    //downside if it is needed
}

void addRandomTile() {
    int emptyCells[SIZE * SIZE][2];
    int countempty = 0;

    for (int i = 0; i < SIZE; i++) {
       for (int j = 0; j < SIZE; j++) {
          if (board[i][j] == 0) {
            emptyCells[countempty][0] = i;
            emptyCells[countempty][1] = j;
            countempty++;
        }
       }
    }
    if (countempty == 0) return;
    int random = rand() % countempty;
    int rvalue = (rand() % 2 == 0) ? 2 : 4;
    board[emptyCells[random][0]][emptyCells[random][1]] = rvalue;
}

void moveLeft() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 1; j < SIZE; j++) //first column cannot move left 
        {
            if (board[i][j] == 0) continue;
            int s = j;
            while (s > 0 && board[i][s - 1] == 0) {
                board[i][s - 1] = board[i][s];
                board[i][s] = 0;
                s--;
            }
            if (s > 0 && board[i][s - 1] == board[i][s]) {
                board[i][s - 1] *= 2;
                board[i][s] = 0;
            }
        }
    }
}

int main()
{
    srand((time(0)));
    initBoard();
    addRandomTile();
   //printBoard();
    while (true) {
        printBoard();
        char command;
        cin >> command;
        if (command == 'q' || command == 'Q') break;
        if (command == 'n' || command == 'N') {
            initBoard();
            addRandomTile();
            addRandomTile();
            continue;
        }
        if (command == 'a' || command == 'A') {
            moveLeft();
            addRandomTile();
        }
    }

    return 0;
}