#include <bits/stdc++.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

CGAL::Gmpq division (int a, int b) {
    return CGAL::Gmpq(a, b);
}

CGAL::Gmpz factorial (int n) {
    CGAL::Gmpz s = 1;
    for (int i = 1; i <= n; i++) s *= i;
    return s;
}

CGAL::Gmpq sqroot (CGAL::Gmpz n, CGAL::Gmpq precision) {
    CGAL::Gmpq err = precision * 2;
    CGAL::Gmpq s = 1.0;
    while (err > precision) {
        s = (s + n / s) / 2;
        err = CGAL::abs(n - s * s);
    }
    return s;
}

CGAL::Gmpz bino (int n, int k) {
    return factorial(n) / (factorial(k) * factorial(n - k));
}

int main () {
    for (int i = 0; i < 10; i++) {
        cout << factorial(i) << " ";
    }
    // cout << factorial(2022) << "\n";
    cout << sqroot(2, std::ldexp(1, -10)) << "\n";
    // cout << sqroot(2, std::ldexp(1, -1000)) << "\n";
    for (int k = 0; k <= 10; k++) {
        cout << bino(10, k) << " \n"[k == 10];
    }
    // cout << bino(2022, 42) << "\n";
    K::Point_2 p(2, 1), q(1, 0), r(-1, -1);
    K::Line_2 l(p, q);
    K::FT d = CGAL::squared_distance(r, l);
    cout << d << "\n";
}
