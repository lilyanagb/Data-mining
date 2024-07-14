#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

void print(vector<vector<int>> board, int N){
    for (int col = 0; col < N; col++) {
        for (int row = 0; row < N; row++) {
            if (board[col][row] == 1) {
                cout << "* ";
            } else {
                cout << "_ ";
            }
        }
        cout << endl;
    }
    // std::cout << "[";
    // for (int col = 0; col < N; col++) {
        
    //     for (int row = 0; row < N; row++) {
    //         if (board[col][row] == 1) {
    //             if(col < N-1) std::cout << row << ", ";
    //             else std::cout << row;
    //         }
    //     }
        
    // }
    // std::cout << "]" << std::endl;
}

void init(vector<vector<int>>& board, int N){
    for (int col = 0; col < N; col++) {
        board[col][0] = 1;
    }

    srand(time(NULL));

    for (int col = 0; col < N; col++) {
        for (int i = 0; i < N; i++) {
            int j = rand() % N;
            swap(board[col][i], board[col][j]);
        }
    }
}

int countConflicts(vector<vector<int>> board, int row, int col, int size) {
    int conflicts = 0;

    for (int i = 0; i < size; i++) {
        if (i != row && board[col][i] == 1) {
            conflicts++;
        }

        if (i != col && board[i][row] == 1) {
            conflicts++;
        }
    }

    for (int i = row, j = col; i < size && j < size; i++, j++) {
        if (i != row && j != col && board[j][i] == 1) {
            conflicts++;
        }
    }

    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
        if (i != row && j != col && board[j][i] == 1) {
            conflicts++;
        }
    }

    for (int i = row, j = col; i < size && j >= 0; i++, j--) {
        if (i != row && j != col && board[j][i] == 1) {
            conflicts++;
        }
    }

    for (int i = row, j = col; i >= 0 && j < size; i--, j++) {
        if (i != row && j != col && board[j][i] == 1) {
            conflicts++;
        }
    }

    return conflicts;
}

int getColWithQueenWithMaxConf(vector<vector<int>> board, int size){
    int maxConflicts = 0;
    vector<int> candidates;

    for (int col = 0; col < size; col++) {
        for (int row = 0; row < size; row++) {
            if (board[col][row] == 1) {
                int currentConflicts = countConflicts(board, row, col, size);
                
                if(currentConflicts == 0) break;

                if (currentConflicts > maxConflicts) {
                    maxConflicts = currentConflicts;
                    candidates.clear(); 
                    candidates.push_back(col);
                } else if (currentConflicts == maxConflicts) {
                    candidates.push_back(col);
                }

                break;
            }
        }
    }

    if (!candidates.empty()) {
        int randIndex = rand() % candidates.size();
        return candidates[randIndex];
    }

    return -1;
}

int getRowWithMinConflicts(vector<vector<int>> board, int col, int size){
   int minConflicts = INT_MAX;
    vector<int> candidates;

    for (int row = 0; row < size; row++) {
        if (board[col][row] != 1) {
            int currentConflicts = countConflicts(board, row, col, size) - 1;
            
            if (currentConflicts < minConflicts) {
                minConflicts = currentConflicts;
                candidates.clear(); 
                candidates.push_back(row);
            } else if (currentConflicts == minConflicts) {
                candidates.push_back(row);
            }
        }
    }

    if (candidates.size() > 1) {
        int randIndex = rand() % candidates.size();
        return candidates[randIndex];
    }

    return candidates[0];
}

void solve(vector<vector<int>>& board, int N){
    int iter = 0;
    int k = 1000;
    int col, row;

    init(board, N);

    while(iter++ <= k*N){
        col = getColWithQueenWithMaxConf(board, N);
        
        if(col == -1){
            print(board, N);
            return;
        }

        row = getRowWithMinConflicts(board, col, N);
        
        for (int i = 0; i < N; i++) {
            board[col][i] = 0;
        }
        board[col][row] = 1;
    }
}

int main(){
    int N; 
    cin >> N;

    if(N == 2 || N == 3){
        cout << -1 << endl;
    }

    vector<vector<int>> board(N, vector<int>(N));

    clock_t start = clock();

    solve(board, N);

    clock_t end = clock();
    double duration = (end - start) / (double) CLOCKS_PER_SEC;
    printf("Execution time: %f secs.\n", duration);

    return 0;
}