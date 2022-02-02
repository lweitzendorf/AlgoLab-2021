#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void solve(int n, int m) {
  Program lp(CGAL::SMALLER, true, 0, false, 0);

  for (int i = 0; i < n; i++) {
    int min, max;
    std::cin >> min >> max;

    lp.set_b(2*i, -min);
    lp.set_b(2*i+1, max);
  }

  for (int i = 0; i < m; i++) {
    int p;
    std::cin >> p;

    lp.set_c(i, p);

    for (int j = 0; j < n; j++) {
      int c;
      std::cin >> c;

      lp.set_a(i, 2*j, -c);
      lp.set_a(i, 2*j+1, c);
    }
  }

  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.is_infeasible()) {
    std::cout << "No such diet." << std::endl;
  } else {
    std::cout << long(CGAL::to_double(s.objective_value())) << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int n, m;
  std::cin >> n >> m;

  while (n > 0) {
    solve(n, m);
    std::cin >> n >> m;
  }
}