#include <bits/stdc++.h>
#define MAXN 1000005
#define DIST 100

using namespace std;

int n;
int cust[MAXN];

void solve_tc () {
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> cust[i];
    }
    sort(cust, cust + n);
    // find the lowest dist with greatest amount possible
    int max_cust = 0, min_dist = -1;
    set<int> s;
    int l = 0;
    cust[n] = INT_MAX >> 1; // unreachable
    for (int f = 0; f < n; f++) {
        while (cust[l + 1] <= cust[f] + DIST * 2) l++;
        int mid = (cust[l] + cust[f]) / 2;
        for (int i = mid - 1; i <= mid + 1; i++) {
            int curr = l - f + 1, curr_d = max(i - cust[f], cust[l] - i);
            // cout << "curr = " << curr << ", curr_d = " << curr_d << " max(" << i - cust[f] << ", " << cust[l] - i << ")\n";
            if (curr > max_cust || (curr == max_cust && curr_d < min_dist)) {
                s.clear();
                max_cust = curr;
                min_dist = curr_d;
            }
            if (curr == max_cust && curr_d == min_dist) {
                s.insert(i);
            }
        }
    }
    cout << max_cust << " " << min_dist << "\n";
    for (auto i: s) {
        cout << i << " ";
    }
    cout << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) {
        solve_tc();
    }
}
