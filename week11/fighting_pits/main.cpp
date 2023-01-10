#include <bits/stdc++.h>
#define MAXN 5005
#define IMB 13 // max imbalance
#define HSH 17 // max value of hash

using namespace std;

int n_fighters, k_types, m_remembered;
int fighters[MAXN];
int memo[MAXN][IMB * 2][HSH][HSH];

int count_unique (vector<int> &curr) {
    set<int> uniq;
    for (auto i: curr) {
        uniq.insert(i);
    }
    return uniq.size();
}

int calc_imbalance (int more_north) {
    if (abs(more_north) >= 30) {
        return INT_MIN;
    }
    return -(1 << abs(more_north));
}

vector<int> add_fight (vector<int> &curr, int i) {
    vector<int> res = curr;
    res.push_back(fighters[i]);
    if (res.size() > m_remembered) {
        res.erase(res.begin());
    }
    return res;
}

int calc_excitement (vector<int> &curr, int more_north) {
    return count_unique(curr) * 1000 + calc_imbalance(more_north);
}

int hash_state (vector<int> &curr) {
    int s = 0;
    // skips the first element
    for (int i = max(0, (int) curr.size() - m_remembered + 1); i < curr.size(); i++) {
        s = s * k_types + curr[i];
    }
    return s;
}

int solve (int i, int more_north, vector<int> north, vector<int> south) {
    if (i == n_fighters) {
        return 0;
    }
    if (memo[i][more_north + IMB][hash_state(north)][hash_state(south)] != -1) {
        return memo[i][more_north + IMB][hash_state(north)][hash_state(south)];
    }
    int best = 0;
    // send north
    vector<int> nextv = add_fight(north, i);
    int excitement = calc_excitement(nextv, more_north + 1);
    if (excitement >= 0) {
        best = excitement + solve(i + 1, more_north + 1, nextv, south);
    }
    // send south
    nextv = add_fight(south, i);
    excitement = calc_excitement(nextv, more_north - 1);
    if (excitement >= 0) {
        best = max(best, excitement + solve(i + 1, more_north - 1, north, nextv));
    }
    return memo[i][more_north + IMB][hash_state(north)][hash_state(south)] = best;
}

void tc () {
    cin >> n_fighters >> k_types >> m_remembered;
    for (int i = 0; i < n_fighters; i++) {
        for (int a = 0; a < IMB * 2; a++) {
            for (int b = 0; b < HSH; b++) {
                fill(memo[i][a][b], memo[i][a][b] + HSH, -1);
            }
        }
    }
    for (int i = 0; i < n_fighters; i++) {
        cin >> fighters[i];
    }
    cout << solve(0, 0, {}, {}) << "\n";
}

int main () {
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
