#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SIZE = 4;
int board[SIZE][SIZE];
int prevBoard[SIZE][SIZE];
bool canUndo = false;

void copyBoard(int src[SIZE][SIZE], int dest[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

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

void moveRight() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = SIZE - 2; j>=0; j--) //last column cannot move right
        {
            if (board[i][j] == 0) continue;
            int s = j;
            while (s < SIZE - 1 && board[i][s + 1] == 0) {
                board[i][s + 1] = board[i][s];
                board[i][s] = 0;
                s++;
            }
            if (s < SIZE - 1 && board[i][s + 1] == board[i][s]) {
                board[i][s + 1] *= 2;
                board[i][s] = 0;
            }
        }
    }
}

void moveUp() {
    for (int i = 0; i < SIZE; i++) // the first row cannot move upside
    {
        for (int j = 1; j < SIZE; j++) {
            if (board[j][i] == 0) continue;
            int s = j;
            while (s > 0 && board[s - 1][i] == 0) {
                board[s - 1][i] = board[s][i];
                board[s][i] = 0;
                s--;
            }
            if (s > 0 && board[s - 1][i] == board[s][i]) {
                board[s - 1][i] *= 2;
                board[s][i] = 0;
            }
        }
    }
}

void moveDown() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = SIZE - 2; j >= 0; j--) {
            if (board[j][i] == 0) continue;
            int s = j;
            while (s < SIZE - 1 && board[s + 1][i] == 0) {
                board[s + 1][i] = board[s][i];
                board[s][i] = 0;
                s++;
            }
            if (s < SIZE - 1 && board[s + 1][i] == board[s][i]) {
                board[s + 1][i] *= 2;
                board[s][i] = 0;
            }


        }
    }
}

bool checkWin() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 2048) {
                return true;
            }
        }
    }
    return false;
}

bool canMove() {
    // Check for empty cells
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) return true;
        }
    }
    // Check for horizontally mergeable neighboring cells
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE - 1; j++) {
            if (board[i][j] == board[i][j + 1]) return true;
        }
    }
    // Check for vertically mergeable neighboring cells
    for (int j = 0; j < SIZE; j++) {
        for (int i = 0; i < SIZE - 1; i++) {
            if (board[i][j] == board[i + 1][j]) return true;
        }
    }
    return false; // No moves or merges possible
}

int calculateScore() {
    int score = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            score += board[i][j];
        }
    }
    return score;
}

void undo() {
    if (!canUndo) {
        cout << "Undo is not possible!" << endl;
        return;
    }

    copyBoard(prevBoard, board);
    canUndo = false; // only one chance to undo
}

int main()
{
    srand(time(0));
    initBoard();
    addRandomTile();
    while (true) {
        printBoard();
        if (checkWin()) {
            cout << "You win!" << endl;
            break;
        }
        if(!canMove()) {
            cout << "Game Over!" << endl;
            break;
        }
        char command;
        cin >> command;

        int oldBoard[SIZE][SIZE]; //to check if the command is valid
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                oldBoard[i][j] = board[i][j];
            }
        }

        if (command == 'q' || command == 'Q') break;
        else if (command == 'n' || command == 'N') {
            initBoard();
            addRandomTile();
            canUndo = false;
            continue;
        }
       else if (command == 'u' || command == 'U') {
            undo();
            continue;
        }
        copyBoard(board, prevBoard);

        int tempBoard[SIZE][SIZE]; //to check valid movement
        copyBoard(board, tempBoard);

        if (command == 'a' || command == 'A') {
            moveLeft();
        }
        else if (command == 'd' || command == 'D') {
            moveRight();
        }
        else if (command == 'w' || command == 'W') {
            moveUp();
        }
        else if (command == 's' || command == 'S') {
            moveDown();
        }
        else {
            continue;
        }

        bool validMovement = false;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (oldBoard[i][j] != board[i][j]) {
                    validMovement = true;
                    break;
                }
            }
            if (validMovement) {
                break;
            }
        }
        if (validMovement) {
            canUndo = true;
            addRandomTile();
        }
    }

    cout << "Final Score: " << calculateScore() << endl;

    return 0;
}