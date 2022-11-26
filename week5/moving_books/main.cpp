#include <bits/stdc++.h>
#define MAXN 300005
using namespace std;

int n, m;
int friends[MAXN];
int books[MAXN];

void tc () {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        cin >> friends[i];
    }
    for (int i = 0; i < m; i++) {
        cin >> books[i];
    }
    sort(friends, friends + n);
    sort(books, books + m);
    if (books[m - 1] > friends[n - 1]) { // too heavy
        cout << "impossible\n";
        return ;
    }
    // after this many time steps, this many friends are available to move the boxes
    // and are strong enough to do so
    int time_steps = 0, available_friends = 0;
    int next_friend = n - 1, next_book = m - 1;
    while (next_book >= 0) {
        if (next_friend >= 0 && friends[next_friend] >= books[next_book]) {
            // this friend can lift this book
            // cout << "adding friend " << next_friend << "\t";
            next_friend--;
            available_friends += time_steps;
        } else if (available_friends == 0) {
            // need more time
            time_steps++, available_friends += n - next_friend - 1;
            // cout << "adding timestep, " << n - next_friend - 1 << " friends\t";
        } else {
            // can move
            // cout << "moving " << next_book << "\t";
            next_book--, available_friends--;
        }
        // cout << "now " << available_friends << "\n";
    }
    cout << time_steps * 3 - 1 << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}
