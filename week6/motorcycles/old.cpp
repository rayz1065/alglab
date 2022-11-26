#include <bits/stdc++.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#define MAXN 500005

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Ray_2 Ray;

int n;
Ray bikers[MAXN];
int order[MAXN];

bool bikers_cmp (int i, int j) {
    return bikers[i].start().y() < bikers[j].start().y();
}

void tc () {
    cin >> n;
    for (int i = 0; i < n; i++) {
        double y0, x1, y1;
        cin >> y0 >> x1 >> y1;
        bikers[i] = Ray(Point(0, y0), Point(x1, y1));
    }
    for (int i = 0; i < n; i++) {
        order[i] = i;
    }
    sort(order, order + n, bikers_cmp);
    vector<int> s;
    for (int k = 0; k < n; k++) {
        int i = order[k];
        while (s.size() > 0) {
            Ray biker_curr = bikers[*s.rbegin()];
            Ray biker_new = bikers[i];
            K::FT delta_curr = biker_curr.direction().dy() / biker_curr.direction().dx();
            K::FT delta_new = biker_new.direction().dy() / biker_new.direction().dx();
            cout << delta_curr << " and " << delta_new << ": ";
            cout << "guess(" << (delta_new < delta_curr ? "intersect" : "don't intersect") << ") ";
            bool do_intersect = delta_new < delta_curr;
            auto inters = CGAL::intersection(bikers[*s.rbegin()], bikers[i]);
            assert(inters.is_initialized() == do_intersect);
            if (inters.is_initialized()) {
                cout << "intersect and ";
                Point *p = boost::get<Point>(&*inters);
                cout << "the path of " << i << " intersects with the path of " << *s.rbegin() << "\n";
                cout << "dist[" << i << "] = " << CGAL::squared_distance(bikers[i].start(), *p) << ", ";
                cout << "dist[" << *s.rbegin() << "] = " << CGAL::squared_distance(bikers[*s.rbegin()].start(), *p) << "\n";
                cout << "guess(" << (delta_curr * delta_curr < delta_new * delta_new ? "curr wins" : "new wins") << ")";
                assert(p);
                if (CGAL::squared_distance(bikers[i].start(), *p) <
                    CGAL::squared_distance(bikers[*s.rbegin()].start(), *p)) {
                    cout << delta_new << " new wins\n";
                    s.pop_back();
                } else {
                    cout << delta_curr << " curr wins\n";
                    break ;
                }
            } else {
                cout << "do not intersect\n";
                s.push_back(i);
                cout << "no intersections for " << i << "\n";
                break ;
            }
        }
        if (s.size() == 0) {
            s.push_back(i);
        }
    }
    sort(s.begin(), s.end());
    for (auto j: s) {
        cout << j << " ";
    }
    cout << "\n";
}

int main () {
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
