#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <utility>
#include <sstream>

using namespace std;

int SIZE;
int board[5][5];
int prevBoard[5][5];
bool canUndo = false;
int bestScore = 0;

void copyBoard(int src[5][5], int dest[5][5]) {
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

void loadBestScore(int SIZE) {
    ifstream file("bestscore.txt");
    if(!file) { bestScore=0; return; }

    string line;
    bestScore =0;
    while(getline(file,line)) {
        istringstream iss(line);
        int s, score;
        if(!(iss >> s >> score)) continue;
        if(s==SIZE) { bestScore = score; break; }
    }
    file.close();
}

void saveBestScore(int SIZE, int currentScore) {
    ifstream file("bestscore.txt");
    vector<pair<int,int>> scores;
    bool updated = false;

    if(file) {
        int s, score;
        while(file >> s >> score) {
            if(s==SIZE) {
                if(currentScore>score) score=currentScore;
                updated = true;
            }
            scores.push_back({s,score});
        }
    }
    file.close();

    if(!updated) scores.push_back({SIZE,currentScore});

    ofstream outfile("bestscore.txt");
    for(auto &p:scores) outfile << p.first << " " << p.second << "\n";
    outfile.close();
}

void saveGame() { 
    ofstream gameFile("savegame.txt"); 
    for(int i = 0; i < SIZE; i++) { 
        for(int j = 0; j < SIZE; j++) { 
            gameFile << board[i][j] << "\t"; 
        } 
        gameFile << "\n"; 
    } 
    gameFile.close(); 
    cout << "Game saved!"; 
} 
 
void loadGame() { 
    ifstream gameFile("savegame.txt"); 
    if(!gameFile) { 
        cout << "No saved game!\n"; 
        return; 
    } 
    for(int i = 0; i < SIZE; i++) { 
        for(int j = 0; j < SIZE; j++) { 
            gameFile >> board[i][j]; 
        } 
    } 
    gameFile.close(); 
    cout << "Game loaded!\n"; 
}

int main()
{
    do {
        cout << "Choose board size (4 or 5): ";
        cin >> SIZE;

        if (SIZE != 4 && SIZE != 5) {
            cout << "Invalid input! Please enter 4 or 5.\n";
        }

    } while (SIZE != 4 && SIZE != 5);

    loadBestScore(SIZE);

    srand(time(0));
    initBoard();
    addRandomTile();
    
    while (true) {
        printBoard();
        cout << "Best Score:" << bestScore << endl;

        if (checkWin()) {
            cout << "You win!" << endl;
            if (calculateScore() > bestScore) {
                bestScore = calculateScore();
                saveBestScore(SIZE, bestScore);
            }
            break;
        }
        if (!canMove()) {
            cout << "Game Over!" << endl;
             if (calculateScore() > bestScore) {
                bestScore = calculateScore();
                saveBestScore(SIZE, bestScore);
            }
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
        int currentScore = calculateScore();
        if (currentScore > bestScore) {
            bestScore = currentScore;
            saveBestScore(SIZE, bestScore);
        }
    }

    cout << "Final Score: " << calculateScore() << endl;
    cout << "Best Score: " << bestScore << endl;

    return 0;
}