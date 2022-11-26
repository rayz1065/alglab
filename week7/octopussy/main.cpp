#include <bits/stdc++.h>
#define MAXN 100005

using namespace std;

int n;
int bombs[MAXN];
// bool defused[MAXN];

void print_bombs (int i) {
    if (i >= n) return ;
    cout << bombs[i] << "("; print_bombs(2 * i + 1); cout << ", "; print_bombs(2 * i + 2); cout << ")";
}

bool defuse () {
    set<pair<int, int>> ff;
    for (int i = 0; i < n; i++) {
        ff.insert({ bombs[i], i });
    }
    // fill(defused, defused + n, false);
    // defused[n] = true;
    int t = 1;
    for (auto i: ff) {
        // assert(defused[min(n, i.second * 2 + 1)] && defused[min(n, i.second * 2 + 2)]);
        if (t > bombs[i.second]) return false;
        // defused[i.second] = true;
        t++;
    }
    return true;
}

void tc () {
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> bombs[i];
    }
    bombs[n] = INT_MAX >> 1;
    for (int i = 1; i < n; i++) {
        int p = (i - 1) / 2;
        bombs[i] = min(bombs[i], bombs[p] - 1);
    }
    // print_bombs(0);
    cout << (defuse() ? "yes" : "no") << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}