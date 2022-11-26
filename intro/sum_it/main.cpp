#include <bits/stdc++.h>

using namespace std;

void tc () {
  int n;
  cin >> n;
  long long s = 0;
  while (n--) {
    long long x;
    cin >> x;
    s += x;
  }
  cout << s << "\n";
}

int main () {
  int t;
  cin >> t;
  while (t--) tc();
}
