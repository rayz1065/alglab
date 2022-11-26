#include <bits/stdc++.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#define MAXN 110005

using namespace std;

typedef long long ll;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator FacesIt;

int m_players, n_lamps;
int p_rad[MAXN];
K::Point_2 players[MAXN];
K::Point_2 lamps[MAXN];
ll h;

bool is_excluded (Triangulation &triang, int i) {
    auto nearest = triang.nearest_vertex(players[i]);
    auto p = triang.point(nearest);
    int r2 = p_rad[i];
    return CGAL::squared_distance(p, players[i]) < ((ll) (h + r2) * (h + r2));
}

bool all_excluded (Triangulation &triang, int round) {
    for (int i = 0; i < m_players; i++) {
        if (!is_excluded(triang, i)) {
            return false;
        }
    }
    return true;
}

Triangulation make_triang (int round) {
    Triangulation triang;
    triang.insert(lamps, lamps + round);
    return triang;
}

int find_last_round () {
    Triangulation triang;
    int l = 1, r = n_lamps;
    int s = 0;
    while (l <= r) {
        int m = (l + r) >> 1;
        triang = make_triang(m);
        if (all_excluded(triang, m)) {
            // try to the left
            r = m - 1;
        } else {
            // can go further right
            s = m;
            l = m + 1;
        }
    }
    return s;
}

void tc () {
    cin >> m_players >> n_lamps;
    for (int i = 0; i < m_players; i++) {
        int x, y;
        cin >> x >> y >> p_rad[i];
        players[i] = { x, y };
    }
    cin >> h;
    for (int i = 0; i < n_lamps; i++) {
        int x, y;
        cin >> x >> y;
        lamps[i] = { x, y };
    }
    int last_round = find_last_round();
    auto triang = make_triang(last_round);
    for (int i = 0; i < m_players; i++) {
        if (!is_excluded(triang, i)) {
            cout << i << " ";
        }
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
