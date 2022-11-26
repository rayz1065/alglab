#include <bits/stdc++.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#define MAXN 120005

using namespace std;

typedef long long ll;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
// typedef CGAL::Triangulation_face_base_with_info_2<ll, K> Fb; // info is min diameter
// typedef CGAL::Triangulation_face_base_with_info_2<bool, K> Fb; // info is visited
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb; // info is id
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Face_handle FaceHandle;

K::Point_2 infected[MAXN];
int ufds_p[MAXN];
int ufds_rank[MAXN];
ll ufds_escape[MAXN];

struct Request {
    int x;
    int y;
    ll d;
    bool can_escape;
} requests[MAXN];
int requests_order[MAXN];

bool order_req (int i, int j) {
    return requests[i].d > requests[j].d;
}

int ufds_getp (int node) {
    if (ufds_p[node] == node) return node;
    return ufds_p[node] = ufds_getp(ufds_p[node]);
}

void ufds_merge (int a, int b) {
    a = ufds_getp(a), b = ufds_getp(b);
    ufds_escape[b] = ufds_escape[a] || ufds_escape[b];
    ufds_escape[a] = ufds_escape[a] || ufds_escape[b];
    if (ufds_rank[a] > ufds_rank[b]) {
        ufds_p[b] = a;
    } else if (ufds_rank[b] > ufds_rank[a]) {
        ufds_p[a] = b;
    } else {
        ufds_p[a] = b;
        ufds_rank[b]++;
    }
}

// bool can_escape (Triangulation &triang, int x, int y, ll d) {
//     for (FaceHandle face: triang.all_face_handles()) {
//         face->info() = false;
//     }
//     // cerr << "point at " << x << "," << y << " is in face...\t";
//     FaceHandle f = triang.locate({ x, y });
//     // cerr << *(f->vertex(0)) << "\t" << *(f->vertex(1)) << "\t" << *(f->vertex(2)) << "\n";
//     // initial distances
//     auto nearest = triang.nearest_vertex({ x, y });
//     if (CGAL::squared_distance(triang.point(nearest), K::Point_2(x, y)) < d) {
//         return false;
//     }
//     stack<FaceHandle> ff;
//     ff.push(f);
//     if (triang.is_infinite(f)) {
//         // can escape already
//         return true;
//     }
//     while (!ff.empty()) {
//         f = ff.top();
//         ff.pop();
//         for (int i = 0; i < 3; i++) {
//             // can it squeeze through?
//             FaceHandle neigh = f->neighbor(i);
//             K::FT dist = CGAL::squared_distance(triang.point(f->vertex((i + 1) % 3)),
//                                                 triang.point(f->vertex((i + 2) % 3)));
//             if (dist >= d * 4 && !neigh->info()) {
//                 // perchance
//                 ff.push(neigh);
//                 neigh->info() = true;
//                 if (triang.is_infinite(neigh)) {
//                     // can escape already
//                     return true;
//                 }
//             }
//         }
//     }
//     return false;
// }

bool tc () {
    int n_infected;
    cin >> n_infected;
    if (!n_infected) return false;
    Triangulation triang;
    for (int i = 0; i < n_infected; i++) {
        int x, y;
        cin >> x >> y;
        infected[i] = { x, y };
    }
    triang.insert(infected, infected + n_infected);
    // cerr << "ff size: " << ff.size() << "\n";
    // cerr.flush();

    // djikstra
    // while (!ff.empty()) {
    //     FaceHandle face = ff.begin()->second;
    //     ll curr_dist = face->info();
    //     if (-ff.begin()->first > curr_dist) {
    //         // cerr << "ff dist is too large\n";
    //         ff.erase(ff.begin());
    //         continue ;
    //     }
    //     ff.erase(ff.begin());
    //     for (int i = 0; i < 3; i++) {
    //         // can it squeeze through?
    //         FaceHandle neigh = face->neighbor(i);
    //         if (triang.is_infinite(neigh) || curr_dist <= neigh->info()) {
    //             // cerr << "won't go to inf neigh\n";
    //             // cerr.flush();
    //             continue ;
    //         }
    //         K::FT dist = CGAL::squared_distance(triang.point(face->vertex((i + 1) % 3)),
    //                                             triang.point(face->vertex((i + 2) % 3)));
    //         ll new_dist = min(curr_dist, (ll) floor(CGAL::to_double(dist)));
    //         if (new_dist > neigh->info()) {
    //             // perchance
    //             // cerr << "can move to "
    //             //      << "(" << *neigh->vertex(0) << "  " << *neigh->vertex(1) << "  " << *neigh->vertex(2) << ") "
    //             //      << "with new dist " << dist << "\n";
    //             ff.insert({ new_dist, neigh });
    //             neigh->info() = new_dist;
    //         }
    //     }
    // }

    // for (FaceHandle face: triang.all_face_handles()) {
    //     cerr << face->info() << " ";
    //     assert(face->info() > 0);
    //     if (triang.is_infinite(face)) {
    //         cerr << "(infinite)";
    //     } else {
    //         cerr << "(" << *face->vertex(0) << "  " << *face->vertex(1) << "  " << *face->vertex(2) << ")";
    //     }
    //     cerr << "\n";
    // }

    // UFDS
    int face_id = 1;
    vector<pair<ll, pair<int, int>>> edges;
    for (FaceHandle face: triang.all_face_handles()) {
        if (triang.is_infinite(face)) {
            face->info() = 0;
        } else {
            face->info() = face_id++;
        }
    }
    for (FaceHandle face: triang.all_face_handles()) {
        if (triang.is_infinite(face)) {
            continue ;
        }
        for (int i = 0; i < 3; i++) {
            auto neigh = face->neighbor(i);
            K::FT dist = CGAL::squared_distance(triang.point(face->vertex((i + 1) % 3)),
                                                triang.point(face->vertex((i + 2) % 3)));
            ll new_dist = (ll) floor(CGAL::to_double(dist));
            edges.push_back({ -new_dist, { face->info(), neigh->info() } });
        }
    }
    sort(edges.begin(), edges.end());
    // cerr << "there are " << edges.size() << " edges\n";
    for (int i = 0; i < face_id; i++) {
        ufds_p[i] = i;
        ufds_rank[i] = 0;
        ufds_escape[i] = 0;
    }
    ufds_escape[0] = 1; // already escaped
    int next_edge = 0;

    int m_queries;
    cin >> m_queries;
    for (int i = 0; i < m_queries; i++) {
        int x, y;
        ll d;
        cin >> x >> y >> d;
        requests[i] = { x, y, d, false };
        requests_order[i] = i;
    }
    sort(requests_order, requests_order + m_queries, order_req);
    for (int j = 0; j < m_queries; j++) {
        int i = requests_order[j];
        int x = requests[i].x, y = requests[i].y;
        ll d = requests[i].d;
        // bool r = can_escape(triang, x, y, d);
        FaceHandle f = triang.locate({ x, y });
        // cerr << "d = " << d << ", " << edges[next_edge].first << "\n";
        while (next_edge < edges.size() && -edges[next_edge].first >= d * 4) {
            // cerr << "merging edge " << next_edge << " with distance " << -edges[next_edge].first << "\n";
            int a = edges[next_edge].second.first;
            int b = edges[next_edge].second.second;
            ufds_merge(a, b);
            next_edge++;
        }
        K::Point_2 p(x, y);
        auto nearest = triang.nearest_vertex(p);
        auto initial_dist = CGAL::squared_distance(triang.point(nearest), p);
        requests[i].can_escape = ufds_escape[ufds_getp(f->info())] && initial_dist >= d;
        // bool other_r = f->info() >= d * 4 && initial_dist >= d;
        // // cerr << "to get to inf " << f->info() << "/" << d * 4 << " ";
        // // cout << (r ? 'y' : 'n');
        // // cerr << "\n";
    }
    for (int i = 0; i < m_queries; i++) {
        cout << (requests[i].can_escape ? 'y' : 'n');
    }
    cout << "\n";
    return true;
}

int main () {
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    while (tc());
}
