#include <bits/stdc++.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Gmpq.h>
#include <CGAL/QP_functions.h>
#define MAXN 32
#define MAXS 10005
#define MAXH 100005
#define MAXE 1000005

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef K::FT IT;
typedef CGAL::Gmpq ST;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ST> Solution;

struct {
    K::Point_2 loc;
    int density;
} asteroids[MAXN];
K::Point_2 shots[MAXS];
K::Point_2 hunters[MAXH];

void tc() {
    int a_asteroids, s_shots, b_hunters;
    int e_energy;
    cin >> a_asteroids >> s_shots >> b_hunters;
    cin >> e_energy;
    for (int i = 0; i < a_asteroids; i++) {
        int x, y;
        cin >> x >> y >> asteroids[i].density;
        asteroids[i].loc = { x, y };
    }
    for (int i = 0; i < s_shots; i++) {
        int x, y;
        cin >> x >> y;
        shots[i] = { x, y };
    }
    for (int i = 0; i < b_hunters; i++) {
        int x, y;
        cin >> x >> y;
        hunters[i] = { x, y };
    }
    Triangulation t;
    t.insert(hunters, hunters + b_hunters);

    Program lp(CGAL::LARGER, true, 0, false, 0);

    for (int i = 0; i < a_asteroids; i++) {
        // total energy must be at least the density
        lp.set_b(i, asteroids[i].density);
    }

    for (int i = 0; i < s_shots; i++) {
        // calculate max radius of explosion
        vector<bool> in_range(a_asteroids, false);
        if (b_hunters > 0) {
            auto hunt_v = t.nearest_vertex(shots[i]);
            K::FT max_rad = CGAL::squared_distance(t.point(hunt_v), shots[i]);
            for (int ast_id = 0; ast_id < a_asteroids; ast_id++) {
                K::FT curr_rad = CGAL::squared_distance(shots[i], asteroids[ast_id].loc);
                in_range[ast_id] = curr_rad < max_rad;
            }
        } else {
            fill(in_range.begin(), in_range.end(), true);
        }
        // add to the system
        for (int ast_id = 0; ast_id < a_asteroids; ast_id++) {
            if (!in_range[ast_id]) {
                // assert(false);
                continue ;
            }
            // energy transmitted = e_i / max{1, d_i^2}
            K::FT curr_rad = CGAL::squared_distance(shots[i], asteroids[ast_id].loc);
            lp.set_a(i, ast_id, 1 / CGAL::max(K::FT(1), curr_rad));
        }
    }
    for (int i = 0; i < s_shots; i++) {
        // limit the output energy
        lp.set_a(i, a_asteroids, 1);
    }
    lp.set_r(a_asteroids, CGAL::SMALLER);
    lp.set_b(a_asteroids, e_energy);

    Solution s = CGAL::solve_linear_program(lp, ST());
    cout << (!s.is_infeasible() ? 'y' : 'n') << "\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}
