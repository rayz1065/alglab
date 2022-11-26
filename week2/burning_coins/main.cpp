#include <bits/stdc++.h>
#define MAXN 2505

using namespace std;

int n;
int coins[MAXN];
int memo[MAXN][MAXN];

int solve (int i, int j) {
    if (i > j) return 0;
    if (memo[i][j] != -1) return memo[i][j];

    return memo[i][j] = max( \
        coins[i] + min(solve(i + 1, j - 1), solve(i + 2, j)), // take left
        coins[j] + min(solve(i + 1, j - 1), solve(i, j - 2)) // take right
    );
}

void solve_tc () {
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> coins[i];
    }
    for (int i = 0; i < n; i++) {
        fill(memo[i], memo[i] + n, -1);
    }
    cout << solve(0, n - 1) << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) solve_tc();
}
