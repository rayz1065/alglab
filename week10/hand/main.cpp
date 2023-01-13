#include <bits/stdc++.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_id_2.h>
#define MAXN 90005

using namespace std;

typedef long long ll;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_id_2<K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Vertex_handle VertexH;

int n, k_min_tents;
Triangulation t;
K::Point_2 tents[MAXN];
vector<pair<int, K::FT>> mst[MAXN]; // first -> v_id, second -> dist
int ufds_p[MAXN];
int ufds_rank[MAXN];
int ufds_tents[MAXN];

void ufds_init () {
    for (int i = 0; i < n; i++) {
        ufds_p[i] = i;
        ufds_tents[i] = 1;
        ufds_rank[i] = 0;
    }
}

int ufds_getp (int node) {
    if (ufds_p[node] == node) return node;
    return ufds_p[node] = ufds_getp(ufds_p[node]);
}

void ufds_merge (int a, int b) {
    a = ufds_getp(a), b = ufds_getp(b);
    if (a == b) return ;
    if (ufds_rank[a] > ufds_rank[b]) {
        ufds_p[b] = a;
        ufds_tents[a] += ufds_tents[b];
    } else if (ufds_rank[a] < ufds_rank[b]) {
        ufds_p[a] = b;
        ufds_tents[b] += ufds_tents[a];
    } else {
        ufds_p[a] = b;
        ufds_tents[b] += ufds_tents[a];
        ufds_rank[b] += 1;
    }
}

int calc_freq () {
    vector<int> freqs(k_min_tents + 1, 0);
    for (int i = 0; i < n; i++) {
        if (ufds_getp(i) == i) {
            freqs[min(ufds_tents[i], (int) freqs.size() - 1)]++;
        }
    }
    if (k_min_tents == 4) {
        // combine ones and threes
        int three_one = min(freqs[1], freqs[3]);
        freqs[1] -= three_one;
        freqs[3] -= three_one;
        freqs[4] += three_one;
        // combine twos and threes
        int three_two = min(freqs[2], freqs[3]);
        freqs[2] -= three_two;
        freqs[3] -= three_two;
        freqs[4] += three_two;
        // combine twos and ones together
        if (freqs[2] % 2 == 1 && freqs[1] >= 2) {
            freqs[2]--, freqs[1] -= 2;
            freqs[4]++;
        }
        // use the rest
        freqs[4] += freqs[1] / 4;
        freqs[4] += freqs[2] / 2;
        freqs[4] += freqs[3] / 2;
    } else if (k_min_tents == 3) {
        int two_one = min(freqs[1], freqs[2]);
        freqs[1] -= two_one; // combine twos and ones
        freqs[2] -= two_one;
        freqs[3] += two_one;
        // use the rest
        freqs[3] += freqs[1] / 3;
        freqs[3] += freqs[2] / 2;
    } else if (k_min_tents == 2) {
        // combine the ones
        freqs[2] += freqs[1] / 2;
    }
    // cerr << freqs[freqs.size() - 1] << "\n";
    return freqs[freqs.size() - 1];
}

int calc_max_f (ll sq_dist) {
    ufds_init();
    for (int i = 0; i < n; i++) {
        for (auto adj: mst[i]) {
            if (adj.second < sq_dist) {
                ufds_merge(i, adj.first);
            }
        }
    }
    return calc_freq();
}

ll find_reasonable_sqdist (int f) {
    ll s = 0; // solution of 0 always plausible
    ll l = 1, r = LLONG_MAX >> 1;
    while (l <= r) {
        ll m = (l + r) >> 1;
        // cerr << l << " " << m << " " << r << "\n";
        if (calc_max_f(m) >= f) {
            // increase sqdist
            s = m, l = m + 1;
        } else {
            // lower sqdist
            r = m - 1;
        }
    }
    return s;
}

void construct_mst () {
    for (auto v: t.all_vertex_handles()) {
        if (t.is_infinite(v)) continue ;
        K::Point_2 p = t.point(v);
        auto v2 = t.incident_vertices(v);
        mst[v->id()].clear();
        do {
            if (t.is_infinite(v2)) continue ;
            K::Point_2 p2 = t.point(v2);
            mst[v->id()].push_back({ v2->id(), CGAL::squared_distance(p, p2) });
        } while (++v2 != t.incident_vertices(v));
    }
}

void tc () {
    int f0;
    ll s1;
    cin >> n >> k_min_tents >> f0 >> s1;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        tents[i] = { x, y };
    }
    t.clear();
    t.insert(tents, tents + n);

    int next_id = 0;
    for (auto v: t.all_vertex_handles()) {
        if (t.is_infinite(v)) continue ;
        v->id() = next_id++;
    }

    construct_mst();

    ll max_s_0 = find_reasonable_sqdist(f0); // reasonable squared distance for f0
    int max_f_1 = calc_max_f(s1); // max families for s1
    cout << max_s_0 << " " << max_f_1 << "\n";
}

int main () {
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
