#include <bits/stdc++.h>
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>

typedef int IT; // inputs
typedef CGAL::Gmpz ET; // solver
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int a, b;

void solve_1 () {
    Program lp (CGAL::SMALLER, true, 0, false, 0);
    const int X = 0, Y = 1;
    lp.set_a(X, 0, 1);
    lp.set_a(Y, 0, 1);
    lp.set_b(0, 4); // X + Y <= 4

    lp.set_a(X, 1, 4);
    lp.set_a(Y, 1, 2);
    lp.set_b(1, a * b); // 4X + 2Y <= AB

    lp.set_a(X, 2, -1);
    lp.set_a(Y, 2, 1);
    lp.set_b(2, 1); // -X + Y <= 1

    lp.set_c(Y, -b);
    lp.set_c(X, a); // minimize BY - AX

    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) {
        cout << "no\n";
    } else if (s.is_unbounded()) {
        cout << "unbounded\n";
    } else {
        long long sol = floor(CGAL::to_double(-s.objective_value()));
        cout << sol << "\n";
    }
}

void solve_2 () {
    Program lp (CGAL::LARGER, false, 0, true, 0);
    const int X = 0, Y = 1, Z = 2;
    lp.set_a(X, 0, 1);
    lp.set_a(Y, 0, 1);
    lp.set_b(   0, -4); // X + Y >= -4

    lp.set_a(X, 1, 4);
    lp.set_a(Y, 1, 2);
    lp.set_a(Z, 1, 1);
    lp.set_b(   1, -a * b); // 4X + 2Y + Z >= -ab

    lp.set_a(X, 2, -1);
    lp.set_a(Y, 2, 1);
    lp.set_b(   2, -1); // -X + Y >= -1

    lp.set_c(X, a);
    lp.set_c(Y, b);
    lp.set_c(Z, 1); // minimize AX + BY + Z

    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) {
        cout << "no\n";
    } else if (s.is_unbounded()) {
        cout << "unbounded\n";
    } else {
        long long sol = ceil(CGAL::to_double(s.objective_value()));
        cout << sol << "\n";
    }
}

bool tc () {
    int type;
    cin >> type;
    if (!type) return false;
    cin >> a >> b;
    if (type == 1) {
        solve_1();
    } else {
        solve_2();
    }
    return true;
}

int main () {
    while (tc());
}
