#include <bits/stdc++.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_id_2.h>
#define MAXN 40005
#define MAXM 40005

using namespace std;

typedef long long ll;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_id_2<K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triang;

int n_trees, m_bones, k_min_inspected;
ll s_sqdiam;
K::Point_2 trees[MAXN];
K::Point_2 bones[MAXN];
vector<ll> close_bones[MAXN];

int ufds_p[MAXN];
int ufds_bones[MAXN];

void ufds_init () {
    for (int i = 0; i < n_trees; i++) {
        ufds_p[i] = i;
        ufds_bones[i] = 0;
    }
}

int ufds_getp (int node) {
    if (ufds_p[node] == node) return node;
    return ufds_p[node] = ufds_getp(ufds_p[node]);
}

void ufds_merge (int a, int b) {
    a = ufds_getp(a), b = ufds_getp(b);
    if (a != b) {
        ufds_p[a] = b;
        ufds_bones[b] += ufds_bones[a];
    }
}

void tc () {
    cin >> n_trees >> m_bones >> s_sqdiam >> k_min_inspected;
    for (int i = 0; i < n_trees; i++) {
        int x, y;
        cin >> x >> y;
        trees[i] = { x, y };
    }
    for (int i = 0; i < m_bones; i++) {
        int x, y;
        cin >> x >> y;
        bones[i] = { x, y };
    }

    Triang triang;
    triang.insert(trees, trees + n_trees);
    int curr_idx = 0;
    // assign an id to each tree
    for (auto v: triang.all_vertex_handles()) {
        if (triang.is_infinite(v)) continue ;
        close_bones[curr_idx].clear();
        v->id() = curr_idx++;
    }
    // find the closest tree to each bone
    for (int i = 0; i < m_bones; i++) {
        auto nearest = triang.nearest_vertex(bones[i]);
        auto tree = triang.point(nearest);
        close_bones[nearest->id()].push_back(CGAL::squared_distance(bones[i], tree));
    }
    ufds_init();
    // add the bones which are near enough
    for (int i = 0; i < n_trees; i++) {
        for (auto j: close_bones[i]) {
            if (j * 4 <= s_sqdiam) {
                ufds_bones[i]++;
            }
        }
    }
    vector<pair<K::FT, pair<int, int>>> elems;
    for (auto v: triang.all_vertex_handles()) {
        if (triang.is_infinite(v) || n_trees == 1) continue ;
        K::Point_2 tree = triang.point(v);

        auto v2 = v->incident_vertices();
        do {
            if (triang.is_infinite(v2) || v->id() > v2->id()) continue ;
            K::Point_2 tree2 = triang.point(v2);
            K::FT sqdist = CGAL::squared_distance(tree, tree2);
            elems.push_back({ sqdist, { v->id(), v2->id() }});
        } while (++v2 != v->incident_vertices());
    }
    // merge trees which are close together
    for (auto edge: elems) {
        if (edge.first <= s_sqdiam) {
            // same component
            ufds_merge(edge.second.first, edge.second.second);
        }
    }
    // add in the closest bones
    for (int i = 0; i < n_trees; i++) {
        for (auto j: close_bones[i]) {
            elems.push_back({ (double) (4 * j), { -1, i } });
        }
    }
    int min_bones_rad_r = *max_element(ufds_bones, ufds_bones + n_trees);
    sort(elems.begin(), elems.end());
    ufds_init();
    K::FT req_radius;
    for (auto elem: elems) {
        req_radius = elem.first;
        if (elem.second.first == -1) {
            // bone
            int p = ufds_getp(elem.second.second);
            ufds_bones[p] += 1;
        } else {
            // edge
            ufds_merge(elem.second.first, elem.second.second);
        }
        int p = ufds_getp(elem.second.second);
        if (ufds_bones[p] >= k_min_inspected) break ; // reached req
    }

    // a: distinct bones with radius r
    // b: min radius to visit k bones
    ll min_sqdiam_k_bones = CGAL::to_double(req_radius);
    cout << min_bones_rad_r << " " << min_sqdiam_k_bones << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}
