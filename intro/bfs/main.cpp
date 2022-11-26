#include <bits/stdc++.h>
#define MAXN 1005

using namespace std;

vector<int> graph[MAXN];
int dist[MAXN];

void bfs (int i) {
    queue<int> ff;
    ff.push(i);
    dist[i] = 0;
    while (!ff.empty()) {
        i = ff.front();
        ff.pop();
        for (auto j: graph[i]) {
            if (dist[j] == -1) {
                ff.push(j);
                dist[j] = dist[i] + 1;
            }
        }
    }
}

void tc() {
    int n, m, start_n;
    cin >> n >> m >> start_n;
    for (int i = 0; i < n; i++) {
        graph[i].clear();
        dist[i] = -1;
    }
    while (m--) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    bfs(start_n);
    for (int i = 0; i < n; i++) {
        cout << dist[i] << " \n"[i == n - 1];
    }
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}
