#include <bits/stdc++.h>
#define MAXN 1005

using namespace std;

vector<int> graph[MAXN];
int visit_time[MAXN];
int end_time[MAXN];
int curr_time;

void dfs (int i) {
    visit_time[i] = curr_time++;
    for (auto j: graph[i]) {
        if (visit_time[j] == -1) {
            dfs(j);
        }
    }
    end_time[i] = curr_time++;
}

void tc() {
    int n, m, start_n;
    cin >> n >> m >> start_n;
    for (int i = 0; i < n; i++) {
        graph[i].clear();
        visit_time[i] = end_time[i] = -1;
    }
    curr_time = 0;
    while (m--) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    for (int i = 0; i < n; i++) {
        sort(graph[i].begin(), graph[i].end());
    }
    dfs(start_n);
    for (int i = 0; i < n; i++) {
        cout << visit_time[i] << " \n"[i == n - 1];
    }
    for (int i = 0; i < n; i++) {
        cout << end_time[i] << " \n"[i == n - 1];
    }
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}
