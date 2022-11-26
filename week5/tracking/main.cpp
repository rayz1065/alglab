#include <bits/stdc++.h>
#define MAXN 10005
#define MAXK 15

using namespace std;

struct BranchT {
    int to;
    int w;
    bool is_river;
};

int n;
vector<BranchT> graph[MAXN];
int dist[MAXN][MAXK];

void dijkstra (int node, int k) {
    set<pair<int, pair<int, int>>> ff;
    for (int i = 0; i < n; i++) {
        fill(dist[i], dist[i] + k + 1, INT_MAX >> 1);
    }
    ff.insert({ dist[node][k] = 0, { node, k }});
    while (!ff.empty()) {
        auto curr = ff.begin();
        node = curr->second.first, k = curr->second.second;
        ff.erase(curr);
        if (curr->first > dist[node][k]) continue ; // already visited
        for (auto b: graph[node]) {
            int new_node = b.to, new_k = max(0, k - b.is_river), new_dist = dist[node][k] + b.w;
            if (dist[new_node][new_k] > new_dist) {
                ff.insert({ dist[new_node][new_k] = new_dist, { new_node, new_k }});
            }
        }
    }
}

void tc () {
    int m, k, x_start, y_end;
    cin >> n >> m >> k >> x_start >> y_end;
    for (int i = 0; i < n; i++) {
        graph[i].clear();
    }
    for (int i = 0; i < m; i++) {
        int a, b, w;
        bool is_river;
        cin >> a >> b >> w >> is_river;
        graph[a].push_back({ b, w, is_river });
        graph[b].push_back({ a, w, is_river });
    }
    dijkstra(x_start, k);
    cout << dist[y_end][0] << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}