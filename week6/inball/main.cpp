#include <bits/stdc++.h>
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#define MAXN 1005
#define MAXD 15

using namespace std;

typedef int IT;
typedef CGAL::Gmpz ST;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ST> Solution;

int coeffs[MAXN][MAXD];
int norms[MAXN];

bool tc () {
    int n, d;
    cin >> n;
    if (!n) return false;
    cin >> d;
    Program lp(CGAL::SMALLER, false, 0, false, 0);
    lp.set_l(d, true, 0);
    CGAL::Gmpz magic = 1;
    for (int i = 0; i < n; i++) {
        long long sqnorm = 0;
        for (int j = 0; j < d; j++) {
            cin >> coeffs[i][j];
            sqnorm += coeffs[i][j] * coeffs[i][j];
        }
        long long norm = sqrt(sqnorm);
        assert (norm * norm == sqnorm);
        norms[i] = norm;
        magic = magic * norms[i] / CGAL::gcd(magic, norms[i]);
        cin >> coeffs[i][d];
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < d; j++) {
            lp.set_a(j, i, CGAL::to_double(coeffs[i][j] * (magic / norms[i])));
        }
        lp.set_a(d, i, 1);
        lp.set_b(i, CGAL::to_double(coeffs[i][d] * (magic / norms[i])));
        // cout << coeffs[i][d] << " / " << norm << " = " << CGAL::Gmpq(coeffs[i][d]) / norm << " = b\n";
    }
    lp.set_c(d, -1);
    Solution s = CGAL::solve_linear_program(lp, ST());
    if (s.is_infeasible()) {
        cout << "none\n";
    } else if (s.is_unbounded()) {
        cout << "inf\n";
    } else {
        // cout << s << "\n";
        CGAL::Gmpz margin = -s.objective_value_numerator() / s.objective_value_denominator();
        cout << floor(CGAL::to_double(margin / magic)) << "\n";
        // CGAL::Gmpq min_dist = -1;
        // for (int i = 0; i < n; i++) {
        //     // calculate the distance
        //     CGAL::Gmpq numer = 0;
        //     CGAL::Gmpq deno = 0;
        //     auto values = s.variable_values_begin();
        //     for (int j = 0; j < d; j++, values++) {
        //         deno += coeffs[i][j] * coeffs[i][j];
        //         numer += coeffs[i][j] * (values->num / values->den);
        //     }
        //     numer += coeffs[i][d];
        //     // auto dist = CGAL::abs<CGAL::Gmpq>(numer) / CGAL::sqrt(deno);
        //     // if (min_dist == -1 || dist < min_dist) {
        //     //     min_dist = dist;
        //     // }
        // }
        // cout << min_dist << "\n";
    }
    return true;
}

int main () {
    while (tc());
}
