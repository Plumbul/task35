#include <iostream>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

bool algoritm_bfs(vector<vector<char>> &test_task0, int row_start, int col_start, int row_end, int col_end, 
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
            int parent_cell_index = parent[row_current][col_current];
            int row_parent = parent_cell_index / cols;
            int col_parent = parent_cell_index % cols;
            
            if(result[row_parent][col_parent] == '0'){
                if(row_parent == row_current){
                    result[row_parent][col_parent] = '-';
                    } else {
                    result[row_parent][col_parent] = '|';
                    }
                }
                    row_current = row_parent;
                    col_current = col_parent;
        }
        return true;
    }
    return false;
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
        {'0', '0', '0', '0', '0', '2', '0', '0', '2', '3'},
        {'0', '0', '0', '0', '0', '0', '0', '1', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '3', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
    };

    vector<char>symbols_pair = {'1','2','3'};

    vector<vector<char>> result = test_task0;

    bool algoritm_retry = true;

    while(algoritm_retry){
        algoritm_retry = false;
        result = test_task0;
        char symbol_fail = 0;

        for(size_t a = 0; a < symbols_pair.size(); a++){
            char symbol = symbols_pair[a];
            vector<pair<int,int>>symbols_position;

            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    if(result[i][j] == symbol){
                        symbols_position.push_back({i, j});
                    }
                }
            }

        if(symbols_position.size() >= 2) {
            for(size_t b=0; b+1<symbols_position.size(); b+=2){
                auto[row_start, col_start] = symbols_position[b];
                auto[row_end, col_end] = symbols_position[b+1];

                bool algoritm_sucess = algoritm_bfs(result, row_start, col_start, row_end, col_end, 
                    rows, cols, result);

                if(!algoritm_sucess) {
                    symbol_fail = symbol;
                    algoritm_retry = true;
                    break;
                }
            }
        }
        if (algoritm_retry) {
            symbols_pair.erase(symbols_pair.begin() + a);
            symbols_pair.insert(symbols_pair.begin(), symbol_fail);
            break;
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