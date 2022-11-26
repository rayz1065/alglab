#include <bits/stdc++.h>
#define MAXN 35
#define MAXM 100005
using namespace std;

typedef long long ll;

int n;
ll d_dist, t_max_time;
struct Movement {
    ll value;
    ll cost;
} movements[MAXN];
ll potions[MAXM];

vector<pair<ll, ll>> possible_sums (int a, int b, int k_potions) {
    vector<pair<ll, ll>> curr;
    curr.push_back({ 0, 0 });
    for (int i = a; i < b; i++) {
        vector<pair<ll, ll>> nxt;
        // to all current ones, add the movement[i]
        for (auto j: curr) {
            nxt.push_back(j);
            if (j.first + movements[i].cost < t_max_time) {
                nxt.push_back({ j.first + movements[i].cost, j.second + movements[i].value + potions[k_potions] });
            }
        }
        curr = nxt;
    }
    sort(curr.begin(), curr.end());
    vector<pair<ll, ll>> best_costs;
    ll curr_best = 0, curr_cost = 0;
    best_costs.push_back({ curr_cost, curr_best });
    for (int j = 0; j < curr.size(); j++) {
        if (curr[j].first > curr_cost) {
            if (best_costs[best_costs.size() - 1].second < curr_best) {
                best_costs.push_back({ curr_cost, curr_best });
            }
            curr_best = max(curr_best, curr[j].second);
            curr_cost = curr[j].first;
        } else { // == curr_cost
            curr_best = max(curr_best, curr[j].second);
        }
    }
    best_costs.push_back({ curr_cost, curr_best });
    return best_costs;
}

ll find_best_cost (vector<pair<ll, ll>> sums, ll max_cost) {
    int l = 0, r = sums.size() - 1, s = -1;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (sums[mid].first <= max_cost) {
            s = mid;
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return s;
}

bool solve_with_potions (int k_potions) {
    // cout << "potions = " << k_potions << "\n";
    vector<pair<ll, ll>> sums_1 = possible_sums(0, n / 2, k_potions);
    vector<pair<ll, ll>> sums_2 = possible_sums(n / 2, n, k_potions);
    // for (auto j: sums_1) {
    //     cout << j.first << " " << j.second << "\n";
    // }
    // cout << "\n";
    // for (auto j: sums_2) {
    //     cout << j.first << " " << j.second << "\n";
    // }
    // cout << "\n";
    for (auto i: sums_1) {
        // binary search a corresponding solution
        ll rem_time = t_max_time - i.first - 1;
        int res = find_best_cost(sums_2, rem_time);
        if (res == -1) {
            // cout << "not found?\n";
            assert(false);
        } else {
            // cout << "to complete " << i.first << " with " << rem_time << " using " << sums_2[res].first << " with gain " << sums_2[res].second << "\n";
            // cout << "tot cost = " << i.first + sums_2[res].first << ", gain = " << i.second + sums_2[res].second << "\n";
            if (i.second + sums_2[res].second >= d_dist) {
                // good enough
                return true;
            }
        }
    }
    return false;
}

int solve (int m_magic) {
    int l = 0, r = m_magic - 1;
    int sol = -1;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (solve_with_potions(mid)) {
            sol = mid;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return sol;
}

void tc () {
    int m_magic;
    cin >> n >> m_magic >> d_dist >> t_max_time;
    for (int i = 0; i < n; i++) {
        cin >> movements[i].value >> movements[i].cost;
    }
    potions[0] = 0; // dummy 0 potions
    m_magic++;
    for (int i = 1; i < m_magic; i++) {
        cin >> potions[i];
    }
    int s = solve(m_magic);
    // cout << ">>>>>>>>>>";
    if (s == -1) cout << "Panoramix captured\n";
    else cout << s << "\n";
}

int main () {
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
