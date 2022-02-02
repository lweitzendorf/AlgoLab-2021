#include <iostream>
#include <cmath>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void solve(int n) {
  int d;
  std::cin >> d;

  Program lp(CGAL::SMALLER, false, 0, false, 0);
  const int R = d;

  for (int i = 0; i < n; i++) {
    int squared_norm = 0;

    for (int j = 0; j < d; j++) {
      int a;
      std::cin >> a;

      squared_norm += a * a;
      lp.set_a(j, i, a);
    }

    int b;
    std::cin >> b;

    lp.set_a(R, i, std::sqrt(squared_norm));
    lp.set_b(i, b);
  }

  lp.set_c(R, -1);
  lp.set_l(R, true, 0);

  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.is_infeasible()) {
    std::cout << "none" << std::endl;
  } else if (s.is_unbounded()) {
    std::cout << "inf" << std::endl;
  } else {
    std::cout << long(CGAL::to_double(-s.objective_value())) << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int n;
  std::cin >> n;

  while (n > 0) {
    solve(n);
    std::cin >> n;
  }
}