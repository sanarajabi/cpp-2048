#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <utility>
#include <sstream>
#include <iomanip>

using namespace std;

int SIZE;
int board[5][5];
int prevBoard[5][5];
bool canUndo = false;
int bestScore = 0;

void printHeader() {
    system("cls");
    
    cout << "\033[1;36m";
    cout << "╔══════════════════════════════════════╗" << endl;
    cout << "║                                      ║" << endl;
    cout << "║  ██████╗   ██████╗  ██╗  ██╗ █████╗  ║" << endl;
    cout << "║  ╚════██╗ ██╔═══██╗ ██║  ██║██╔══██╗ ║" << endl;
    cout << "║   █████╔╝ ██║   ██║ ███████║╚█████╔╝ ║" << endl;
    cout << "║  ██╔═══╝  ██║   ██║ ╚════██║██╔══██╗ ║" << endl;
    cout << "║  ███████╗ ╚██████╔╝      ██║╚█████╔╝ ║" << endl;
    cout << "║  ╚══════╝  ╚═════╝       ╚═╝ ╚════╝  ║" << endl;
    cout << "║                                      ║" << endl;
    cout << "║               2  0  4  8             ║" << endl;
    cout << "╚══════════════════════════════════════╝" << endl;
    cout << "\033[0m";
}

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

int calculateScore() {
    int score = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            score += board[i][j];
        }
    }
    return score;
}

void printCentered(int num, int width = 6) {    string numStr = to_string(num);
    int numLen = numStr.length();
    int leftSpaces = (width - numLen) / 2;
    int rightSpaces = width - numLen - leftSpaces;
    
    for(int i = 0; i < leftSpaces; i++) cout << " ";
    cout << numStr;
    for(int i = 0; i < rightSpaces; i++) cout << " ";
}

void printBoard() {
    cout << " .--------------------------." << endl;
    cout << " | SCORE: " << left << setw(17) << calculateScore() << " |" << endl;
    cout << " | BEST:  " << left << setw(17) << bestScore << " |" << endl;
    cout << " '--------------------------'" << endl << endl;

    const char* HORIZ = "──────";
    
    cout << "┌";
    for (int j = 0; j < SIZE; j++) {
        cout << HORIZ;
        if (j < SIZE - 1) cout << "┬";
    }
    cout << "┐" << endl;
    
    for (int i = 0; i < SIZE; i++) {
        cout << "│";
        
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                cout << "      │";
            } else {
                if (board[i][j] == 2) cout << "\033[1;33m";
                else if (board[i][j] == 4) cout << "\033[1;32m";
                else if (board[i][j] == 8) cout << "\033[1;36m";
                else if (board[i][j] == 16) cout << "\033[1;34m";
                else if (board[i][j] == 32) cout << "\033[1;35m";
                else if (board[i][j] == 64) cout << "\033[1;31m";
                else if (board[i][j] == 128) cout << "\033[1;33m";
                else if (board[i][j] == 256) cout << "\033[1;32m";
                else if (board[i][j] == 512) cout << "\033[1;36m";
                else if (board[i][j] == 1024) cout << "\033[1;35m";
                else if (board[i][j] == 2048) cout << "\033[1;31m";
                else cout << "\033[1;37m";
                
                printCentered(board[i][j], 6);
                
                cout << "\033[0m" << "│";
            }
        }
        cout << endl;
        
        if (i < SIZE - 1) {
            cout << "├";
            for (int j = 0; j < SIZE; j++) {
                cout << HORIZ;
                if (j < SIZE - 1) cout << "┼";
            }
            cout << "┤" << endl;
        }
    }
    
    cout << "└";
    for (int j = 0; j < SIZE; j++) {
        cout << HORIZ;
        if (j < SIZE - 1) cout << "┴";
    }
    cout << "┘" << endl;
    
    cout << "\n (W)Up (S)Down (A)Left (D)Right | (U)Undo | (Q)Quit: " << endl;
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
    gameFile << SIZE << "\n";
    gameFile << bestScore << "\n";
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
    gameFile >> SIZE;
    gameFile >> bestScore;
    for(int i = 0; i < SIZE; i++) { 
        for(int j = 0; j < SIZE; j++) { 
            gameFile >> board[i][j]; 
        } 
    } 
    gameFile.close(); 
    cout << "Game loaded!\n"; 
}

void playGame(bool isNew) {
    if (isNew) {
        initBoard();
        addRandomTile();
        addRandomTile();
    }
    else {
        loadGame();
    }

    while (true) {
        printHeader();
        printBoard();

        if (!canMove()) {
            cout << "\n!!! GAME OVER !!!" << endl;
            if (calculateScore() > bestScore) {
                bestScore = calculateScore();
                saveBestScore(SIZE, bestScore);
            }
            cout << "Press any key to return to menu...";
            char wait; cin >> wait;
            return; 
        }

        char command;
        cin >> command;

        if (command == 'q' || command == 'Q') {
            cout << "Save game before returning to menu? (y/n): ";
            char saveChoice; cin >> saveChoice;
            if (saveChoice == 'y') saveGame();
            return; 
        }

        if (command == 'u' || command == 'U') {
            undo();
            continue;
        }

        copyBoard(board, prevBoard);
        canUndo = true;

        int tempBoard[5][5];
        copyBoard(board, tempBoard);

        if (command == 'a' || command == 'A') moveLeft();
        else if (command == 'd' || command == 'D') moveRight();
        else if (command == 'w' || command == 'W') moveUp();
        else if (command == 's' || command == 'S') moveDown();
        else continue;

        bool changed = false;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (tempBoard[i][j] != board[i][j]) {
                    changed = true;
                    break;
                }
            }
        }

        if (changed) {
            addRandomTile();
            int currentScore = calculateScore();
            if (currentScore > bestScore) {
                bestScore = currentScore;
                saveBestScore(SIZE, bestScore);
            }
        }
    }
}

int main()
{

    system("chcp 65001 > nul");
    srand(time(0));
    
    while (true) {
        
        printHeader(); 
        cout << "\n  1. New Game" << endl;
        cout << "  2. Continue Previous Game" << endl;
        cout << "  3. Exit" << endl;
        cout << "\n  Choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) { 
            printHeader(); 
            do {
                cout << "\n  Choose board size (4 or 5): ";
                cin >> SIZE;

                if (SIZE != 4 && SIZE != 5) {
                    cout << "  Invalid input! Please enter 4 or 5.\n";
                }
            } while (SIZE != 4 && SIZE != 5);

            loadBestScore(SIZE); 
            playGame(true);
        } 
        else if (choice == 2) {
            playGame(false);
        } 
        else if (choice == 3) {
            break; 
        }
    }
    return 0;
}