#include <bits/stdc++.h>
#pragma GCC optimize("Ofast")
#define MAXN 1005

using namespace std;

int n;
int stacks[2][MAXN];
int pref_sums[2][MAXN];
int memo[MAXN][MAXN];
pair<int, int> step[MAXN][MAXN];
pair<int, int> get_step (int i, int j);

int calc_sum (int k, int from, int to) {
    if (from == 0) return pref_sums[k][to];
    return pref_sums[k][to] - pref_sums[k][from - 1];
}

int solve (int i, int j) {
    if (i == -1 && j == -1) return 0; // valid
    if (i == -1 || j == -1) return INT_MAX >> 1; // fail
    if (memo[i][j] != -1) return memo[i][j];
    int s = solve(i - 1, j - 1) + stacks[0][i] * stacks[1][j];
    step[i][j] = { i - 1, j - 1 };
    auto copy_i = get_step(i - 1, j);
    auto copy_j = get_step(i, j - 1);
    // cerr << "starting from " << i << "," << j << " steps possible are "
    //      << copy_i.first << "," << copy_i.second << " or "
    //      << copy_j.first << "," << copy_j.second << "\n";
    int score_i = calc_sum(0, copy_i.first + 1, i)
                * calc_sum(1, copy_i.second + 1, j)
                + solve(copy_i.first, copy_i.second); // copy (i - 1, j)
    if (score_i < s) {
        s = score_i;
        step[i][j] = copy_i;
    }
    int score_j = calc_sum(0, copy_j.first + 1, i)
                * calc_sum(1, copy_j.second + 1, j)
                + solve(copy_j.first, copy_j.second); // copy (i, j - 1)
    if (score_j < s) {
        s = score_j;
        step[i][j] = copy_j;
    }
    return memo[i][j] = s;
}

pair<int, int> get_step (int i, int j) {
    if (i == -1 && j == -1) return { i, j }; // valid
    if (i == -1 || j == -1) return { i, j }; // fail
    return step[i][j];
}

// void print_path (int i, int j) {
//     cerr << i << "," << j;
//     if (i >= 0 && j >= 0 && (stacks[0][i] == 0 || stacks[0][j] == 0)) cerr << "*";
//     auto step = get_step(i, j);
//     cerr << " step (" << step.first + 1 << "," << step.second + 1 << ")";
//     cerr << "\n";
//     if (i == -1 || j == -1) return ;
//     print_path(i - step.first - 1, j - step.second - 1);
// }

void fastin (int &x) {
    x = 0;
    for (char c = getchar_unlocked(); c >= '0'; c = getchar_unlocked()) x = x * 10 + c - '0';
}

void tc () {
    fastin(n);
    for (int i = 0; i < n; i++) {
        fill(memo[i], memo[i] + n, -1);
    }
    for (int k = 0; k < 2; k++) {
        for (int i = 0; i < n; i++) {
            fastin(stacks[k][i]);
            stacks[k][i]--;
            pref_sums[k][i] = stacks[k][i];
            if (i > 0) pref_sums[k][i] += pref_sums[k][i - 1];
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            solve(i, j);
        }
    }
    cout << solve(n - 1, n - 1) << "\n";
    // print_path(n - 1, n - 1);
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n; j++) {
    //         auto s = get_step(i, j);
    //         // cerr << s.first << "," << s.second;
    //         // if (stacks[0][i] == 0 || stacks[1][j] == 0) cerr << "*";
    //         // else if ((s.first > 50 || s.second > 50) && s.first > 50 && s.second > 50) cerr << "!";
    //         // cerr << " ";
    //     }
    //     cerr << "\n";
    // }
    // cerr << "\n";
}

int main () {
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    fastin(t);
    while (t--) tc();
}
