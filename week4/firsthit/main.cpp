#include <bits/stdc++.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#define MAXN 30005

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel Ke;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

long long n;
K::Ray_2 ray;
Ke::Ray_2 ray_ke;
double obstacle_coords[MAXN][4];
K::Segment_2 obstacles[MAXN];

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

long long round_down (Ke::FT n) {
    long long x = floor(CGAL::to_double(n));
    while ((double) x > n) x--;
    while ((double) (x + 1) <= n) x++;
    return x;
}

void tc () {
    {
        long long x, y, x1, y1;
        fastin(x), fastin(y);
        fastin(x1), fastin(y1);
        ray = K::Ray_2(K::Point_2((double) x, (double) y), K::Point_2((double) x1, (double) y1));
        ray_ke = Ke::Ray_2(Ke::Point_2((double) x, (double) y), Ke::Point_2((double) x1, (double) y1));
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 4; j++) {
            long long k;
            fastin(k);
            obstacle_coords[i][j] = k;
        }
        obstacles[i] = K::Segment_2(K::Point_2(obstacle_coords[i][0], obstacle_coords[i][1]),\
                                    K::Point_2(obstacle_coords[i][2], obstacle_coords[i][3]));
    }
    Ke::FT min_dist = -1;
    Ke::Point_2 closest_point;
    for (int i = 0; i < n; i++) {
        if (CGAL::do_intersect(ray, obstacles[i])) {
            Ke::Segment_2 obstacle = Ke::Segment_2(Ke::Point_2(obstacle_coords[i][0], obstacle_coords[i][1]),\
                                                   Ke::Point_2(obstacle_coords[i][2], obstacle_coords[i][3]));
            auto inter = CGAL::intersection(ray_ke, obstacle);
            if (const Ke::Point_2 *p = boost::get<Ke::Point_2>(&*inter)) {
                Ke::FT new_dist = CGAL::squared_distance(ray_ke.start(), *p);
                if (min_dist == -1 || new_dist < min_dist) {
                    min_dist = new_dist;
                    closest_point = *p;
                }
            } else if (const Ke::Segment_2 *seg = boost::get<Ke::Segment_2>(&*inter)) {
                // if segment is fully contained, then it's one of the ends
                // otherwise it must be the very start
                if (*seg == obstacle) {
                    Ke::FT start_dist = CGAL::squared_distance(ray_ke.start(), obstacle.start());
                    Ke::FT end_dist = CGAL::squared_distance(ray_ke.start(), obstacle.end());
                    Ke::FT new_dist = CGAL::min(start_dist, end_dist);
                    if (min_dist == -1 || new_dist < min_dist) {
                        min_dist = new_dist;
                        closest_point = start_dist == new_dist ? obstacle.start() : obstacle.end();
                    }
                } else {
                    min_dist = 0;
                    closest_point = ray_ke.start();
                }
            } else {
                assert(0);
            }
        }
    }
    if (min_dist != -1) {
        cout << round_down(closest_point.x()) << " " << round_down(closest_point.y()) << "\n";
        return ;
    }
    cout << "no\n";
}

int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    fastin(n);
    while (n) tc(), fastin(n);
}
