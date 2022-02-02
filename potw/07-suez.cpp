#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Poster {
  int x, y;
};

void solve() {
  int n, m, h, w;
  std::cin >> n >> m >> h >> w;

  std::vector<Poster> new_posters(n), old_posters(m);

  for (int i = 0; i < n; i++) {
    std::cin >> new_posters[i].x >> new_posters[i].y;
  }

  for (int i = 0; i < m; i++) {
    std::cin >> old_posters[i].x >> old_posters[i].y;
  }

  Program lp(CGAL::SMALLER, true, 1, false, 0);
  int num_ineq = 0;

  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      int dx = std::abs(new_posters[i].x - new_posters[j].x);
      int dy = std::abs(new_posters[i].y - new_posters[j].y);

      CGAL::Gmpq max_scale_x(2*dx, w), max_scale_y(2*dy, h);

      lp.set_a(i, num_ineq, 1);
      lp.set_a(j, num_ineq, 1);
      lp.set_b(num_ineq++, std::max(max_scale_x, max_scale_y));
    }

    CGAL::Gmpq min_max_scale = std::numeric_limits<int>::max();

    for (int j = 0; j < m; j++) {
      int dx = std::abs(new_posters[i].x - old_posters[j].x);
      int dy = std::abs(new_posters[i].y - old_posters[j].y);

      CGAL::Gmpq max_scale_x(2*dx, w), max_scale_y(2*dy, h);
      CGAL::Gmpq max_scale = std::max(max_scale_x, max_scale_y);
      min_max_scale = std::min(min_max_scale, max_scale);
    }

    lp.set_a(i, num_ineq, 1);
    lp.set_b(num_ineq++, min_max_scale-1);

    lp.set_c(i, -2*(w+h));
  }

  Solution s = CGAL::solve_linear_program(lp, ET());

  long max_perimeter = std::ceil(CGAL::to_double(-s.objective_value()));
  std::cout << max_perimeter << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}