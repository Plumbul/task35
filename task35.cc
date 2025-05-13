#include <iostream>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

void algoritm_bfs(vector<vector<char>> &test_task0, int row_start, int col_start, int row_end, int col_end, 
                    int rows, int cols, vector<vector<char>> &result){
    queue<pair<int,int>> q;

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    vector<vector<int>> parent(rows, vector<int>(cols, -1));

    q.push({row_start, col_start});
    visited[row_start][col_start] = true;

    vector<int> row_offsets = {-1, 1, 0, 0};
    vector<int> col_offsets = {0, 0, -1, 1};

    while (!q.empty()){
        auto [row_current, col_current] = q.front();
        q.pop();

        for (int move = 0; move < 4; move++){
            int row_next = row_current + row_offsets[move];
            int col_next = col_current + col_offsets[move];

            if (row_next >= 0 && row_next < rows && col_next >= 0 && col_next < cols && !visited[row_next][col_next]){
                if (result[row_next][col_next] == '0' || (row_next == row_end && col_next == col_end)){
                    visited[row_next][col_next] = true;
                    q.push({row_next, col_next});
                    parent[row_next][col_next] = row_current * cols + col_current;
                }
            }
        }
    }

    if(visited[row_end][col_end]) {
        int row_current = row_end;
        int col_current = col_end;

        while(row_current != row_start || col_current != col_start){
            if(result[row_current][col_current] == '0'){
                int parent_cell_index = parent[row_current][col_current];
                int row_parent = parent_cell_index / cols;
                int col_parent = parent_cell_index % cols;

                if(row_parent == row_current){
                    result[row_current][col_current] = '-';
                    } else {
                    result[row_current][col_current] = '|';
                    }
                    row_current = row_parent;
                    col_current = col_parent;
                } else {
            int parent_cell_index = parent[row_current][col_current];
            row_current = parent_cell_index / cols;
            col_current = parent_cell_index % cols;
                }
        }
    }
}

int main() {
    int rows = 10;
    int cols = 10;
    /* int test_task0[10][10] = {
    {1, 0, 0, 0, 0, 0, 0, 0, 2, 3},
    {0, 0, 0, 0, 0, 0, 0, 4, 0, 0},
    {0, 2, 0, 5, 0, 3, 0, 0, 0, 1},
    {0, 6, 0, 4, 0, 0, 0, 0, 6, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 7, 0, 0, 0, 7, 0, 5, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 8, 0, 0},
    {0, 0, 8, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    char test_task1[10][10] = {
        {'.', '.', '.', '.', 'E', '.', '.', '.', 'D'},
        {'.', 'B', 'C', 'A', '.', '.', '.', '.', '.'},
        {'D', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', 'C', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', 'F', '.', '.', '.', '.', '.', '.'},
        {'.', '.', 'G', '.', '.', 'E', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', 'B', '.', '.', '.', '.', '.', '.', 'F'},
        {'A', '.', '.', '.', '.', '.', '.', '.', 'G'}
    };

    char test_task2[10][10] = {
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', 'D'},
        {'.', 'C', 'F', '.', '.', '.', '.', 'A', 'G', '.'},
        {'.', '.', '.', '.', '.', '.', 'D', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', 'E', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', 'G', '.', '.', '.', '.', '.', 'C', 'F', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', 'A', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', 'B', '.'},
        {'B', '.', '.', '.', 'E', '.', '.', '.', '.', '.'}
    };
    */
    vector<vector<char>> test_task0 = {
        {'1', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
        {'0', '0', '3', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'2', '0', '0', '0', '2', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '3', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
    };

    vector<char>symbols_pair = {'1','2','3'};

    vector<vector<char>> result = test_task0;

    for(char symbol : symbols_pair){
        vector<pair<int,int>>symbols_position;

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                if(result[i][j] == symbol){
                    symbols_position.push_back({i, j});
                }
            }
        }

        if(symbols_position.size() >= 2) {
            for(size_t i=0; i+1<symbols_position.size(); i+=2){
                auto[row_start, col_start] = symbols_position[i];
                auto[row_end, col_end] = symbols_position[i+1];
                algoritm_bfs(test_task0, row_start, col_start, row_end, col_end, 
                    rows, cols, result);
            }
        }
    } 

    cout << "test_task0:" << endl;
    for(auto row : test_task0) {
        for(char cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }

    cout << "result:" << endl;
    for(auto row : result) {
        for(char cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }

    return 0;
}