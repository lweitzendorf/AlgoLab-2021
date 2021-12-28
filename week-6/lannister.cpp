#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Quadratic_program<long> Program;
typedef CGAL::Quadratic_program_solution<CGAL::Gmpz> Solution;

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    int n, m;
    long s;
    std::cin >> n >> m >> s;

    Program lp (CGAL::SMALLER, false, 0, false, 0);

    // x = ay + b
    const int A = 0;
    const int B = 1; 

    std::vector<std::pair<int, int>> noble_houses;
    std::vector<std::pair<int, int>> common_houses;

    int num_ineq = 0;

    for (int i = 0; i < n; i++) {
      int x, y;
      std::cin >> x >> y;

      lp.set_a(A, num_ineq, -y);
      lp.set_a(B, num_ineq, -1);
      lp.set_b(num_ineq++, -x);
      noble_houses.emplace_back(x, y);
    }

    for (int i = 0; i < m; i++) {
      int x, y;
      std::cin >> x >> y;

      lp.set_a(A, num_ineq, y);
      lp.set_a(B, num_ineq, 1);
      lp.set_b(num_ineq++, x);
      common_houses.emplace_back(x, y);
    }

    Solution sol = CGAL::solve_linear_program(lp, CGAL::Gmpz());

    if (sol.is_infeasible()) {
      std::cout << "Yuck!" << std::endl;
      continue;
    }

    if (s != -1) {
      long sum_x = 0, sum_y = 0;

      for (int i = 0; i < n; i++) {
        sum_x -= noble_houses[i].first;
        sum_y += noble_houses[i].second;
      }
      for (int i = 0; i < m; i++) {
        sum_x += common_houses[i].first;
        sum_y -= common_houses[i].second;
      }

      lp.set_a(A, num_ineq, sum_y);
      lp.set_a(B, num_ineq, n-m);
      lp.set_b(num_ineq++, s-sum_x);
      
      sol = CGAL::solve_linear_program(lp, CGAL::Gmpz());
      
      if (sol.is_infeasible()) {
        std::cout << "Bankrupt!" << std::endl;
        continue;
      }
    }
    
    // y = -ax + c
    const int C = 2;
    const int F = 3;

    for (int i = 0; i < n; i++) {
      lp.set_a(A, num_ineq, -noble_houses[i].first);
      lp.set_a(C, num_ineq, 1);
      lp.set_a(F, num_ineq, -1);
      lp.set_b(num_ineq++, noble_houses[i].second);

      lp.set_a(A, num_ineq, noble_houses[i].first);
      lp.set_a(C, num_ineq, -1);
      lp.set_a(F, num_ineq, -1);
      lp.set_b(num_ineq++, -noble_houses[i].second);
    }
    
    for (int i = 0; i < m; i++) {
      lp.set_a(A, num_ineq, -common_houses[i].first);
      lp.set_a(C, num_ineq, 1);
      lp.set_a(F, num_ineq, -1);
      lp.set_b(num_ineq++, common_houses[i].second);

      lp.set_a(A, num_ineq, common_houses[i].first);
      lp.set_a(C, num_ineq, -1);
      lp.set_a(F, num_ineq, -1);
      lp.set_b(num_ineq++, -common_houses[i].second);
    }

    lp.set_l(F, true, 0);
    lp.set_c(F, 1);

    sol = CGAL::solve_linear_program(lp, CGAL::Gmpz());

    long longest_pipe = std::ceil(CGAL::to_double(sol.objective_value()));
    std::cout << longest_pipe << std::endl;
  }
}
