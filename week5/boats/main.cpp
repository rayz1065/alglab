#include <bits/stdc++.h>
#define MAXN 200005
#define UNPLACEABLE (INT_MAX >> 1)

using namespace std;

int n;
struct Boat {
    int ring;
    int len;

    bool operator < (const Boat &b) const {
        return this->ring < b.ring;
    }
} boats[MAXN];

int boat_place (int i, int next_pos) {
    if (boats[i].ring < next_pos) return UNPLACEABLE;
    return max(next_pos, boats[i].ring - boats[i].len + 1) + boats[i].len;
}

int solve () {
    int s = 0;
    int best = -1, last_p = 0; // first and last analyzed boats
    int next_pos = INT_MIN >> 1; // first available spot
    int best_pos = INT_MAX >> 1; // new value of next_pos if we take best
    while (best < n) {
        if (last_p < n && boats[last_p].ring < best_pos) {
            // include last_p
            if (boat_place(last_p, next_pos) < best_pos) {
                // improved
                best_pos = boat_place(best = last_p, next_pos);
            }
            last_p++;
        } else {
            // best should be taken
            next_pos = best_pos, s++;
            // cout << s << ": taking " << best << " moving to " << next_pos << "\n";
            while (best < last_p && boat_place(best, next_pos) == UNPLACEABLE) {
                // discard best
                best_pos = boat_place(++best, next_pos);
            }
        }
    }
    return s;
}

void tc () {
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> boats[i].len >> boats[i].ring;
    }
    sort(boats, boats + n);
    cout << solve() << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}