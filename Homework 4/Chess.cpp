#include <iostream>
#include <vector>

using namespace std;

const char PLAYER_X = 'X';
const char PLAYER_O = 'O';

int maximizer(vector<vector<char>>& board, int alpha, int beta, int depth);
int minimizer(vector<vector<char>>& board, int alpha, int beta, int depth);

int maxScore = 10;
int minScore = -10;

bool isMovePossible(const vector<vector<char>>& board) {
    for (int col = 0; col < board.size(); col++) {
        for (int row = 0; row < board.size(); row++) {
            if (board[col][row] == '-') {
                return true;
            }
        }
    }

    return false;
}

void print(const vector<vector<char>>& board) {
    for (int col = 0; col < board.size(); col++) {
        cout << "|";
        for (int row = 0; row < board.size(); row++) {
            cout << board[col][row] << "|";
        }
        cout << endl;
    }
}

void makeMove(vector<vector<char>>& board, int row, int col, char player){
    if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == '-'){
        board[row][col] = player;
    }
}

int evaluate(const vector<vector<char>>& board, int depth) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == PLAYER_X) {
                //return -1;
                return maxScore - depth;
            } else if (board[i][0] == PLAYER_O) {
                //return 1;
                return minScore + depth;
            }
        }

        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == PLAYER_X) {
                return maxScore - depth;
            } else if (board[0][i] == PLAYER_O) {
                return minScore + depth;
            }
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == PLAYER_X) {
            return maxScore - depth;
        } else if (board[0][0] == PLAYER_O) {
            return minScore + depth;
        }
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == PLAYER_X) {
            return maxScore - depth;
        } else if (board[0][2] == PLAYER_O) {
            return minScore + depth;
        }
    }

    return 0;
}

int maximizer(vector<vector<char>>& board, int alpha, int beta, int depth) {
    int score = evaluate(board, depth);

    if (score != 0) {
        return score;
    }

    if (!isMovePossible(board)) {
        return 0;
    }

    int v = INT_MIN;

    for (int col = 0; col < 3; col++) {
        for (int row = 0; row < 3; row++) {
            if (board[col][row] == '-') {
                //board[col][row] = PLAYER_O;
                board[col][row] = PLAYER_X;
                v = max(v, minimizer(board, alpha, beta, depth + 1));
                board[col][row] = '-';

                if (v >= beta)
                    return v;

                alpha = max(alpha, v);
            }
        }
    }

    return v;
}

int minimizer(vector<vector<char>>& board, int alpha, int beta, int depth) {
    int score = evaluate(board, depth);

    if (score != 0) {
        return score;
    }

    if (!isMovePossible(board)) {
        return 0;
    }

    int v = INT_MAX;

    for (int col = 0; col < 3; col++) {
        for (int row = 0; row < 3; row++) {
            if (board[col][row] == '-') {
                //board[col][row] = PLAYER_X;
                board[col][row] = PLAYER_O;
                v = min(v, maximizer(board, alpha, beta, depth + 1));
                board[col][row] = '-';

                if (v <= alpha)
                    return v;

                beta = min(beta, v);
            }
        }
    }

    return v;
}

void alphaBetaDecision(vector<vector<char>>& board) {
    int bestScore = INT_MIN;
    int bestMoveRow = -1;
    int bestMoveCol = -1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '-') {
                board[i][j] = PLAYER_X;
                int v = minimizer(board, INT_MIN, INT_MAX, 0);
                board[i][j] = '-';

                if (v > bestScore) {
                    bestMoveRow = i;
                    bestMoveCol = j;
                    bestScore = v;
                }
            }
        }
    }

    makeMove(board, bestMoveRow, bestMoveCol, PLAYER_X);
}

int main() {
    vector<vector<char>> board(3, vector<char>(3, '-'));

    print(board);

    char curr;
    cout << "Computer(1) or Player(2) is first? ";
    cin >> curr;

    if (curr == '1') {
        curr = PLAYER_X;
    } else {
        curr = PLAYER_O;
    }

    while (isMovePossible(board)) {
        if (curr == PLAYER_X) {
            alphaBetaDecision(board);
            curr = PLAYER_O;
        } else {
            int playerRow, playerCol;
            cout << "Enter your move (row and column): ";
            cin >> playerRow >> playerCol;
            makeMove(board, playerRow, playerCol, PLAYER_O);
            curr = PLAYER_X;
        }

        print(board);

        int res = evaluate(board, 0);
        if (res == maxScore) {
            cout << "Computer wins!" << endl;
            break;
        } else if (res == minScore) {
            cout << "Player wins!" << endl;
            break;
        }
    }

    if (!isMovePossible(board)) {
        cout << "The game is a draw!" << endl;
    }

    return 0;
}