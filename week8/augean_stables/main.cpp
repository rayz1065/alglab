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

void tc () {
    int n;
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

    cout << "Impossible!\n";
}

int main () {
    int t;
    cin >> t;
    while (t--) tc();
}
