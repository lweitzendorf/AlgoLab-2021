#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

struct Warehouse {
  K::Point_2 p;
  int s, a;
};

struct Stadium {
  K::Point_2 p;
  int d, u;
};

struct ContourLine {
  K::Point_2 p;
  long r;

  bool in_range(K::Point_2 q) {
    return CGAL::squared_distance(p, q) < CGAL::square(r);
  }
};

void solve() {
  int n, m, c;
  std::cin >> n >> m >> c;

  std::vector<Warehouse> warehouses;
  std::vector<Stadium> stadiums;
  std::vector<ContourLine> lines;
  std::vector<std::vector<int>> revenue(n, std::vector<int>(m));

  for (int w = 0; w < n; w++) {
    int x, y, s, a;
    std::cin >> x >> y >> s >> a;

    warehouses.push_back({ K::Point_2(x, y), s, a });
  }

  for (int s = 0; s < m; s++) {
    int x, y, d, u;
    std::cin >> x >> y >> d >> u;

    stadiums.push_back({ K::Point_2(x, y), d, u });
  }

  for (int w = 0; w < n; w++) {
    for (int s = 0; s < m; s++) {
      std::cin >> revenue[w][s];
    }
  }

  for (int l = 0; l < c; l++) {
    int x, y, r;
    std::cin >> x >> y >> r;

    lines.push_back({ K::Point_2(x, y), r });
  }

  std::vector<std::vector<int>> w_lines(n), s_lines(m);
  std::vector<std::vector<int>> num_lines_crossed(n, std::vector<int>(m));

  for (int l = 0; l < c; l++) {
    for (int w = 0; w < n; w++) {
      if (lines[l].in_range(warehouses[w].p)) {
        w_lines[w].push_back(l);
      }
    }

    for (int s = 0; s < m; s++) {
      if (lines[l].in_range(stadiums[s].p)) {
        s_lines[s].push_back(l);
      }
    }
  }

  for (int w = 0; w < n; w++) {
    for (int s = 0; s < m; s++) {
      std::vector<int> diff;
      std::set_symmetric_difference(w_lines[w].begin(), w_lines[w].end(),
                                    s_lines[s].begin(), s_lines[s].end(),
                                    std::back_inserter(diff));
      num_lines_crossed[w][s] = diff.size();
    }
  }

  Program lp(CGAL::SMALLER, true, 0, false, 0);
  int num_ineq = 0;

  for (int w = 0; w < n; w++) {
    for (int s = 0; s < m; s++) {
      lp.set_a(w*m + s, num_ineq, 1);
    }
    lp.set_b(num_ineq++, warehouses[w].s);
  }

  for (int s = 0; s < m; s++) {
    for (int w = 0; w < n; w++) {
      lp.set_a(w*m + s, num_ineq, 1);
      lp.set_a(w*m + s, num_ineq+1, -1);
      lp.set_a(w*m + s, num_ineq+2, warehouses[w].a);
      lp.set_c(w*m + s, num_lines_crossed[w][s] - 100*revenue[w][s]);
    }
    lp.set_b(num_ineq++, stadiums[s].d);
    lp.set_b(num_ineq++, -stadiums[s].d);
    lp.set_b(num_ineq++, 100*stadiums[s].u);
  }

  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.is_infeasible()) {
    std::cout << "RIOT!" << std::endl;
  } else {
    long max_profit = std::floor(CGAL::to_double(-s.objective_value() / 100));
    std::cout << max_profit << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}