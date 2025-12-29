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

int main()
{
    cout << "Hello 2048!" << endl;
    return 0;
}