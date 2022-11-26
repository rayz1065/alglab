#include <bits/stdc++.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/intersections.h>
#define MAXN 2005

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

long long m_points, n_maps;
K::Point_2 path_points[MAXN];
K::Triangle_2 triangles[MAXN];
bool map_contains[MAXN][MAXN]; // does map i contain segment j?
int freq[MAXN], curr_found; // how many times have i found segment i?

void fastin (long long &v) {
    v = 0;
    char c = 0;
    while (c < '-') c = getchar_unlocked();
    bool sgn = c == '-';
    if (sgn) c = getchar_unlocked();
    while (c >= '0') {
        v = v * 10 + c - '0';
        c = getchar_unlocked();
    }
    if (sgn) v = -v;
}

inline bool in_triangle (const K::Point_2 &p, const K::Triangle_2 &t) {
    return CGAL::do_intersect(p, t);
}

void compute_map_contains () {
    // find the intersecting triangles and segments
    for (int i = 0; i < n_maps; i++) {
        bool prev_in_tri = in_triangle(path_points[0], triangles[i]);
        for (int j = 0; j < m_points - 1; j++) {
            bool in_tri = in_triangle(path_points[j + 1], triangles[i]);
            map_contains[i][j] = prev_in_tri && in_tri;
            prev_in_tri = in_tri;
        }
    }
}

void add_map (int i, int am) {
    for (int j = 0; j < m_points - 1; j++) {
        if (map_contains[i][j]) {
            if (freq[j] == 0) curr_found++;
            freq[j] += am;
            if (freq[j] == 0) curr_found--;
        }
    }
}

int solve () {
    int a = 0, b = 0, s = INT_MAX >> 1;
    fill(freq, freq + m_points - 1, 0);
    curr_found = 0;
    while (a < n_maps) {
        if (b < n_maps && curr_found < m_points - 1) {
            // need more
            add_map(b++, 1);
        } else {
            // try removing the first
            add_map(a++, -1);
        }
        if (curr_found == m_points - 1) {
            s = min(s, b - a);
        }
    }
    // cout << curr_found << "\n";
    return s;
}

void tc () {
    fastin(m_points), fastin(n_maps);
    for (int i = 0; i < m_points; i++) {
        long long x, y;
        fastin(x), fastin(y);
        path_points[i] = K::Point_2((double) x, (double) y);
    }
    for (int i = 0; i < n_maps; i++) {
        K::Line_2 lines[6];
        for (int i = 0; i < 3; i++) {
            long long x, y, x1, y1;
            fastin(x), fastin(y), fastin(x1), fastin(y1);
            lines[i] = K::Line_2(K::Point_2((double) x, (double) y), K::Point_2((double) x1, (double) y1));
        }
        // find intersections
        K::Point_2 points[3];
        for (int i = 0; i < 3; i++) {
            auto a = CGAL::intersection(lines[i], lines[(i + 1) % 3]);
            if (const K::Point_2 *op = boost::get<K::Point_2>(&*a)) {
                points[i] = *op;
            } else {
                assert(false);
            }
        }
        triangles[i] = K::Triangle_2(points[0], points[1], points[2]);
    }

    compute_map_contains();
    cout << solve() << "\n";
}

int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    long long t;
    fastin(t);
    while (t--) tc();
}
