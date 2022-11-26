#include <bits/stdc++.h>

using namespace std;

void tc () {
    string a, b;
    cin >> a >> b;
    cout << a.size() << " " << b.size() << "\n";
    cout << a + b << "\n";
    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());
    swap(a[0], b[0]);
    cout << a << " " << b << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}