#include <bits/stdc++.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#define MAXN 20005

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

long long n;
K::Ray_2 ray;
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

void tc () {
    {
        long long x, y, x1, y1;
        fastin(x), fastin(y);
        fastin(x1), fastin(y1);
        ray = K::Ray_2(K::Point_2(x, y), K::Point_2(x1, y1));
    }
    for (int i = 0; i < n; i++) {
        long long x, y, x1, y1;
        fastin(x), fastin(y);
        fastin(x1), fastin(y1);
        obstacles[i] = K::Segment_2(K::Point_2(x, y), K::Point_2(x1, y1));
    }
    for (int i = 0; i < n; i++) {
        if (CGAL::do_intersect(ray, obstacles[i])) {
            cout << "yes\n";
            return ;
        }
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
