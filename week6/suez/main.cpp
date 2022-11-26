#include <bits/stdc++.h>
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>

#define MAXN 35
#define MAXM 1005

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ST;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ST> Solution;

using namespace std;

int n, m, h, w;
pair<int, int> nails[MAXN + MAXM];
CGAL::Gmpq max_alphas[MAXN + MAXM];

CGAL::Gmpq get_horiz_dist (int i, int j) {
    return CGAL::Gmpq(2) / w * abs(nails[j].first - nails[i].first);
}

CGAL::Gmpq get_vert_dist (int i, int j) {
    return CGAL::Gmpq(2) / h * abs(nails[j].second - nails[i].second);
}

CGAL::Gmpq get_max_dist (int i, int j) {
    return CGAL::max(get_horiz_dist(i, j), get_vert_dist(i, j));
}

void tc () {
    cin >> n >> m >> h >> w;
    for (int i = 0; i < n + m; i++) {
        cin >> nails[i].first >> nails[i].second;
        max_alphas[i] = INT_MAX;
    }
    // nails[n + m] = { INT_MAX, INT_MAX }; // dummy nail
    Program lp(CGAL::SMALLER, true, 1, false, 0);
    lp.set_u(n, true, 1); // An = 1
    int ineq_count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n + m; j++) {
            CGAL::Gmpq max_dist = get_max_dist(i, j);
            max_alphas[i] = min(max_alphas[i], max_dist - 1);
            max_alphas[min(n, j)] = min(max_alphas[min(n, j)], max_dist - 1);
        }
    }
    for (int i = 0; i < n; i++) {
        lp.set_u(i, true, max_alphas[i]);
        for (int j = i + 1; j < n + m; j++) {
            CGAL::Gmpq max_dist = get_max_dist(i, j);
            if (max_alphas[i] + max_alphas[min(n, j)] >= max_dist - 1) {
                lp.set_a(i, ineq_count, 1);
                lp.set_a(min(n, j), ineq_count, 1);
                lp.set_b(ineq_count, max_dist);
                ineq_count++;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        lp.set_c(i, -2 * (h + w));
    }
    Solution s = CGAL::solve_linear_program(lp, ST());
    // auto sol = s.variable_values_begin();
    long long per = ceil(CGAL::to_double(-s.objective_value_numerator() / s.objective_value_denominator()));
    cout << per << "\n";
}

int main () {
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) tc();
}
