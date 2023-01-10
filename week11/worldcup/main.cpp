#include <bits/stdc++.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
#define MAXN 205
#define MAXM 21
#define MAXC 1000005

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef int IT;
typedef CGAL::Gmpq ST;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ST> Solution;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

struct Warehouse {
    K::Point_2 pos;
    int supply;
    short alcohol_perc;
} warehouses[MAXN];

struct Stadium {
    K::Point_2 pos;
    int demand;
    int alcohol_limit;
} stadiums[MAXM];

short revenues[MAXN][MAXM];

K::Circle_2 countours[MAXC];

void tc () {
    int n_warehouses, m_stadiums, c_countours;
    cin >> n_warehouses >> m_stadiums >> c_countours;
    Triangulation t;
    for (int i = 0; i < n_warehouses; i++) {
        int x, y;
        cin >> x >> y >> warehouses[i].supply >> warehouses[i].alcohol_perc;
        warehouses[i].pos = { x, y };
        t.insert(warehouses[i].pos);
    }
    for (int i = 0; i < m_stadiums; i++) {
        int x, y;
        cin >> x >> y >> stadiums[i].demand >> stadiums[i].alcohol_limit;
        stadiums[i].pos = { x, y };
        t.insert(stadiums[i].pos);
    }
    for (int i = 0; i < n_warehouses; i++) {
        for (int j = 0; j < m_stadiums; j++) {
            cin >> revenues[i][j];
        }
    }
    // cerr << c_countours << "\t";
    for (int i = 0; i < c_countours; i++) {
        int x, y;
        long long r;
        cin >> x >> y >> r;
        countours[i] = K::Circle_2(K::Point_2(x, y), (double)(r * r));
        // check if it contains a warehouse/stadium
        auto nearest = t.nearest_vertex(countours[i].center());
        K::Point_2 point = t.point(nearest);
        if (CGAL::squared_distance(point, countours[i].center()) > r * r) {
            c_countours--;
            i--;
        }
    }
    // cerr << "actual c is " << c_countours << "\n";
    // ignoring countours
    Program lp(CGAL::SMALLER, true, 0, false, 0);
    int lp_idx = 0;

    // supply limit: sum of outgoing beer sent <= warehouse supply
    for (int i = 0; i < n_warehouses; i++) {
        for (int j = 0; j < m_stadiums; j++) {
            // beer from i to j
            int var_idx = i * m_stadiums + j;
            lp.set_a(var_idx, lp_idx, 1);
        }
        lp.set_b(lp_idx++, warehouses[i].supply);
    }

    // pure alcohol limit: sum of (alcohol to stadium) * 100 <= (stadium limit) * 100
    for (int j = 0; j < m_stadiums; j++) {
        for (int i = 0; i < n_warehouses; i++) {
            int var_idx = i * m_stadiums + j;
            lp.set_a(var_idx, lp_idx, warehouses[i].alcohol_perc);
        }
        lp.set_b(lp_idx++, stadiums[j].alcohol_limit * 100);
    }

    // demand: sum of incoming beer = demand
    for (int j = 0; j < m_stadiums; j++) {
        for (int i = 0; i < n_warehouses; i++) {
            int var_idx = i * m_stadiums + j;
            lp.set_a(var_idx, lp_idx, 1);
        }
        lp.set_r(lp_idx, CGAL::EQUAL);
        lp.set_b(lp_idx++, stadiums[j].demand);
    }

    // objective value
    for (int i = 0; i < n_warehouses; i++) {
        for (int j = 0; j < m_stadiums; j++) {
            int passed_over = 0;
            for (int c_idx = 0; c_idx < c_countours; c_idx++) {
                K::FT wareh_dist = CGAL::squared_distance(warehouses[i].pos, countours[c_idx].center());
                K::FT stad_dist = CGAL::squared_distance(stadiums[j].pos, countours[c_idx].center());
                K::FT sqrad = countours[c_idx].squared_radius();
                bool warehouse_in = (wareh_dist <= sqrad), stad_in = (stad_dist <= sqrad);
                bool passes_over = warehouse_in ^ stad_in;
                passed_over += passes_over;
            }
            int var_idx = i * m_stadiums + j;
            lp.set_c(var_idx, -(revenues[i][j] * 100 - passed_over));
        }
    }

    Solution s = CGAL::solve_linear_program(lp, ST());
    // cerr << s << "\n";
    if (s.is_infeasible()) {
        cout << "RIOT!\n";
    } else {
        long long res = floor(CGAL::to_double(-s.objective_value() / 100));
        cout << res << "\n";
    }
}

int main () {
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
