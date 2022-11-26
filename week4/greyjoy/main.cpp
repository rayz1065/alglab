#include <bits/stdc++.h>
#define MAXN 50005

using namespace std;

int n, k, m;
int islands[MAXN];
int dist[MAXN];
int in_waterway[MAXN];
vector<int> waterways[MAXN];

typedef long long ll;

int best_in_waterway (int i) {
    int l = 0, r = 0, s = 0;
    ll curr_sum = 0;
    while (r <= waterways[i].size()) {
        // cout << "[" << l << ", " << r << "): " << curr_sum << "\n";
        if (curr_sum > k) curr_sum -= islands[waterways[i][l++]]; // too high
        else if (curr_sum < k) {
            if (r == waterways[i].size()) {
                break ;
            }
            curr_sum += islands[waterways[i][r++]]; // too low
        }
        else {
            // valid sum
            s = max(s, r - l);
            // cout << "in range " << l << " " << r << "\n";
            if (r == waterways[i].size()) {
                break ;
            }
            curr_sum += islands[waterways[i][r++]];
        }
    }
    return s;
}

void tc () {
    cin >> n >> k >> m;
    for (int i = 0; i < n; i++) {
        cin >> islands[i];
    }
    for (int i = 0; i < m; i++) {
        int l;
        cin >> l;
        waterways[i].clear();
        for (int j = 0; j < l; j++) {
            int node;
            cin >> node;
            waterways[i].push_back(node);
            dist[node] = j;
            if (j > 0) in_waterway[node] = i;
        }
    }
    // case 1, sum within a waterway
    int s = 0;
    for (int i = 0; i < m; i++) {
        s = max(s, best_in_waterway(i));
    }
    unordered_map<ll, vector<int>> prefix_sums;
    // combine two waterways
    for (int i = 0; i < m; i++) {
        ll curr_sum = 0;
        for (int j = 1; j < waterways[i].size(); j++) {
            curr_sum += islands[waterways[i][j]];
            prefix_sums[curr_sum].push_back(waterways[i][j]);
        }
    }
    for (int i = 0; i < m; i++) {
        ll curr_sum = islands[0];
        for (int j = 1; j < waterways[i].size(); j++) {
            int curr = waterways[i][j];
            curr_sum += islands[curr];
            for (auto other: prefix_sums[k - curr_sum]) {
                if (in_waterway[other] == in_waterway[curr]) {
                    continue ;
                }
                s = max(s, dist[curr] + dist[other] + 1);
            }
        }
    }
    cout << s << "\n";
}

int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t;
    cin >> t;
    while (t--) {tc();}
}