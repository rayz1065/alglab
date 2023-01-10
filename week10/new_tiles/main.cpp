#include <bits/stdc++.h>
#define MAXN 105
#define MAXM 17
#define TTTM 1000

using namespace std;

int n, m;
bitset<MAXM> grid[MAXN];
vector<bitset<MAXM>> possibilities;
int memo[MAXN][TTTM];

int solve (int r, int prev_id) {
    if (r == n - 1) return 0;
    if (memo[r][prev_id] != -1) return memo[r][prev_id];
    int s = 0;
    bitset<MAXM> free_spots = possibilities[prev_id] & grid[r] & grid[r + 1];
    int free_tiles = MAXM - free_spots.count();
    for (int poss_id = 0; poss_id < possibilities.size(); poss_id++) {
        bitset<MAXM> i = possibilities[poss_id];
        bitset<MAXM> res = free_spots & i;
        int i_tiles = MAXM - i.count();
        int res_tiles = MAXM - res.count();
        if (free_tiles + i_tiles == res_tiles) {
            // matches
            int curr = i_tiles / 2 + solve(r + 1, poss_id);
            s = max(s, curr);
        }
    }
    return memo[r][prev_id] = s;
}

int count (int x, int n, bitset<MAXM> curr) {
    possibilities.push_back(curr);
    int s = 1;
    if (x == n) {
        return s;
    }
    for (int i = 0; x + i + 2 <= n; i++) {
        // skip i tiles and place 2
        curr[x + i] = curr[x + i + 1] = 0;
        s += count(x + i + 2, n, curr);
        curr[x + i] = curr[x + i + 1] = 1;
    }
    return s;
}

void tc () {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < TTTM; j++) {
            memo[i][j] = -1;
        }
    }
    // for (int i = 0; i < MAXM; i++) {
    //     possibilities.clear();
    //     cout << i << " -> " << count(0, i, {}) << "\n";
    //     cout << possibilities.size() << "\n";
    // }
    for (int i = 0; i < n; i++) {
        grid[i].set().flip();
        for (int j = 0; j < m; j++) {
            bool cell;
            cin >> cell;
            grid[i][j] = cell;
        }
    }

    cout << solve(0, 0) << "\n";
}

int main () {
    count(1, MAXM - 1, bitset<MAXM>().set());
    int t;
    cin >> t;
    while (t--) tc();
}
