#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef long IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct House {
  int x, y;
};

void solve() {
  int n, m;
  long s;
  std::cin >> n >> m >> s;

  std::vector<House> noble_houses(n), common_houses(m);

  Program lp (CGAL::SMALLER, false, 0, false, 0);

  const int A = 0;
  const int B = 1;
  const int C = 2;
  const int L = 3;

  int num_ineq = 0;

  for (auto& house : noble_houses) {
    std::cin >> house.x >> house.y;

    lp.set_a(A, num_ineq, -house.y);
    lp.set_a(B, num_ineq, -1);
    lp.set_b(num_ineq++, -house.x);
  }

  for (auto& house : common_houses) {
    std::cin >> house.x >> house.y;

    lp.set_a(A, num_ineq, house.y);
    lp.set_a(B, num_ineq, 1);
    lp.set_b(num_ineq++, house.x);
  }

  Solution sol = CGAL::solve_linear_program(lp, ET());

  if (sol.is_infeasible()) {
    std::cout << "Yuck!" << std::endl;
    return;
  }

  if (s != -1) {
    long sum_x = 0, sum_y = 0;

    for (auto house : noble_houses) {
      sum_x -= house.x;
      sum_y += house.y;
    }

    for (auto house : common_houses) {
      sum_x += house.x;
      sum_y -= house.y;
    }

    lp.set_a(A, num_ineq, sum_y);
    lp.set_a(B, num_ineq, n-m);
    lp.set_b(num_ineq++, s-sum_x);

    sol = CGAL::solve_linear_program(lp, ET());

    if (sol.is_infeasible()) {
      std::cout << "Bankrupt!" << std::endl;
      return;
    }
  }

  for (auto house : noble_houses) {
    lp.set_a(A, num_ineq, -house.x);
    lp.set_a(C, num_ineq, 1);
    lp.set_a(L, num_ineq, -1);
    lp.set_b(num_ineq++, house.y);

    lp.set_a(A, num_ineq, house.x);
    lp.set_a(C, num_ineq, -1);
    lp.set_a(L, num_ineq, -1);
    lp.set_b(num_ineq++, -house.y);
  }

  for (auto house : common_houses) {
    lp.set_a(A, num_ineq, -house.x);
    lp.set_a(C, num_ineq, 1);
    lp.set_a(L, num_ineq, -1);
    lp.set_b(num_ineq++, house.y);

    lp.set_a(A, num_ineq, house.x);
    lp.set_a(C, num_ineq, -1);
    lp.set_a(L, num_ineq, -1);
    lp.set_b(num_ineq++, -house.y);
  }

  lp.set_l(L, true, 0);
  lp.set_c(L, 1);

  sol = CGAL::solve_linear_program(lp, ET());

  long longest_pipe = std::ceil(CGAL::to_double(sol.objective_value()));
  std::cout << longest_pipe << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}