#include <bits/stdc++.h>
#define MAXN 505
#define MAXS 15

using namespace std;

int n, m, s;

// graphs[s] combined network
vector<pair<int, int>> graphs[MAXS][MAXN];
int dist[MAXN];
int adj_mat[MAXN][MAXN];

struct PathInfo {
    int node;
    int prev;
    int w;

    bool operator < (const PathInfo &other) const {
        return this->w < other.w;
    }
};

void prim (int node, int species) {
    fill(dist, dist + n, INT_MAX >> 1);
    set<pair<int, PathInfo>> ff;
    ff.insert({ dist[node] = 0, { node, -1, INT_MAX >> 1 } });
    while (!ff.empty()) {
        auto path_info = ff.begin()->second;
        node = path_info.node;
        int stored_dist = ff.begin()->first;
        ff.erase(ff.begin());
        if (stored_dist > dist[node]) continue ;
        if (path_info.prev != -1) {
            // this branch can be used in the final graph
            adj_mat[path_info.prev][node] = min(adj_mat[path_info.prev][node], path_info.w);
            adj_mat[node][path_info.prev] = adj_mat[path_info.prev][node];
        }
        for (int i = 0; i < graphs[species][node].size(); i++) {
            auto b = graphs[species][node][i];
            int new_dist = b.second;
            if (dist[b.first] > new_dist) {
                ff.insert({ dist[b.first] = new_dist, { b.first, node, b.second } });
            }
        }
    }
}

void dijkstra (int node, int species) {
    fill(dist, dist + n, INT_MAX >> 1);
    set<pair<int, int>> ff;
    ff.insert({ dist[node] = 0, node });
    while (!ff.empty()) {
        node = ff.begin()->second;
        ff.erase(ff.begin());
        for (int i = 0; i < graphs[species][node].size(); i++) {
            auto b = graphs[species][node][i];
            int new_dist = dist[node] + b.second;
            if (dist[b.first] > new_dist) {
                ff.insert({ dist[b.first] = new_dist, b.first });
            }
        }
    }
}

void solve_tc () {
    int a, b;
    cin >> n >> m >> s >> a >> b;
    for (int i = 0; i <= s; i++) {
        for (int j = 0; j < n; j++) {
            graphs[i][j].clear();
        }
    }
    for (int i = 0; i < n; i++) {
        fill(adj_mat[i], adj_mat[i] + n, INT_MAX >> 1);
    }
    for (int i = 0 ; i < m; i++) {
        int t0, t1;
        cin >> t0 >> t1;
        for (int j = 0; j < s; j++) {
            int w;
            cin >> w;
            graphs[j][t0].push_back({ t1, w });
            graphs[j][t1].push_back({ t0, w });
        }
    }
    for (int i = 0; i < s; i++) {
        int nest;
        cin >> nest;
        prim(nest, i);
    }
    // recalc graph
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graphs[s][i].push_back({ j, adj_mat[i][j] });
        }
    }
    dijkstra(a, s);
    cout << dist[b] << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) solve_tc();
}