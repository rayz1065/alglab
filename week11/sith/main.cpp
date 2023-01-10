#include <bits/stdc++.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_id_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#define MAXN 30005

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_id_2<K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Vertex_handle VH;
typedef long long ll;

using namespace std;

K::Point_2 points[MAXN];
// vector<int> graph[MAXN];
// VH v_handles[MAXN];
// map<K::Point_2, int> points_order;
int ufds_p[MAXN];
int ufds_sz[MAXN];
int ufds_rank[MAXN];

int ufds_getp (int v) {
    if (v == ufds_p[v]) return v;
    return ufds_p[v] = ufds_getp(ufds_p[v]);
}

void ufds_merge (int a, int b) {
    a = ufds_getp(a), b = ufds_getp(b);
    if (a == b) return ;
    ufds_sz[a] += ufds_sz[b];
    ufds_p[b] = a;
}

void tc () {
    int n, r;
    cin >> n >> r;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        points[i] = { x, y };
        // points_order[points[i]] = i;
        ufds_p[i] = i;
        ufds_sz[i] = 1;
        // graph[i].clear();
    }
    Triangulation t;
    // for (auto v: t.all_vertex_handles()) {
    //     if (t.is_infinite(v)) continue ;
    //     v->id() = points_order[t.point(v)];
    //     v_handles[v->id()] = v;
    // }
    // cerr << "strict graph {\n";
    // for (auto v: t.all_vertex_handles()) {
    //     if (t.is_infinite(v)) continue ;
    //     auto p1 = t.point(v);
    //     cerr << "\t" << v->id() << " [pos=\"" << p1.x()/100 << "," << p1.y()/100 << "!\"]\n";
    //     auto v2 = v->incident_vertices();
    //     do {
    //         if (t.is_infinite(v2)) continue ;
    //         auto p2 = t.point(v2);
    //         if (CGAL::squared_distance(p1, p2) <= (ll) r * r) {
    //             graph[v->id()].push_back(v2->id());
    //             if (v->id() < v2->id()) {
    //                 cerr << "\t" << v->id() << " -- " << v2->id() << "\n";
    //             }
    //         }
    //     } while (++v2 != v->incident_vertices());
    // }
    // cerr << "}\n";

    int s = 1;
    for (int i = n - 1; i >= 0; i--) {
        VH v = t.insert(points[i]);
        v->id() = i;
        if (i == n - 1) continue ;
        auto v2 = v->incident_vertices();
        do {
            if (t.is_infinite(v2)) continue ;
            auto p2 = t.point(v2);
            if (CGAL::squared_distance(points[i], p2) <= (ll) r * r) {
                ufds_merge(v->id(), v2->id());
            }
        } while (++v2 != v->incident_vertices());
        // for (auto j: graph[i]) {
        //     if (j > i && ufds_getp(i) != ufds_getp(j)) {
        //         ufds_merge(i, j);
        //     }
        // }
        s = max(s, min(i, ufds_sz[ufds_getp(i)]));
        // cerr << i << " -> " << ufds_getp(i) << " -> " << ufds_sz[ufds_getp(i)] << "\n";
    }
    cout << s << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}
