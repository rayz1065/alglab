#include <bits/stdc++.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#define MAXN 110005

using namespace std;

typedef long long ll;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator FacesIt;

K::Point_2 restaurants[MAXN];

bool tc () {
    int n;
    cin >> n;
    if (!n) return false;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        restaurants[i] = { x, y };
    }
    Triangulation triang;
    triang.insert(restaurants, restaurants + n);
    // for (FacesIt it = triang.finite_faces_begin(); it != triang.faces_end(); it++) {
    //     cout << triang.triangle(it) << "\n";
    // }
    int m;
    cin >> m;
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        auto nearest = triang.nearest_vertex({ x, y });
        auto p = triang.point(nearest);
        cout << (ll) std::round(CGAL::squared_distance(p, K::Point_2(x, y))) << "\n";
    }
    return true;
}

int main () {
    while (tc());
}
