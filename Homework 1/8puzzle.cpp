#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

const int FOUND = -1;
int root, goalPositionOfZero;
int zero_x, zero_y;

void print(vector<string> path){
    cout << path.size() << endl;

    for (int i = 0; i < path.size(); i++) {
            cout << path[i] << endl;
    }
}

//Manhattan distance
int manhattan(const vector<vector<int>>& board){
    int h = 0;
    int target_row, target_col, value;
    int row, col;

    for (row = 0; row < root; row++) {
        for (col = 0; col < root; col++) {
            value = board[row][col];
            if (value != 0) {
                target_row = (value - 1) / root;
                target_col = (value - 1) % root;

                if (goalPositionOfZero < value) {
                    if (target_col + 1 >= root) {
                        target_row++;
                        target_col = 0;
                    } else {
                        target_col++;
                    }
                }

                int dx = row - target_row;
                int dy = col - target_col;
                h += (dx * dx) + (dy * dy);
            }
        }
    }
    return h;
}

bool isSolvable(vector<int> tiles){
    int inversions = 0;

    for(int i = 0; i < root*root - 1; i++){
        for(int j = i+1; j < root*root; j++){
            if(tiles[i] == 0 || tiles[j] == 0) continue;
           
            if(tiles[i] > tiles[j]) inversions++;
        }
    }
    return inversions % 2 == 0;
}

vector<vector<int>> move(const vector<vector<int>>& board, int new_zero_x, int new_zero_y){
    vector<vector<int>> tmp = board;
    swap(tmp[zero_x][zero_y], tmp[new_zero_x][new_zero_y]);

    return tmp;
}

//IDA* algorithm
int search(vector<vector<int>> board, vector<string>& path, int g, int threshold){
    int manh = manhattan(board);
    int f = g + manh;

    if(f > threshold) return f;

    if(manh == 0){
        print(path);
        return FOUND;
    } 
    
    int min = INT_MAX;
    int t = 0;

    const vector<int> dx = {1, -1, 0, 0};
    const vector<int> dy = {0, 0, 1, -1};
    const vector<string> move_names = {"up", "down", "left", "right"};

    for(int i = 0; i < 4; i++){
        int new_zero_x = zero_x + dx[i];
        int new_zero_y = zero_y + dy[i];

        if (new_zero_x >= 0 && new_zero_x < root && new_zero_y >= 0 && new_zero_y < root) {
            vector<vector<int>> successor = move(board, new_zero_x, new_zero_y);

            zero_x = new_zero_x;
            zero_y = new_zero_y;

            path.push_back(move_names[i]);
            t = search(successor, path, g + 1, threshold);

            if(t == FOUND) return FOUND;

            if(t < min) min = t;

            path.pop_back();

            zero_x = new_zero_x - dx[i];
            zero_y = new_zero_y - dy[i];
        }
    }
    
    return min;
}

void IDAstar(vector<vector<int>> board, vector<int> tiles){
    if(!isSolvable(tiles)){
        cout << -1 << endl;
        return;
    }

    vector<string> path;
    int threshold = manhattan(board);
    int tmp;

    while(1){
        tmp = search(board, path, 0, threshold);

        if(tmp == FOUND) break;

        threshold = tmp;
    }
}

int main(){
    int N;
    cin >> N;
    cin >> goalPositionOfZero;

    root = sqrt(N + 1);
    vector<vector<int>> startBoard(root, vector<int>(root));
    vector<int> tiles;

    if(goalPositionOfZero == -1){
        goalPositionOfZero = root*root - 1;
    }

    for(int i = 0; i < root; i++){
        for(int j = 0; j < root; j++){
            cin >> startBoard[i][j];
            tiles.push_back(startBoard[i][j]);

            if (startBoard[i][j] == 0) {
                zero_x = i;
                zero_y = j;
            }
        }
    }

    clock_t start = clock();
    IDAstar(startBoard, tiles);
    clock_t end = clock();
    double duration = (end - start) / (double) CLOCKS_PER_SEC;
    printf("Execution time: %f secs.\n", duration);
    
    return 0;
}