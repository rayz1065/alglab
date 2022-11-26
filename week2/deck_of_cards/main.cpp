#include <bits/stdc++.h>
#define MAXN 100005
using namespace std;

int numbers[MAXN];
int pref_sum[MAXN];

int get_sum (int l, int r) {
    if (l == 0) return pref_sum[r];
    return pref_sum[r] - pref_sum[l - 1];
}

void tc () {
    int n, k;
    cin >> n >> k;
    for (int i = 0; i < n; i++) {
        cin >> numbers[i];
        pref_sum[i] = numbers[i];
        if (i > 0) pref_sum[i] += pref_sum[i - 1];
    }
    int l = 0, r = 0, curr_sum = 0;
    int best_dist = INT_MAX >> 1, best_l, best_r;
    while (l < n) {
        if (r < n && curr_sum <= k) curr_sum += numbers[r++];
        else curr_sum -= numbers[l++];
        if (l < r && abs(curr_sum - k) < best_dist) {
            best_dist = abs(curr_sum - k);
            best_l = l, best_r = r;
        }
    }
    cout << best_l << " " << best_r - 1 << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}