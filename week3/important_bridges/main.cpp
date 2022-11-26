#include <bits/stdc++.h>
#define MAXN 30005

using namespace std;

vector<int> graph[MAXN];
int start_time[MAXN];
int min_found[MAXN];
int dfs_parent[MAXN];
int curr_time;
vector<pair<int, int>> bridges;

void dfs (int node) {
    start_time[node] = min_found[node] = ++curr_time;
    for (auto j: graph[node]) {
        if (start_time[j] == 0) {
            dfs_parent[j] = node;
            dfs(j);
            if (min_found[j] > start_time[node]) {
                // can't go back
                bridges.push_back({ node, j });
            }
            min_found[node] = min(min_found[node], min_found[j]);
        } else if (j != dfs_parent[node]) {
            min_found[node] = min(min_found[node], min_found[j]);
        }
    }
}

void run_tc () {
    int n, m;
    cin >> n >> m;
    curr_time = 0;
    bridges.clear();
    fill(start_time, start_time + n, 0);
    for (int i = 0; i < n; i++) {
        graph[i].clear();
    }
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    dfs(0);
    for (int i = 0; i < bridges.size(); i++) {
        if (bridges[i].first > bridges[i].second) {
            swap(bridges[i].first, bridges[i].second);
        }
    }
    sort(bridges.begin(), bridges.end());
    cout << bridges.size() << "\n";
    for (auto i: bridges) {
        cout << i.first << " " << i.second << "\n";
    }
}

int main () {
    int t;
    cin >> t;
    while (t--) run_tc();
}
