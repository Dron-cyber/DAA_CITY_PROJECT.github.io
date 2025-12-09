#include <bits/stdc++.h>
using namespace std;

struct Cell { int r, c; };
int dr[4] = { -1, 1, 0, 0 };
int dc[4] = { 0, 0, -1, 1 };
string dirName[4] = { "UP", "DOWN", "LEFT", "RIGHT" };


bool valid(int r, int c, int R, int C, const vector<string>& grid) {
    return r >= 0 && r < R && c >= 0 && c < C && grid[r][c] != '#' && grid[r][c] != 'F';
}

bool findShortestEscapePath(const vector<string>& grid, Cell start, vector<Cell>& path) {
    int R = (int)grid.size();
    if (R == 0) return false;
    int C = (int)grid[0].size();

    vector<vector<bool>> vis(R, vector<bool>(C, false));

    vector<vector<pair<int,int>>> parent(R, vector<pair<int,int>>(C, {-1,-1}));
    vector<vector<int>> parentDir(R, vector<int>(C, -1));

    queue<Cell> q;
    q.push(start);
    vis[start.r][start.c] = true;

    Cell found = {-1,-1};
    while (!q.empty()) {
        Cell cur = q.front(); q.pop();
        if (grid[cur.r][cur.c] == 'E') { found = cur; break; }
        for (int k = 0; k < 4; ++k) {
            int nr = cur.r + dr[k], nc = cur.c + dc[k];
            if (valid(nr, nc, R, C, grid) && !vis[nr][nc]) {
                vis[nr][nc] = true;
                parent[nr][nc] = {cur.r, cur.c};
                parentDir[nr][nc] = k;
                q.push({nr, nc});
            }
        }
    }

    if (found.r == -1) return false;
    vector<Cell> rev;
    Cell cur = found;
    while (!(cur.r == start.r && cur.c == start.c)) {
        rev.push_back(cur);
        auto p = parent[cur.r][cur.c];
        cur = {p.first, p.second};
    }
    rev.push_back(start);
    reverse(rev.begin(), rev.end());
    path = move(rev);
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);


    int R, C;
    if (!(cin >> R >> C)) {
        cerr << "Invalid input. Provide R C then grid rows.\n";
        return 1;
    }
    vector<string> grid(R);
    for (int i = 0; i < R; ++i) {
        cin >> grid[i];
        if ((int)grid[i].size() != C) {
            cerr << "Row " << i << " must have exactly " << C << " characters.\n";
            return 1;
        }
    }


    Cell start = {-1, -1};
    for (int i = 0; i < R; ++i) for (int j = 0; j < C; ++j)
        if (grid[i][j] == 'S') start = {i, j};

    if (start.r == -1) {
        cerr << "No 'S' start cell found in grid. Please place 'S' or provide start coordinates.\n";
        return 1;
    }

    vector<Cell> path;
    bool ok = findShortestEscapePath(grid, start, path);
    if (!ok) {
        cout << "No safe escape path found. Stay put and await rescue.\n";
        return 0;
    }

    cout << "Escape path found! Steps (" << path.size()-1 << " moves):\n";
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        Cell cur = path[i], nxt = path[i+1];
        cout << "(" << cur.r << "," << cur.c << ") -> ";
        int drc = nxt.r - cur.r, dcc = nxt.c - cur.c;
        int dirIdx = -1;
        for (int k=0;k<4;++k) if (dr[k]==drc && dc[k]==dcc) { dirIdx=k; break; }
        if (dirIdx >= 0) cout << dirName[dirIdx];
        else cout << "MOVE";
        cout << " -> (" << nxt.r << "," << nxt.c << ")\n";
    }


    vector<string> out = grid;
    for (size_t i = 1; i + 1 < path.size(); ++i) {
        out[path[i].r][path[i].c] = 'P';
    }
    cout << "\nGrid with path (P marks route):\n";
    for (int i = 0; i < R; ++i) cout << out[i] << "\n";

    return 0;
}
