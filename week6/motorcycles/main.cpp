#include <bits/stdc++.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#define MAXN 500005

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Ray_2 Ray;

int n;
// Ray bikers[MAXN];
// K::FT deltas[MAXN];
int order[MAXN];

struct Biker {
    K::FT delta;
    long long y;
} bikers[MAXN];

bool bikers_cmp (int i, int j) {
    return bikers[i].y < bikers[j].y;
}

void tc () {
    cin >> n;
    for (int i = 0; i < n; i++) {
        long long y0, x1, y1;
        cin >> y0 >> x1 >> y1;
        K::FT delta = K::FT((double) (y1 - y0)) / K::FT((double) (x1 - 0));
        bikers[i] = { delta, y0 };
    }
    for (int i = 0; i < n; i++) {
        order[i] = i;
    }
    sort(order, order + n, bikers_cmp);
    vector<int> s;
    for (int k = 0; k < n; k++) {
        int i = order[k];
        while (s.size() > 0) {
            K::FT delta_curr = bikers[*s.rbegin()].delta;
            K::FT delta_new = bikers[i].delta;
            bool do_intersect = delta_new < delta_curr;
            if (do_intersect) {
                if (delta_curr * delta_curr > delta_new * delta_new) {
                    // new wins
                    s.pop_back();
                } else {
                    // curr wins
                    break ;
                }
            } else {
                s.push_back(i);
                break ;
            }
        }
        if (s.size() == 0) {
            s.push_back(i);
        }
    }
    sort(s.begin(), s.end());
    for (auto j: s) {
        cout << j << " ";
    }
    cout << "\n";
}

int main () {
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
