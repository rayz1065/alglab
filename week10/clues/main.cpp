#include <bits/stdc++.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_id_2.h>
#define MAXN 90005

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_id_2<K> VB;
typedef CGAL::Triangulation_face_base_2<K> FB;
typedef CGAL::Triangulation_data_structure_2<VB, FB> Tdt;
typedef CGAL::Delaunay_triangulation_2<K, Tdt> Triangulation;
typedef Triangulation::Vertex_handle VertexH;
typedef long long ll;

int n;
ll squared_r;
K::Point_2 stations[MAXN];
VertexH v_handles[MAXN];
Triangulation t;
int out_freq[MAXN];
int cc[MAXN];
bool valid_network;

bool in_range (K::Point_2 a, K::Point_2 b) {
    return CGAL::squared_distance(a, b) <= squared_r;
}

void cc_dfs (VertexH v, int freq, int cc_id) {
    if (cc[v->id()] != -1) {
        if (out_freq[v->id()] != freq && valid_network) {
            valid_network = false;
        }
        return ;
    }
    out_freq[v->id()] = freq;
    cc[v->id()] = cc_id;
    auto it = v->incident_vertices();
    auto p = t.point(v);
    do {
        if (t.is_infinite(it)) continue ;
        auto p2 = t.point(it);
        if (!in_range(p, p2)) continue ;
        cc_dfs(it, 3 - freq, cc_id);
    } while (++it != v->incident_vertices());
}

int find_networks () {
    valid_network = true;
    fill(out_freq, out_freq + n, 0);
    fill(cc, cc + n, -1);
    int cc_count = 0;
    for (auto i: t.all_vertex_handles()) {
        if (t.is_infinite(i)) continue ;
        if (cc[i->id()] == -1) {
            cc_dfs(i, 1, cc_count);
            cc_count++;
        }
    }
    for (auto v: t.all_vertex_handles()) {
        if (t.is_infinite(v)) continue ;
        auto it = v->incident_vertices();
        do {
            if (t.is_infinite(it)) continue ;
            if (!in_range(t.point(v), t.point(it))) continue ;
            auto it2 = v->incident_vertices();
            do {
                if (t.is_infinite(it2) || it->id() >= it2->id()) continue ;
                if (in_range(t.point(it), t.point(it2)) && in_range(t.point(v), t.point(it2))) {
                    valid_network = false;
                    // cerr << t.point(v) << " .. " << t.point(it) << " .. " << t.point(it2) << "\n";
                    return cc_count;
                }
            } while (++it2 != v->incident_vertices());
        } while (++it != v->incident_vertices());
    }
    return cc_count;
}

void tc () {
    int m, r;
    cin >> n >> m >> r;
    squared_r = (ll) r * r;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        stations[i] = { x, y };
    }
    t.clear();
    t.insert(stations, stations + n);
    int v_id = 0;
    for (auto i: t.all_vertex_handles()) {
        if (t.is_infinite(i)) continue ;
        v_handles[v_id] = i;
        i->id() = v_id++;
    }
    find_networks();
    // cerr << "strict graph {\n";
    // for (auto v: t.all_vertex_handles()) {
    //     if (t.is_infinite(v)) continue ;
    //     auto p = t.point(v);
    //     cerr << v->id() << " [color=" << (out_freq[v->id()] == 1 ? "red" : "blue") << ", pos=\"" << p.x()/1000 << "," << p.y()/1000 << "!\"]\n";
    //     for (auto it: t.all_vertex_handles()) {
    //         if (t.is_infinite(it)) continue ;
    //         auto p2 = t.point(it);
    //         if (in_range(p, p2) && v->id() < it->id()) {
    //             cerr << v->id() << " -- " << it->id() << " ";
    //             if (out_freq[v->id()] == out_freq[it->id()]) {
    //                 if (valid_network) {
    //                     cout << "clash\n";
    //                     auto it2 = v->incident_vertices();
    //                     cout << "clash between " << t.point(v) << " (" << v->id() << ") and "
    //                          << t.point(it) << " (" << it->id() << ")\n";
    //                     do
    //                     {
    //                         cout << "adjacent with " << t.point(it2) << " (" << it2->id() << ")\n";
    //                         assert (it2->id() != it->id());
    //                     } while (++it2 != v->incident_vertices());
    //                 }
    //             }
    //             cerr << "\n";
    //         }
    //     }
    // }
    // cerr << "}\n";
    // cerr << "A valid network " << (valid_network ? "exists" : "does not exist") << "\n";
    // for (int i = 0; i < n; i++) {
    //     cerr << cc[i] << " \n"[i + 1 == n];
    // }
    // cerr << "\n";
    for (int i = 0; i < m; i++) {
        int x, y, xb, yb;
        cin >> x >> y >> xb >> yb;
        K::Point_2 a(x, y);
        K::Point_2 b(xb, yb);
        auto stat_a = t.nearest_vertex({ x, y });
        auto stat_b = t.nearest_vertex({ xb, yb });
        bool in_range_a = in_range(a, t.point(stat_a));
        bool in_range_b = in_range(b, t.point(stat_b));
        // cerr << t.point(stat_a) << " and " << t.point(stat_b)
        //      << " " << in_range_a << ", " << in_range_b << "\n";
        if (!valid_network) {
            cout << 'n';
        } else if (in_range(a, b)) {
            cout << "y";
        } else if (in_range_a && in_range_b) {
            // check that the network is valid
            int net_a = cc[stat_a->id()];
            int net_b = cc[stat_b->id()];
            cout << (net_a == net_b ? 'y' : 'n');
        } else {
            cout << "n";
        }
    }
    cout << "\n";
}

int main () {
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
