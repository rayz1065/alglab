#include <bits/stdc++.h>
#define MAXN 105
#define MAXH 1050

using namespace std;

typedef long long ll;

int n_a, n_b;
int potion_a_cost, potion_b_cost; // loss in wit, loss in power
int req_p, req_h, req_w;
ll memo_a[MAXN][MAXN][MAXH];
ll memo_b[MAXN][MAXN];

struct PotionA {
    int h;
    int power;
} potions_a[MAXN];
int potions_b[MAXN];

ll solve_a (int i, int h, int rem_potions) {
    // i'm at potion i with h happiness, rem_potions can still be taken, maximize power
    if (i == n_a || rem_potions == 0) return h < req_h ? LLONG_MIN >> 1 : 0;
    if (memo_a[i][rem_potions][h] != -1) return memo_a[i][rem_potions][h];
    return memo_a[i][rem_potions][h] = max(
        solve_a(i + 1, h, rem_potions), // skip
        solve_a(i + 1, min(h + potions_a[i].h, req_h), rem_potions - 1) + potions_a[i].power // drink
    );
}

ll solve_b (int i, int rem_potions) {
    // i'm at potion i, rem_potions can still be taken, maximize wit
    if (i == n_b || rem_potions == 0) return 0;
    if (memo_b[i][rem_potions] != -1) return memo_b[i][rem_potions];
    return memo_b[i][rem_potions] = max(
        solve_b(i + 1, rem_potions), // skip
        solve_b(i + 1, rem_potions - 1) + potions_b[i] // drink
    );
}

int solve () {
    // empty memo
    for (int i = 0; i <= n_a; i++) {
        for (int j = 0; j <= n_a; j++) {
            fill(memo_a[i][j], memo_a[i][j] + req_h + 1, -1);
        }
    }
    for (int i = 0; i <= n_b; i++) {
        fill(memo_b[i], memo_b[i] + n_b + 1, -1);
    }
    int best = INT_MAX >> 1;
    for (int i = 0; i <= n_a; i++) {
        // drink i potions of type A
        ll pw = solve_a(0, 0, i);
        // now find a corresponding number of potions of type B
        // cout << "pw(0, 0, " << i << ") = " << pw << " / " << req_w << "\n";
        for (int j = 0; j <= n_b; j++) {
            ll wit = solve_b(0, j);
            // cout << "wit(0, " << j << ") = " << wit << " / " << req_w << "\n";
            if (wit - (ll) potion_a_cost * i >= req_w && pw - (ll) potion_b_cost * j >= req_p) {
                // cout << "With " << i << " A and " << j << " B have " << wit << ", " << pw << "\n";
                best = min(best, i + j);
                break ;
            }
        }
    }
    if (best == INT_MAX >> 1) return -1;
    return best;
}

void tc () {
    cin >> n_a >> n_b;
    cin >> potion_a_cost >> potion_b_cost;
    cin >> req_p >> req_h >> req_w;
    for (int i = 0; i < n_a; i++) {
        cin >> potions_a[i].power >> potions_a[i].h;
    }
    for (int i = 0; i < n_b; i++) {
        cin >> potions_b[i];
    }
    cout << solve() << "\n";
}

int main () {
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
