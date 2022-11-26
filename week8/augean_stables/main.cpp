#include <bits/stdc++.h>
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#define DAY 24
#define MAXN 105

using namespace std;

typedef int IT;
typedef CGAL::Gmpz ST;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ST> Solution;

int river_consts[2][DAY]; // 0: a_i (Alpheius), 1: p_i (Peneius)
struct Stable {
    int filth, c_clean, k, l, m;
} stables[MAXN];
int n;

bool valid_sol (int hours_a, int hours_p) {
    Program lp(CGAL::LARGER, true, 0, true, 1);
    int a = 0, p = 0;
    for (int i = 0; i < hours_a; i++) a += river_consts[0][i];
    for (int i = 0; i < hours_p; i++) p += river_consts[1][i];
    for (int i = 0; i < n; i++) {
        // estimated rem filth: h1 * k + h2 * l + h3 * m
        // k' = k + a^2, l' = l + p^2, m' = m + a * p
        // requirement: h1 * k' + h2 * l' + h3 * m' >= filth - c_clean
        int k = stables[i].k + a * a;
        int l = stables[i].l + p * p;
        int m = stables[i].m + a * p;
        lp.set_a(0, i, k); // h1
        lp.set_a(1, i, l); // h2
        lp.set_a(2, i, m); // h3
        lp.set_b(i, stables[i].filth - stables[i].c_clean);
    }
    Solution s = CGAL::solve_linear_program(lp, ST());
    // cerr << s << "\n";
    return !s.is_infeasible();
}

void tc () {
    cin >> n;
    for (int i = 0; i < n; i++) {
        int filth, c_clean, k, l, m;
        cin >> filth >> c_clean >> k >> l >> m;
        stables[i] = { filth, c_clean, k, l, m };
    }
    for (int i = 0; i < DAY; i++) {
        cin >> river_consts[0][i];
    }
    for (int i = 0; i < DAY; i++) {
        cin >> river_consts[1][i];
    }
    int min_hours = INT_MAX >> 1;
    int hours_a = DAY, hours_p = DAY;
    while (hours_a >= 0 && hours_p <= DAY) {
        // cout << hours_a << "," << hours_p << "\n";
        if (valid_sol(hours_a, hours_p)) {
            min_hours = hours_a + hours_p;
            if (hours_p == 0) {
                hours_a--;
            } else {
                hours_p--;
            }
        } else {
            hours_a--, hours_p++;
        }
    }

    if (min_hours == INT_MAX >> 1) {
        cout << "Impossible!\n";
    } else {
        cout << min_hours << "\n";
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
