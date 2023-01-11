#include <bits/stdc++.h>
#define MAXN 100005
#define MAXM 105

using namespace std;

int n_artifacts;
int k_power;
int artifacts[MAXN];
int end_point[MAXN]; // inclusive end point
int memo[MAXN][MAXM];

void calc_end_points () {
    int a = 0, b = 0; // inclusive indices
    int sum = artifacts[a];
    fill(end_point, end_point + n_artifacts, -1);
    while (b < n_artifacts) {
        if (sum == k_power) {
            end_point[a] = b;
        }
        if (sum < k_power) {
            sum += artifacts[++b];
        } else {
            sum -= artifacts[a++];
        }
    }
}

int solve (int i, int m_mages) {
    if (m_mages == 0) return 0; // nothing more to take
    if (i == n_artifacts) return INT_MIN >> 1; // unfeasible
    if (memo[i][m_mages] != INT_MIN) return memo[i][m_mages];
    int sol = solve(i + 1, m_mages); // skip this one
    if (end_point[i] != -1) {
        // take this one
        sol = max(sol,
            solve(end_point[i] + 1, m_mages - 1) + end_point[i] - i + 1
        );
    }
    return memo[i][m_mages] = sol;
}

void tc () {
    int m_mages;
    cin >> n_artifacts >> m_mages >> k_power;
    for (int i = 0; i < n_artifacts; i++) {
        fill(memo[i], memo[i] + m_mages + 1, INT_MIN);
    }
    for (int i = 0; i < n_artifacts; i++) {
        cin >> artifacts[i];
    }
    calc_end_points();
    int res = solve(0, m_mages);
    if (res < 0) cout << "fail\n";
    else cout << res << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}
