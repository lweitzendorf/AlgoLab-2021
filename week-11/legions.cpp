#include <iostream>
#include <cmath>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void solve() {
  int x_s, y_s, n;
  std::cin >> x_s >> y_s >> n;

  Program lp(CGAL::SMALLER, false, 0, false, 0);

  const int X = 0;
  const int Y = 1;
  const int S = 2;

  for (int i = 0; i < n; i++) {
    long a, b, c, v;
    std::cin >> a >> b >> c >> v;

    long numerator = a*x_s + b*y_s + c;
    long norm = std::sqrt(a*a + b*b);

    if (numerator < 0) {
      lp.set_a(X, i, a);
      lp.set_a(Y, i, b);
      lp.set_a(S, i, v*norm);
      lp.set_b(i, -c);
    } else {
      lp.set_a(X, i, -a);
      lp.set_a(Y, i, -b);
      lp.set_a(S, i, v*norm);
      lp.set_b(i, c);
    }
  }

  lp.set_l(S, true, 0);
  lp.set_c(S, -1);

  Solution s = CGAL::solve_linear_program(lp, ET());

  long max_time = std::floor(CGAL::to_double(-s.objective_value()));
  std::cout << max_time << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}