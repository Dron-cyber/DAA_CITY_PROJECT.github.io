/*
    NOTE:
    This program uses a sample 6×6 grid dataset representing different
    city areas, tower locations, and initial signal strengths.  
    The dataset was manually created only to demonstrate the mobile tower
    coverage analysis algorithm using BFS.

    The logic is fully scalable — more rows, more columns, dynamic datasets
    from files, or real-world tower data can be integrated without changing
    the BFS propagation or coverage computation logic.
*/

#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <queue>
#include <iomanip>
#include <sstream>

using namespace std;

struct Cell {
    int row, col;
    string area;
    bool hasTower;
    int signal;
    bool covered;
};

class CoverageAnalyzer {
private:
    const int N = 6;
    vector<vector<Cell>> grid;
    vector<tuple<int, int, string, int, int, int>> dataset;

public:
    CoverageAnalyzer() : grid(N, vector<Cell>(N)) {
        dataset = {
            {1, 1, "Market Circle", 1, 5, 1},
            {1, 2, "Market Circle", 0, 4, 1},
            {1, 3, "Shopping Lane A", 0, 3, 1},
            {1, 4, "Shopping Lane B", 0, 2, 1},
            {1, 5, "Temple Street", 0, 1, 1},
            {1, 6, "Temple Street", 0, 0, 1},
            {2, 1, "Market Circle", 0, 4, 1},
            {2, 2, "City Hospital Road", 0, 3, 1},
            {2, 3, "Shopping Lane A", 0, 2, 1},
            {2, 4, "Shopping Lane B", 0, 1, 1},
            {2, 5, "Residential A", 0, 0, 1},
            {2, 6, "Residential A", 0, -1, 0},
            {3, 1, "University Road", 0, 3, 1},
            {3, 2, "University Road", 0, 2, 1},
            {3, 3, "IT Park Shops", 0, 1, 1},
            {3, 4, "IT Park Shops", 1, 5, 1},
            {3, 5, "IT Park", 0, 4, 1},
            {3, 6, "IT Park", 0, 3, 1},
            {4, 1, "Industrial Zone", 0, 2, 1},
            {4, 2, "Industrial Zone", 0, 1, 1},
            {4, 3, "City Bus Stand Area", 0, 0, 1},
            {4, 4, "City Bus Stand Area", 0, 3, 1},
            {4, 5, "Residential B", 0, 2, 1},
            {4, 6, "Residential B", 0, 1, 1},
            {5, 1, "Airport Road", 0, 1, 1},
            {5, 2, "Airport Road", 1, 5, 1},
            {5, 3, "Airport Road Shops", 0, 4, 1},
            {5, 4, "Airport Road Shops", 0, 3, 1},
            {5, 5, "New Market Zone", 0, 2, 1},
            {5, 6, "New Market Zone", 0, 1, 1},
            {6, 1, "Outer Ring Road", 0, 0, 1},
            {6, 2, "Outer Ring Road", 0, -1, 0},
            {6, 3, "Outer Ring Road", 0, -1, 0},
            {6, 4, "Outer Ring Road", 0, 2, 1},
            {6, 5, "Outer Ring Road", 0, 1, 1},
            {6, 6, "Outer Ring Road", 0, 0, 1}
        };
        loadGrid();
    }

    void loadGrid() {
        for (auto& entry : dataset) {
            int r, c, has, sig, cov;
            string area;
            tie(r, c, area, has, sig, cov) = entry;
            r--; c--;
            grid[r][c] = {r+1, c+1, area, (bool)has, sig, (bool)cov};
        }
    }

    void analyzeCoverage() {
        const int INF = 1e9;
        vector<vector<int>> dist(N, vector<int>(N, INF));
        queue<pair<int, int>> q;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j].hasTower) {
                    dist[i][j] = 0;
                    grid[i][j].signal = 5;
                    grid[i][j].covered = true;
                    q.push(make_pair(i, j));
                }
            }
        }

        int dr[] = {-1, 0, 1, 0};
        int dc[] = {0, 1, 0, -1};

        while (!q.empty()) {
            pair<int, int> front = q.front(); q.pop();
            int r = front.first;
            int c = front.second;

            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                    if (dist[nr][nc] > dist[r][c] + 1) {
                        dist[nr][nc] = dist[r][c] + 1;
                        int newSignal = 5 - dist[nr][nc];
                        grid[nr][nc].signal = newSignal;
                        if (newSignal > 0) {
                            grid[nr][nc].covered = true;
                            q.push(make_pair(nr, nc));
                        }
                    }
                }
            }
        }
    }

    void printGaps() {
        cout << "\nCoverage Analysis Complete\n";
        int total = 0, gaps = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                total++;
                if (!grid[i][j].covered) gaps++;
            }
        }
        cout << "Total cells: " << total << "\n";
        cout << "Coverage gaps: " << gaps << "\n\n";
        
        cout << "COVERAGE GAPS:\n";
        bool found = false;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (!grid[i][j].covered) {
                    cout << "  [" << grid[i][j].row << "," << grid[i][j].col 
                         << "] " << grid[i][j].area << " (Signal: " << grid[i][j].signal << ")\n";
                    found = true;
                }
            }
        }
        if (!found) cout << "  Perfect coverage!\n";
    }

    void queryArea(string areaName) {
        int covered = 0, total = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j].area == areaName) {
                    total++;
                    if (grid[i][j].covered) covered++;
                }
            }
        }
        cout << "'" << areaName << "': " << covered << "/" << total 
             << " covered (" << (total > 0 ? (covered*100)/total : 0) << "%)\n";
    }

    void printGrid() {
        cout << "\nCoverage Grid (5=Strong, 0=No signal, -1=No coverage):\n";
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int s = grid[i][j].signal;
                if (s >= 3) cout << "G";
                else if (s >= 1) cout << "Y";
                else cout << "R";
                cout << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    CoverageAnalyzer analyzer;
    
    cout << "=== Mobile Tower Coverage Gap Finder (BFS) ===\n";
    cout << "36 cells loaded. Ready for analysis.\n\n";
    
    cout << "Commands:\n";
    cout << "  analyze          - Run BFS coverage simulation\n";
    cout << "  gaps             - Show coverage gaps\n";
    cout << "  grid             - Display coverage heatmap\n";
    cout << "  query <area>     - Check area coverage\n";
    cout << "  Example: query Outer Ring Road\n\n";
    
    string command;
    while (true) {
        cout << "coverage> ";
        getline(cin, command);
        if (command.empty()) continue;
        
        stringstream ss(command);
        string type;
        ss >> type;
        
        if (type == "analyze" || type == "bfs") {
            analyzer.analyzeCoverage();
            analyzer.printGaps();
            
        } else if (type == "gaps") {
            analyzer.printGaps();
            
        } else if (type == "grid") {
            analyzer.analyzeCoverage();
            analyzer.printGrid();
            
        } else if (type == "query") {
            string area;
            getline(ss, area);
            size_t start = area.find_first_not_of(" \t");
            if (start != string::npos) area = area.substr(start);
            analyzer.analyzeCoverage();
            analyzer.queryArea(area);
            
        } else if (type == "exit" || type == "quit") {
            break;
        } else {
            cout << "Unknown command. Use: analyze, gaps, grid, query <area>, exit\n";
        }
    }
    
    cout << "\n=== Analysis Complete ===\n";
    return 0;
}

