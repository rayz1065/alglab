#include <bits/stdc++.h>
#define MAXN 1005
#define MAXM 4005

using namespace std;

typedef long long int ll;

struct Branch {
    int to;
    int cost;
    int gain;
};

int n;
ll x_score;
vector<Branch> graph[MAXN];
ll memo[MAXN][MAXM];

ll solve (int node, int rem_moves) {
    if (rem_moves == 0) return 0;
    if (memo[node][rem_moves] != -1) return memo[node][rem_moves];
    ll s = 0;
    for (auto j: graph[node]) {
        s = max(s, solve(j.to, rem_moves - j.cost) + j.gain);
    }
    return memo[node][rem_moves] = s;
}

int calc_moves (ll x_score) {
    for (int i = 1;; i++) {
        if (solve(0, i) >= x_score) {
            return i;
        }
    }
}

void tc () {
    int m, k_moves;
    cin >> n >> m >> x_score >> k_moves;
    for (int i = 0; i < n; i++) {
        graph[i].clear();
        fill(memo[i], memo[i] + k_moves + 1, -1);
    }
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({ v, 1, w });
    }
    for (int i = 0; i < n; i++) {
        if (graph[i].size() == 0) {
            graph[i].push_back({ 0, 0, 0 });
        }
    }
    if (solve(0, k_moves) < x_score) cout << "Impossible\n";
    else cout << calc_moves(x_score) << "\n";
}

int main () {
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}