#include <bits/stdc++.h>
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>

typedef int IT; // inputs
typedef CGAL::Gmpz ET; // solver
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int main () {
    Program lp (CGAL::SMALLER, true, 0, false);
    const int X = 0, Y = 1;
    lp.set_a(X, 0, 1);
    lp.set_a(Y, 0, 1);
    lp.set_b(0, 7); // X + Y <= 7

    lp.set_a(X, 1, -1);
    lp.set_a(Y, 1, 2);
    lp.set_b(1, 4); // -X + 2Y <= 4

    lp.set_u(Y, true, 4);

    lp.set_c(Y, -32);
    lp.set_c0(64); // minimize -32y + 64

    cout << (lp.is_valid() ? "valid" : "invalid") << "\n";
    Solution s = CGAL::solve_linear_program(lp, ET());
    cout << s << "\n";
}
