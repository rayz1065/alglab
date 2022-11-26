#include <bits/stdc++.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#define MAXN 100005

using namespace std;

typedef long long ll;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

K::Point_2 germs[MAXN];
ll min_diams[MAXN];
int n;
int d_l, d_r, d_d, d_u; // dish left right down up

bool tc () {
    cin >> n;
    if (!n) return false;
    cin >> d_l >> d_d >> d_r >> d_u;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        germs[i] = { x, y };
    }
    Triangulation t;
    t.insert(germs, germs + n);
    int i = 0;
    vector<int> death_times;
    for (auto v: t.all_vertex_handles()) {
        if (t.is_infinite(v)) continue ;
        // t.finite_vertex()
        // Triangulation::Vertex_handle v = t.vertices_begin(); // t.nearest_vertex(germs[i]);
        ll min_diam = min(
            min((ll) CGAL::to_double(t.point(v).x()) - d_l, d_r - (ll) CGAL::to_double(t.point(v).x())) * 2, // x diam
            min((ll) CGAL::to_double(t.point(v).y()) - d_d, d_u - (ll) CGAL::to_double(t.point(v).y())) * 2  // y diam
        );
        min_diam = min_diam * min_diam; // squared
        Triangulation::Edge_circulator j = t.incident_edges(v);
        if (j != NULL) {
            do {
                if (!t.is_infinite(j)) {
                    K::Segment_2 seg = t.segment(j);
                    min_diam = min(min_diam, (ll) CGAL::to_double(seg.squared_length()));
                }
            } while (++j != t.incident_edges(v));
        }
        // cerr << "vertex: " << *v << "\n";
        double death_time = sqrt(sqrt((min_diam) - 1) / 2);
        int upper_death_time = ceil(death_time);
        int before_death_time = max(0, upper_death_time - 1);
        ll diam_at_death = 2 * ((ll) upper_death_time * upper_death_time) + 1;
        ll diam_bef_death = 2 * ((ll) before_death_time * before_death_time) + 1;
        if (diam_bef_death * diam_bef_death >= min_diam) {
            upper_death_time = before_death_time;
        }
        // cerr << diam_bef_death * diam_bef_death << " -> " << diam_at_death * diam_at_death << "\t";
        // cerr << "diam " << min_diam << " -> " << death_time << " -> " << ceil(CGAL::to_double(death_time)) << "\n";
        death_times.push_back(upper_death_time);
    }
    sort(death_times.begin(), death_times.end());
    cout << death_times[0] << " " << death_times[n / 2] << " " << death_times[n - 1] << "\n";
    return true;
}

int main () {
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    while (tc());
}
