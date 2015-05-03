#include <fstream>
#include <iostream>

using namespace std;

const char _Q = 14; //(char('?') - char(1));

class task {
private:
    bool qplane[9][9][9];
    char plane[9][9];
    void refresh_row (char row) {
        for (char i = 0; i < 9; i++) {
            if (plane[row][i] != _Q) {
                refresh_row_by_cell(row, i);
            }
        }
    }
    void refresh_row_by_cell (char row, char col) {
        for (char j = 0; j < 9; j++) {
            if (col != j) qplane[row][j][plane[row][col]] = false;
        }
    }
    void refresh_col (char col) {
        for (char i = 0; i < 9; i++) {
            if (plane[i][col] != _Q) {
                refresh_col_by_cell(i, col);
            }
        }
    }
    void refresh_col_by_cell (char row, char col) {
        for (char j = 0; j < 9; j++) {
            if (row != j) qplane[j][col][plane[row][col]] = false;
        }
    }
    void refresh_block (char row, char col) {
        for (char i = 0; i < 9; i++) {
            if (plane[i / 3 + 3 * row][i % 3 + 3 * col] != _Q) {
                refresh_block_by_cell(row, col, i);
            }
        }
    }
    void refresh_block_by_cell (char block_row, char block_col, char pos) {
        for (char j = 0; j < 9; j++) {
            if (pos != j) qplane[j / 3 + 3 * block_row][j % 3 + 3 * block_col][plane[pos / 3 + 3 * block_row][pos % 3 + 3 * block_col]] = false;
        }
    }
    void _set (char row, char col, char number) {
        if ((plane[row][col] != _Q) || (number == _Q)) {
            plane[row][col] = _Q;
            refresh_all();
            if (number == _Q) {
                return;
            }
        }
        plane[row][col] = number;
        refresh_block_by_cell(row / 3, col / 3, (row % 3) * 3 + col % 3);
        refresh_row_by_cell(row, col);
        refresh_col_by_cell(row, col);
    }
    bool find_place_in_row (char row, char number) {
        char cnt = 0;
        for (char i = 0; i < 9; i++) {
            if ((qplane[row][i][number]) && (plane[row][i] == _Q)) cnt++;
        }
        if (cnt == 1) {
            for (cnt = 0; ((!qplane[row][cnt][number]) || (plane[row][cnt] != _Q)); cnt++);
            _set(row, cnt, number);
            return true;
        }
        else {
            return false;
        }
    }
    bool find_place_in_col (char col, char number) {
        char cnt = 0;
        for (char i = 0; i < 9; i++) {
            if (qplane[i][col][number] && (plane[i][col] == _Q)) cnt++;
        }
        if (cnt == 1) {
            for (cnt = 0; (!qplane[cnt][col][number] || (plane[cnt][col] != _Q)); cnt++);
            _set(cnt, col, number);
            return true;
        }
        else {
            return false;
        }
    }
    bool find_place_in_block (char block_row, char block_col, char number) {
        char cnt = 0;
        for (char i = 0; i < 9; i++) {
            if (qplane[block_row * 3 + i / 3][block_col * 3 + i % 3][number] && (plane[block_row * 3 + i / 3][block_col * 3 + i % 3] == _Q)) cnt++;
        }
        if (cnt == 1) {
            for (cnt = 0; ((!qplane[block_row * 3 + cnt / 3][block_col * 3 + cnt % 3][number]) || (plane[block_row * 3 + cnt / 3][block_col * 3 + cnt % 3] != _Q)); cnt++);
            _set(block_row * 3 + cnt / 3, block_col * 3 + cnt % 3, number);
            return true;
        }
        else {
            return false;
        }
    }
    bool try_set () {
        char res = 0;
        char cnt;
        for (char p = 0; p < 81; p++) {
            if (plane[p / 9][p % 9] == _Q) {
                cnt = 0;
                for (char i = 0; i < 9; i++) {
                    if (qplane[p / 9][p % 9][i])
                        cnt++;
                }
                if (cnt == 1) {
                    for (cnt = 0; !qplane[p / 9][p % 9][cnt]; cnt++);
                    _set(p / 9, p % 9, cnt);
                    res++;
                }
            }
        }
        return res;
    }
    bool try_resolve_row (char row) {
        bool nums[9] = {false, false, false, false, false, false, false, false, false}, res = false;
        for (char i = 0; i < 9; i++) {
            if (plane[row][i] != _Q) {
                nums[plane[row][i]] = true;
            }
        }
        for (char i = 0; i < 9; i++) {
            if (!nums[i]) {
                res = find_place_in_row(row, i) || res;
            }
        }
        return res;
    }
    bool try_resolve_col (char col) {
        bool nums[9] = {false, false, false, false, false, false, false, false, false}, res = false;
        for (char i = 0; i < 9; i++) {
            if (plane[i][col] != _Q) {
                nums[plane[i][col]] = true;
            }
        }
        for (char i = 0; i < 9; i++) {
            if (!nums[i]) {
                res = find_place_in_col(col, i) || res;
            }
        }
        return res;
    }
    bool try_resolve_block (char block_row, char block_col) {
        bool nums[9] = {false, false, false, false, false, false, false, false, false}, res = false;
        for (char i = 0; i < 9; i++) {
            if (plane[3 * block_row + i / 3][3 * block_col + i % 3] != _Q) {
                nums[plane[3 * block_row + i / 3][3 * block_col + i % 3]] = true;
            }
        }
        for (char i = 0; i < 9; i++) {
            if (!nums[i]) {
                res = find_place_in_block(block_row, block_col, i) || res;
            }
        }
        return res;
    }
    char try_resolve () {
        char res = 0;
        for (char i = 0; i < 9; i++) {
            if (try_resolve_row(i)) res++;
            if (try_resolve_col(i)) res++;
            if (try_resolve_block(i / 3, i % 3)) res++;
        }
        return res;
    }
public:
    task (ifstream *file) {
        char n;
        for (char p = 0; p < 81; p++) {
            do {
                (*file) >> n;
            } while ((n == ' ') || (n == '\n'));
            n -= '1';
            for (char i = 0; i < 9; i++) {
                qplane[p / 9][p % 9][i] = true;
            }
            plane[p / 9][p % 9] = n;
        }
        refresh_all();
    }
    bool check () {
        for (char i = 0; i < 9; i++) {
            for (char j = 0; j < 9; j++) {
                if (plane[i][j] == _Q) {
                    return false;
                }
            }
        }
        return true;
    }
    void refresh_all () {
        for (char i = 0; i < 9; i++) {
            refresh_col(i);
            refresh_row(i);
            refresh_block(i / 3, i % 3);
        }
    }
    void set (char row, char col, char number) {
        _set(--row, --col, --number);
    }
    bool resolve () {
        if (check()) {
            return true;
        }
        char c, ac;
        do {
            ac = 0;
            do {
                ac++;
                c = try_set();
                cout << "Setted " << (int)c << " cells (only one match)" << endl;
            } while (c > 0);
            do {
                ac++;
                c = try_resolve();
                cout << "Setted " << (int)c << " cells (only one place)" << endl;
            } while (c > 0);
        } while (ac > 2);
        return check();
    }
    bool print (ostream *str) {
        for (char i = 0; i < 9; i++) {
            for (char j = 0; j < 9; j++) {
                *str << char(plane[i][j] + '1') << ' ';
            }
            *str << endl;
        }
    }
    void dump (char row, char col) const {
        for (char i = 0; i < 9; i++) {
            cout << qplane[row][col][i] << ' ';
        }
        cout << endl;
    }
};

int main (int argc, char **argv) {
    if (argc == 1) {
        cout << "Usage:\n\t" << argv[0] << " <input file> <output file>\nIf <output file> is undefined, you get output in 'sudoku.out'" << endl;
        return 0;
    }
    ifstream *inf = new ifstream(argv[1]);
    task sdk(inf);
    inf->close();
    delete(inf);
    sdk.resolve();
    ofstream *ouf;
    if (argc == 2) {
        ouf = new ofstream("sudoku.out");
    }
    else {
        ouf = new ofstream(argv[2]);
    }
    sdk.print(ouf);
    ouf->close();
    delete(ouf);
    return 0;
}