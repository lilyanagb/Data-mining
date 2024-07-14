#include <iostream>
#include <vector>
using namespace std;

vector<vector<char>> fullPath;

void jump(vector<char> &current, int from, int to){
    char tmp = current[from];
    current[from] = current[to];
    current[to] = tmp;
}

vector<vector<char>> getPossibleMoves(const vector<char> &current) {
    vector<vector<char>> possibleMoves;
    
    for (unsigned int i = 0; i < current.size(); ++i) {
        if (i < current.size() - 1 && current[i] == '>' && current[i + 1] == '_') {
            vector<char> move = current;
            jump(move, i, i + 1);
            possibleMoves.push_back(move);
        }
        if (i < current.size() - 2 && current[i] == '>' && current[i + 2] == '_') {
            vector<char> move = current;
            jump(move, i, i + 2);
            possibleMoves.push_back(move);
        }
        if (i > 0 && current[i] == '<' && current[i - 1] == '_') {
            vector<char> move = current;
            jump(move, i, i - 1);
            possibleMoves.push_back(move);
        }
        if (i > 1 && current[i] == '<' && current[i - 2] == '_') {
            vector<char> move = current;
            jump(move, i, i - 2);
            possibleMoves.push_back(move);
        }
    }

    return possibleMoves;
}

bool isGoalState(const vector<char>& state, int zeroState){
    int size = state.size();

    for(int i = 0; i < zeroState; i++){
        if(state[i] == '>' || state[i] == '_') return false;
        if(state[size-i-1] == '<' || state[size-i-1] == '_') return false;
    }

    return true;
}

bool dfs(vector<char> &current, int zeroState) {
    
    if (isGoalState(current, zeroState)) {
        return true;
    }

    vector<vector<char>> moves = getPossibleMoves(current);
    for (vector<char> &move : moves) {
        if (dfs(move, zeroState)) {
            fullPath.push_back(move);
            return true;
        }
    }

    //23
    //Execution time: 104.677000 secs.
    
    return false;
}

int main() {
    int N;
    cin >> N;

    int size = N * 2 + 1;
    
    vector<char> board = vector<char>(size, '_');
    for(int i = 0; i < N; i++){
        board[i] = '>';
        board[size - i - 1] = '<';
    }

    clock_t start = clock();
    
    if(!dfs(board, N)){
        cout << "No solution found" << endl;
    }

    clock_t end = clock();
    double duration = (end - start) / (double) CLOCKS_PER_SEC;
    printf("Execution time: %f secs.\n", duration);

    fullPath.push_back(board); 
    
    for (int i = fullPath.size() - 1; i >= 0; i--) {
        for (char frog : fullPath[i]) {
            cout << frog;
        }
        cout << endl;
    }

    return 0;
}