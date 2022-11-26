#include <bits/stdc++.h>

using namespace std;

void tc () {
    int n;
    cin >> n;
    vector<int> numbers(n);
    for (int i = 0; i < n; i++) {
        cin >> numbers[i];
    }
    int d;
    cin >> d;
    numbers.erase(numbers.begin() + d);
    int a, b;
    cin >> a >> b;
    numbers.erase(numbers.begin() + a, numbers.begin() + b + 1);
    if (numbers.size() == 0) cout << "Empty\n";
    else {
        bool f = true;
        for (auto j: numbers) {
            if (!f) cout << " ";
            f = false;
            cout << j;
        }
        cout << "\n";
    }
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}