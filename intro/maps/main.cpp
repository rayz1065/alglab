#include <bits/stdc++.h>

using namespace std;

void tc() {
    int n;
    map<string, vector<int>> values;
    cin >> n;
    while (n--) {
        int a;
        string b;
        cin >> a >> b;
        if (a) values[b].push_back(a);
        else   values.erase(b);
    }
    string c;
    cin >> c;
    if (values.find(c) == values.end()) cout << "Empty\n";
    else {
        sort(values[c].begin(), values[c].end());
        for (auto j: values[c]) {
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