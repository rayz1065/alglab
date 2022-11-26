#include <bits/stdc++.h>
#define MAXN 1000005

using namespace std;

int n;
int height[MAXN];

typedef long long ll;

int fall () {
    int max_fall = 0;
    for (int i = 0; i <= max_fall; i++) {
        max_fall = min(n - 1, (int) max((ll) max_fall, (ll) i + height[i] - 1));
    }
    return max_fall + 1;
}

void run_tc () {
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> height[i];
    }
    cout << fall() << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) run_tc();
}
