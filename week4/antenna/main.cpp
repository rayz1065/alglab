#include <bits/stdc++.h>
#include <CGAL/Gmpfr.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#define MAXN 200005

using namespace std;

// typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K>  Traits;
typedef CGAL::Min_circle_2<Traits>      Min_circle;

int n;
K::Point_2 points[MAXN];

long long ceil_to_double(const K::FT& x) {
  long long a = ceil(CGAL::to_double(x));
  while ((double) a < x) a += 1;
  while ((double) a - 1 >= x) a -= 1;
  return a;
}

void tc () {
    for (int i = 0; i < n; i++) {
        double x, y;
        cin >> x >> y;
        points[i] = { x, y };
    }
    Min_circle mc(points, points + n, true);
    Traits::Circle c = mc.circle();
    auto sqradius = c.squared_radius();
    K::FT root = CGAL::sqrt(sqradius);
    cout << ceil_to_double(root) << "\n";
}

int main () {
    while (cin >> n && n) tc();
}
