#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

// Typedef for grid
typedef vector<vector<int> > Grid;

// Initialize random grid
Grid randomStart(int rows, int cols) {
    Grid g(rows, vector<int>(cols, 0));
    srand(time(0));
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            g[r][c] = rand() % 2;
        }
    }
    return g;
}

// Load grid from a PBM file
Grid loadFromFile(const string& filename, int& rows, int& cols) {
    ifstream input(filename);
    if (!input) {
        cout << "Could not open file.\n";
        exit(1);
    }

    string header;
    input >> header;

    string comment;
    getline(input, comment); // discard rest of line

    input >> cols >> rows;
    Grid g(rows, vector<int>(cols, 0));

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            input >> g[r][c];
        }
    }

    input.close();
    return g;
}

// Save grid to PBM
void saveToPBM(const string& filename, const Grid& g, int rows, int cols) {
    ofstream output(filename);
    output << "P1\n";
    output << "# Created by Game of Life\n";
    output << cols << " " << rows << endl;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            output << g[r][c] << " ";
        }
        output << endl;
    }
}

// Get number of live neighbors
int getLiveNeighbors(const Grid& g, int r, int c, int rows, int cols) {
    int n = 0;
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr;
            int nc = c + dc;
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                n += g[nr][nc];
            }
        }
    }
    return n;
}

// Move to next generation
Grid nextGen(const Grid& curr, int rows, int cols) {
    Grid future(rows, vector<int>(cols, 0));

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int liveN = getLiveNeighbors(curr, r, c, rows, cols);

            if (curr[r][c] == 1) {
                if (liveN == 2 || liveN == 3) future[r][c] = 1;
            } else {
                if (liveN == 3) future[r][c] = 1;
            }
        }
    }
    return future;
}

int main() {
    int rows, cols;
    Grid board;
    char method;

    cout << "Start randomly (r) or load from file (f)? ";
    cin >> method;

    if (method == 'r' || method == 'R') {
        rows = rand() % 20 + 40;
        cols = rand() % 20 + 40;
        board = randomStart(rows, cols);
    } else {
        string fname;
        cout << "Enter file name: ";
        cin >> fname;
        board = loadFromFile(fname, rows, cols);
    }

    system("mkdir -p frames"); // Make frames folder automatically

    for (int gen = 0; gen < 1000; gen++) {
        char fname[50];
        snprintf(fname, sizeof(fname), "frames/frame%04d.pbm", gen);
        saveToPBM(fname, board, rows, cols);
        board = nextGen(board, rows, cols);
    }

    cout << "Finished creating frames in 'frames/' folder. Check the PBM files\n";
    return 0;
}

