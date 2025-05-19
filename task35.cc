#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

struct Shift {
    int row_start_shift;
    int col_start_shift;
    int row_end_shift;
    int col_end_shift;
};

class Field {
private:
    vector<vector<char>> field_play;
    int rows_num;
    int cols_num;
    vector<char> symbol_pairs;

public:
    Field(int rows, int cols) : rows_num(rows), cols_num(cols) {
        field_play = vector<vector<char>>(rows, vector<char>(cols, '.'));
    }

    void cell_set(int row, int col, char value) {
        if (is_valid(row, col)) {
            field_play[row][col] = value;
        }
    }

    char cell_get(int row, int col) const {
        if (is_valid(row, col)) {
            return field_play[row][col];
        }
        return '.';
    }

    void symbol_pairs_set(vector<char> &symbols) {
        symbol_pairs = symbols;
    }

    vector<char> symbol_pairs_get() const {
        return symbol_pairs;
    }

    bool is_valid(int row, int col) const {
        return row >= 0 && row < rows_num && col >= 0 && col < cols_num;
    }

    int row_num_get() const {
        return rows_num;
    }

    int col_num_get() const {
        return cols_num;
    }
};

class Algoritm {
private:
    bool algoritm_bfs(Field &field, int row_start, int col_start, int row_end, int col_end, Field &result) {
        queue<pair<int, int>> q;
        vector<vector<bool>> visited(field.row_num_get(), vector<bool>(field.col_num_get(), false));
        vector<vector<int>> parent(field.row_num_get(), vector<int>(field.col_num_get(), -1));

        q.push({row_start, col_start});
        visited[row_start][col_start] = true;

        vector<int> row_offsets = {-1, 1, 0, 0};
        vector<int> col_offsets = {0, 0, -1, 1};

        while (!q.empty()) {
            auto [row_current, col_current] = q.front();
            q.pop();

            for (int move = 0; move < 4; move++) {
                int row_next = row_current + row_offsets[move];
                int col_next = col_current + col_offsets[move];

                if (field.is_valid(row_next, col_next) && !visited[row_next][col_next]) {
                    if (result.cell_get(row_next, col_next) == '.' || 
                        (row_next == row_end && col_next == col_end)) {
                        visited[row_next][col_next] = true;
                        q.push({row_next, col_next});
                        parent[row_next][col_next] = row_current * field.col_num_get() + col_current;
                    }
                }
            }
        }

        if (visited[row_end][col_end]) {
            int row_current = row_end;
            int col_current = col_end;

            while (row_current != row_start || col_current != col_start) {
                int parent_cell_index = parent[row_current][col_current];
                int row_parent = parent_cell_index / field.col_num_get();
                int col_parent = parent_cell_index % field.col_num_get();

                if (result.cell_get(row_parent, col_parent) == '.') {
                    result.cell_set(row_parent, col_parent, row_parent == row_current ? '-' : '|');
                }
                row_current = row_parent;
                col_current = col_parent;
            }
            return true;
        }
        return false;
    }

    bool algoritm_shift(Field &field, int row_start, int col_start, int row_end, int col_end, 
                        int rows, int cols, Field &result, char symbol, map<char, vector<Shift>> &symbol_shifts) {
        if (symbol_shifts.find(symbol) == symbol_shifts.end()) {
            return false;
        }
        auto& shifts = symbol_shifts.at(symbol);
        for (const auto& shift : shifts) {
            int row_start_new = row_start + shift.row_start_shift;
            int col_start_new = col_start + shift.col_start_shift;
            int row_end_new = row_end + shift.row_end_shift;
            int col_end_new = col_end + shift.col_end_shift;

            if (!field.is_valid(row_start_new, col_start_new) || 
                result.cell_get(row_start_new, col_start_new) != '.' ||
                !field.is_valid(row_end_new, col_end_new) || 
                field.cell_get(row_end_new, col_end_new) != symbol) {
                continue;
            }

            if (!algoritm_bfs(field, row_start_new, col_start_new, row_end_new, col_end_new, result)) {
                continue;
            }

            auto shift_route = [&](int start_row, int start_col, int row_shift, int col_shift) {
                if (row_shift == 0 && col_shift == 0) return;
                int shift_distance = max(abs(row_shift), abs(col_shift));
                for (int i = 1; i <= shift_distance; i++) {
                    int row = start_row + (row_shift * i + shift_distance - 1) / shift_distance;
                    int col = start_col + (col_shift * i + shift_distance - 1) / shift_distance;
                    if (field.is_valid(row, col) && result.cell_get(row, col) == '.') {
                        result.cell_set(row, col, row_shift != 0 ? '|' : '-');
                    }
                }
            };

            shift_route(row_start, col_start, shift.row_start_shift, shift.col_start_shift);
            shift_route(row_end, col_end, shift.row_end_shift, shift.row_end_shift);

            return true;
        }
        return false;
    }

public:
    void algoritm_retry(Field &field, int rows, int cols, Field &result, const vector<char> &symbols_pair, 
                        map<char, vector<Shift>> &symbol_shifts) {
        bool algoritm_bfs_retry = true;
        vector<char> symbols_pair_change = symbols_pair;

        while (algoritm_bfs_retry) {
            algoritm_bfs_retry = false;
            result = field;
            char symbol_fail = 0;

            for (size_t a = 0; a < symbols_pair_change.size(); a++) {
                char symbol = symbols_pair_change[a];
                vector<pair<int, int>> symbols_position;

                for (int i = 0; i < field.row_num_get(); i++) {
                    for (int j = 0; j < field.col_num_get(); j++) {
                        if (result.cell_get(i, j) == symbol) {
                            symbols_position.push_back({i, j});
                        }
                    }
                }

                if (symbols_position.size() >= 2) {
                    auto [row_start, col_start] = symbols_position[0];
                    auto [row_end, col_end] = symbols_position[1];
                    if (col_start < col_end || (col_start == col_end && row_start < row_end)) {
                        swap(row_start, row_end);
                        swap(col_start, col_end);
                    }

                    bool algoritm_success = algoritm_shift(field, row_start, col_start, row_end, col_end, 
                                                           rows, cols, result, symbol, symbol_shifts) 
                                        || algoritm_bfs(field, row_start, col_start, row_end, col_end, result);

                    if (!algoritm_success) {
                        symbol_fail = symbol;
                        algoritm_bfs_retry = true;
                        break;
                    }
                }

                if (algoritm_bfs_retry) {
                    symbols_pair_change.erase(symbols_pair_change.begin() + a);
                    symbols_pair_change.insert(symbols_pair_change.begin(), symbol_fail);
                    break;
                }
            }
        }
    }
};

class Input {
public:
    void input_user(Field &field) {
        int rows, cols;
        cout << "Enter number of rows: ";
        cin >> rows;
        cout << "Enter number of columns: ";
        cin >> cols;
        cin.ignore();

        if (rows <= 0 || cols <= 0) {
            cout << "Invalid dimensions: rows and columns must be positive.\n";
            return;
        }

        Field user_field(rows, cols);
        cout << "Enter field (separate each cell by space):\n";
        for (int i = 0; i < rows; i++) {
            string line;
            getline(cin, line);
            istringstream iss(line);
            vector<char> row;
            char input_ch;
            while (iss >> input_ch) {
                row.push_back(input_ch);
            }
            if (row.size() != static_cast<size_t>(cols)) {
                cout << "Invalid number of columns in row " << i + 1 << ".\n";
                i--;
                continue;
            }
            for (int j = 0; j < cols; j++) {
                user_field.cell_set(i, j, row[j]);
            }
        }

        vector<char> symbol_pairs;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                char cell = user_field.cell_get(i, j);
                if (cell != '.' && cell != '-' && cell != '|' && 
                    find(symbol_pairs.begin(), symbol_pairs.end(), cell) == symbol_pairs.end()) {
                    symbol_pairs.push_back(cell);
                }
            }
        }
        user_field.symbol_pairs_set(symbol_pairs);
        field = user_field;
    }

    void input_program(Field &field, const vector<vector<char>> &test_task, const vector<char> &symbol_pairs) {
        int rows = test_task.size(), cols = test_task[0].size();
        Field user_field(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                user_field.cell_set(i, j, test_task[i][j]);
            }
        }
        user_field.symbol_pairs_set(const_cast<vector<char>&>(symbol_pairs));
        field = user_field;
    }
};

class Output {
public:
    void print_field(string &title, Field &field) {
        cout << title << ":\n";
        for (int i = 0; i < field.row_num_get(); i++) {
            for (int j = 0; j < field.col_num_get(); j++) {
                cout << field.cell_get(i, j) << " ";
            }
            cout << endl;
        }
    }
};

class Menu {
private:
    Input inp;
    Output out;
    Algoritm alg;
    Field task1, task2;
    vector<char> symbol_pairs1, symbol_pairs2;
    map<char, vector<Shift>> shifts1, shifts2;

public:
    Menu(const vector<vector<char>> &test_task1, const vector<char> &symbols_pair1,
         const vector<vector<char>> &test_task2, const vector<char> &symbols_pair2,
         const map<char, vector<Shift>> &symbol_shifts1, const map<char, vector<Shift>> &symbol_shifts2)
        : task1(test_task1.size(), test_task1[0].size()), task2(test_task2.size(), test_task2[0].size()),
          symbol_pairs1(symbols_pair1), symbol_pairs2(symbols_pair2), shifts1(symbol_shifts1), shifts2(symbol_shifts2) {
        inp.input_program(task1, test_task1, symbols_pair1);
        inp.input_program(task2, test_task2, symbols_pair2);
    }

    void Menu_run() {
        while (true) {
            cout << "1 - User input \n";
            cout << "2 - Program input \n";
            cout << "3 - Exit \n";
            int choice;
            cin >> choice;

            switch (choice) {
            case 1: {
                Field field(1, 1);
                inp.input_user(field);
                Field result = field;
                int rows = field.row_num_get();
                int cols = field.col_num_get();
                string title = "User input";
                alg.algoritm_retry(field, rows, cols, result, field.symbol_pairs_get(), shifts1);
                out.print_field(title, field);
                title = "Result of user input";
                out.print_field(title, result);
                break;
            }
            case 2: {
                cout << "Solving of test task #1:\n";
                Field result1 = task1;
                int rows = task1.row_num_get();
                int cols = task1.col_num_get();
                string title = "Test task #1";
                alg.algoritm_retry(task1, rows, cols, result1, symbol_pairs1, shifts1);
                out.print_field(title, task1);
                title = "Result of task #1";
                out.print_field(title, result1);

                cout << "Solving of test task #2:\n";
                Field result2 = task2;
                rows = task2.row_num_get();
                cols = task2.col_num_get();
                title = "Test task #2";
                alg.algoritm_retry(task2, rows, cols, result2, symbol_pairs2, shifts2);
                out.print_field(title, task2);
                title = "Result of task #2";
                out.print_field(title, result2);
                break;
            }
            case 3:
                return;
            default:
                cout << "Invalid choice. Try again\n";
            }
        }
    }
};

int main() {
    vector<vector<char>> test_task1 = {
        {'.', '.', '.', '.', '.', 'E', '.', '.', '.', 'D'},
        {'.', 'B', 'C', 'A', '.', '.', '.', '.', '.', '.'},
        {'D', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', 'C', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', 'F', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', 'G', '.', '.', '.', 'E', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', 'B', '.', '.', '.', '.', '.', '.', '.', 'F'},
        {'A', '.', '.', '.', '.', '.', '.', '.', '.', 'G'}
    };

    vector<char> symbols_pair1 = {'B', 'C', 'F', 'G', 'A', 'E', 'D'};

    map<char, vector<Shift>> symbol_shifts1 = {
        {'B', {{0, -1, 0, 0}}},
        {'C', {{0, -3, 0, 0}}},
        {'E', {{0, 2, 0, 0}}},
        {'F', {{0, -5, 0, 0}}},
        {'G', {{0, -6, 0, 0}}},
        {'A', {{0, 0, 1, 0}}},
        {'D', {{0, 0, 0, 0}}}
    };

    vector<vector<char>> test_task2 = {
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

    vector<char> symbols_pair2 = {'B', 'C', 'F', 'G', 'A', 'E', 'D'};

    map<char, vector<Shift>> symbol_shifts2 = {
        {'B', {{0, 0, 0, 0}}},
        {'C', {{0, 0, 0, 0}}},
        {'E', {{0, 0, 0, 0}}},
        {'F', {{0, 0, 0, 0}}},
        {'G', {{0, 0, 0, 0}}},
        {'A', {{0, 0, 0, 0}}},
        {'D', {{0, 0, 0, 0}}}
    };

    Menu menu(test_task1, symbols_pair1, test_task2, symbols_pair2, symbol_shifts1, symbol_shifts2);
    menu.Menu_run();

    return 0;
}