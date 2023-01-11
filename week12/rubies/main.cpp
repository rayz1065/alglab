#include <bits/stdc++.h>
#define MAXN 50005

using namespace std;

typedef long long ll;
vector<pair<int, ll>> graph[MAXN];
int values[MAXN];

struct Sol {
    ll acc_pen;
    int nodes;
    int weights;
};

bool weight_cmp (Sol &a, Sol &b) {
    return a.weights < b.weights;
}

bool avg_weight_cmp (Sol &a, Sol &b) {
    return (ll) b.nodes * a.weights < (ll) a.nodes * b.weights;
}

Sol solve (int node) {
    if (graph[node].size() == 0) {
        return { 0, 1, 0 };
    }
    vector<Sol> ch_sols;
    int tt_nodes = 1;
    ll tt_cost = 0;
    for (auto j: graph[node]) {
        Sol s = solve(j.first);
        s.weights += j.second * 2;
        tt_nodes += s.nodes;
        tt_cost += j.second * s.nodes + s.acc_pen;
        ch_sols.push_back(s);
    }
    // sort them by weight, visit from lighter to heavier
    sort(ch_sols.begin(), ch_sols.end(), avg_weight_cmp);
    int tt_weights = 0;
    for (auto &s: ch_sols) {
        tt_cost += (ll) tt_weights * s.nodes;
        tt_weights += s.weights;
    }
    return { tt_cost, tt_nodes, tt_weights };
}

void tc () {
    int n;
    cin >> n;
    n++;
    ll tot_val = 0;
    for (int i = 0; i < n; i++) {
        graph[i].clear();
    }
    for (int i = 0; i < n - 1; i++) {
        cin >> values[i];
        tot_val += values[i];
    }
    for (int i = 0; i < n - 1; i++) {
        int u, v, l;
        cin >> u >> v >> l;
        graph[u].push_back({ v, l });
    }
    Sol s = solve(0);
    // cerr << s.acc_pen << " " << s.nodes << " " << s.weights << "\n";
    cout << tot_val - s.acc_pen << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}
