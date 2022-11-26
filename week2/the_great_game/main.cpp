#include <bits/stdc++.h>
#define MAXN 100005

using namespace std;

int n, m;
int r, b;
vector<int> graph[MAXN];
int memo[MAXN][2];

int solve (int i, bool minimize);

int find_min (int i) {
    int s = INT_MAX >> 1;
    for (auto j: graph[i]) {
        s = min(s, solve(j, false) + 1);
    }
    return s;
}

int find_max (int i) {
    int s = -1;
    for (auto j: graph[i]) {
        s = max(s, solve(j, true) + 1);
    }
    return s;
}

int solve (int i, bool minimize) {
    if (i == n) return 0;
    if (memo[i][minimize] != -1) return memo[i][minimize];
    if (minimize) return memo[i][minimize] = find_min(i);
    else return memo[i][minimize] = find_max(i);
}

int r_turn (int moves) {
    int groups = moves / 2;
    return groups * 4 + moves % 2;
}

int b_turn (int moves) {
    int groups = moves / 2;
    if (moves % 2 == 0) return groups * 4 - 1;
    else return groups * 4 + 2;
}

void solve_tc() {
    cin >> n >> m;
    cin >> r >> b;
    for (int i = 0; i < n; i++) {
        graph[i].clear();
        memo[i][0] = memo[i][1] = -1;
    }
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
    }
    int r_moves = solve(r, true);
    int b_moves = solve(b, true);
    // moves order is RBBR
    cout << (r_turn(r_moves) > b_turn(b_moves)) << "\n";
}

int main () {
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) solve_tc();
}