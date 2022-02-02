#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void solve(int p) {
  int a, b;
  std::cin >> a >> b;

  const int X = 0;
  const int Y = 1;
  const int Z = 2;

  if (p == 1) {
    Program lp(CGAL::SMALLER, true, 0, false, 0);

    lp.set_a(X, 0, 1);
    lp.set_a(Y, 0, 1);
    lp.set_b(0, 4);

    lp.set_a(X, 1, 4);
    lp.set_a(Y, 1, 2);
    lp.set_b(1, a*b);

    lp.set_a(X, 2, -1);
    lp.set_a(Y, 2, 1);
    lp.set_b(2, 1);

    lp.set_c(X, a);
    lp.set_c(Y, -b);

    Solution s = CGAL::solve_linear_program(lp, ET());

    if (s.is_infeasible()) {
      std::cout << "no" << std::endl;
    } else {
      long opt = CGAL::to_double(-s.objective_value());
      std::cout << opt << std::endl;
    }
  } else if (p == 2) {
    Program lp(CGAL::SMALLER, false, 0, true, 0);

    lp.set_a(X, 0, -1);
    lp.set_a(Y, 0, -1);
    lp.set_b(0, 4);

    lp.set_a(X, 1, -4);
    lp.set_a(Y, 1, -2);
    lp.set_a(Z, 1, -1);
    lp.set_b(1, a*b);

    lp.set_a(X, 2, 1);
    lp.set_a(Y, 2, -1);
    lp.set_b(2, 1);

    lp.set_c(X, a);
    lp.set_c(Y, b);
    lp.set_c(Z, 1);

    Solution s = CGAL::solve_linear_program(lp, ET());

    if (s.is_infeasible()) {
      std::cout << "no" << std::endl;
    } else {
      long opt = std::ceil(CGAL::to_double(s.objective_value()));
      std::cout << opt << std::endl;

    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int p;
  std::cin >> p;

  while (p > 0) {
    solve(p);
    std::cin >> p;
  }
}