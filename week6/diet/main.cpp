#include <bits/stdc++.h>
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#define MAXN 45

using namespace std;

typedef int IV;
typedef CGAL::Gmpz SV;
typedef CGAL::Quadratic_program<IV> Program;
typedef CGAL::Quadratic_program_solution<SV> Solution;

int n, m;
pair<int, int> quantities[MAXN];

bool tc () {
    cin >> n >> m;
    if (!n) return false;
    Program lp(CGAL::SMALLER, true, 0, false, 0);
    for (int i = 0; i < n; i++) {
        cin >> quantities[i].first >> quantities[i].second;
        // lp[2i]: quantity of i >= min
        lp.set_r(2 * i, CGAL::LARGER);
        lp.set_b(2 * i, quantities[i].first);
        // lp[2i + 1]: quantity of i <= max
        lp.set_b(2 * i + 1, quantities[i].second);
    }
    for (int j = 0; j < m; j++) {
        int cost;
        cin >> cost;
        for (int i = 0; i < n; i++) {
            int nutrient_am;
            cin >> nutrient_am;
            lp.set_a(j, 2 * i, nutrient_am); // add nutrient_am * Xj
            lp.set_a(j, 2 * i + 1, nutrient_am);
            lp.set_c(j, cost); // add cost * Xj
        }
    }
    Solution s = CGAL::solve_linear_program(lp, SV());
    if (s.is_infeasible()) {
        cout << "No such diet.\n";
    } else {
        cout << floor(CGAL::to_double(s.objective_value())) << "\n";
    }
    return true;
}

int main () {
    while (tc());
}
