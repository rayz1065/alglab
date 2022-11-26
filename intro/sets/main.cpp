#include <bits/stdc++.h>

using namespace std;

void tc() {
    int n;
    set<int> values;
    cin >> n;
    while (n--) {
        int a, b;
        cin >> a >> b;
        if (a) values.erase(b);
        else   values.insert(b);
    }
    if (values.empty()) cout << "Empty\n";
    else {
        for (auto j: values) {
            cout << j << " ";
        }
        cout << "\n";
    }
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}