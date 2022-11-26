#include <bits/stdc++.h>
#define MAXN 1005

using namespace std;

int numbers[MAXN];

void tc() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> numbers[i];
    }
    int x;
    cin >> x;
    sort(numbers, numbers + n);
    if (x) {
        reverse(numbers, numbers + n);
    }
    for (int i = 0; i < n; i++) {
        cout << numbers[i] << " \n"[i + 1 == n];
    }
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}
