#include <bits/stdc++.h>
#define MAXN 100005

using namespace std;

int temperatures[MAXN];
vector<int> graph[MAXN];
int m_trip, k_danger;
bool valid_start[MAXN];

void solve (int node, deque<int> &window, multiset<int> &values) {
    // cerr << "solving for node " << node << "\twsize = "
    //      << window.size() << " == " << m_trip << "\t"
    //      << *values.rbegin() << " - " << *values.begin() << " <= " << k_danger << "\n";
    if (window.size() == m_trip && *values.rbegin() - *values.begin() <= k_danger) {
        // cerr << "valid trip: ";
        // for (auto i: window) {
        //     cerr << i << " ";
        // }
        // cerr << "\n";
        valid_start[window[0]] = true;
    }
    int front = -1;
    if (window.size() == m_trip) {
        front = window.front();
        window.pop_front();
        values.erase(values.find(temperatures[front]));
    }
    for (auto i: graph[node]) {
        window.push_back(i);
        values.insert(temperatures[i]);
        solve(i, window, values);
        window.pop_back();
        values.erase(values.find(temperatures[i]));
    }
    if (front != -1) {
        window.push_front(front);
        values.insert(temperatures[front]);
    }
}

void tc () {
    int n;
    cin >> n >> m_trip >> k_danger;
    for (int i = 0; i < n; i++) {
        cin >> temperatures[i];
        graph[i].clear();
    }
    for (int i = 0; i < n - 1; i ++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
    }
    fill(valid_start, valid_start + n, false);
    deque<int> window({ 0 });
    multiset<int> values({ temperatures[0] });
    solve(0, window, values);
    bool found = false;
    for (int i = 0; i < n; i++) {
        if (valid_start[i]) {
            found = true;
            cout << i << " ";
        }
    }
    if (!found) cout << "Abort mission";
    cout << "\n";
}

int main () {
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
