#include <bits/stdc++.h>
#define MAXN 105

using namespace std;

int n, m;
vector<pair<int, int>> graph[MAXN];
pair<int, pair<int, int>> all_branches[MAXN * MAXN];
int ufds_parent[MAXN];
int dist[MAXN];

int ufds_get_p (int i) {
    if (i == ufds_parent[i]) return i;
    return ufds_parent[i] = ufds_get_p(ufds_parent[i]);
}

void ufds_merge (int i, int j) {
    ufds_parent[i] = j;
}

void dijkstra (int node) {
    fill(dist, dist + n, INT_MAX >> 1);
    set<pair<int, int>> ff;
    ff.insert({ dist[node] = 0, node });
    while (!ff.empty()) {
        node = ff.begin()->second;
        ff.erase(ff.begin());
        for (auto b: graph[node]) {
            int new_dist = dist[node] + b.second;
            if (new_dist < dist[b.first]) {
                ff.insert({ dist[b.first] = new_dist, b.first });
            }
        }
    }
}

void solve_tc () {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        graph[i].clear();
        ufds_parent[i] = i;
    }
    for (int i = 0; i < m; i++) {
        int a, b, w;
        cin >> a >> b >> w;
        graph[a].push_back({ b, w });
        graph[b].push_back({ a, w });
        all_branches[i] = { w, { a, b } };
    }
    sort(all_branches, all_branches + m);
    int mst_size = 0;
    for (int i = 0; i < m; i++) {
        int a = ufds_get_p(all_branches[i].second.first);
        int b = ufds_get_p(all_branches[i].second.second);
        if (a != b) {
            mst_size += all_branches[i].first;
            ufds_merge(a, b);
        }
    }
    dijkstra(0);
    int max_dist = *max_element(dist, dist + n);
    cout << mst_size << " " << max_dist << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) solve_tc();
}
