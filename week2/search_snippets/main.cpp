#include <bits/stdc++.h>
#define MAXN 2060
#define MAXM 65540
using namespace std;

int n;
vector<int> word_loc[MAXN];
pair<int, int> all_word_loc[MAXM];
int last_occ[MAXN];

void solve_tc() {
    cin >> n;
    for (int i = 0; i < n; i++) {
        int m;
        cin >> m;
        word_loc[i].resize(m);
        last_occ[i] = -1;
    }
    int words_count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < word_loc[i].size(); j++) {
            cin >> word_loc[i][j];
            all_word_loc[words_count++] = { word_loc[i][j], i };
        }
    }
    queue<pair<int, int>> occ_freq;
    int present = 0;
    // map<int, int> occ_freq;
    sort(all_word_loc, all_word_loc + words_count);
    int best = INT_MAX >> 1;
    for (int i = 0; i < words_count; i++) {
        // add the curr word
        int loc = all_word_loc[i].first, w = all_word_loc[i].second;
        occ_freq.push({ loc, w });
        if (last_occ[w] == -1) present++;
        last_occ[w] = loc;
        // clean the queue from stale values
        while (occ_freq.front().first < last_occ[occ_freq.front().second]) occ_freq.pop();
        if (present == n) {
            best = min(best, loc - occ_freq.front().first + 1);
        }
    }
    cout << best << "\n";
}

int main () {
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) solve_tc();
}