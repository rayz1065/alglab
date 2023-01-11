#include <bits/stdc++.h>
#define MAXN 100005

using namespace std;

struct Solution {
    int by_fixing;
    int by_not_fixing;
};

int n_nodes;
vector<int> graph[MAXN];
int cost[MAXN];
Solution memo[MAXN][2];

Solution solve (int node, bool parent_fixed) {
    if (graph[node].size() == 0) {
        return { cost[node], parent_fixed ? 0 : (INT_MAX >> 1) };
    }
    if (memo[node][parent_fixed].by_fixing != -1) {
        return memo[node][parent_fixed];
    }
    Solution s = { cost[node], 0 };
    // fix this node, choose whether to fix each child
    for (auto i: graph[node]) {
        auto res = solve(i, true);
        s.by_fixing += min(res.by_fixing, res.by_not_fixing);
    }
    // don't fix this node
    int delta = INT_MAX >> 1;
    for (auto i: graph[node]) {
        auto res = solve(i, false);
        delta = min(delta, res.by_fixing - res.by_not_fixing);
        s.by_not_fixing += min(res.by_fixing, res.by_not_fixing);
    }
    if (!parent_fixed) {
        // at least one child must be fixed, fix the one with lowest penalty
        s.by_not_fixing += max(delta, 0);
    }

    return memo[node][parent_fixed] = s;
}

void tc () {
    cin >> n_nodes;
    for (int i = 0; i < n_nodes; i++) {
        memo[i][0].by_fixing = memo[i][1].by_fixing = -1;
        graph[i].clear();
    }
    for (int i = 0; i < n_nodes - 1; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
    }
    for (int i = 0; i < n_nodes; i++) {
        cin >> cost[i];
    }
    Solution s = solve(0, false);
    cout << min(s.by_fixing, s.by_not_fixing) << "\n";
}

int main () {
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
