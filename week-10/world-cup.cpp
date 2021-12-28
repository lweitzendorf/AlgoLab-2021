#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <unordered_set>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
  std::ios_base::sync_with_stdio(false);

  int tc;
  std::cin >> tc;

  while (tc--) {
    int n, m, c;
    std::cin >> n >> m >> c;

    std::vector<std::tuple<K::Point_2, int, int>> warehouses;

    for (int w = 0; w < n; w++) {
      int x, y, s, a;
      std::cin >> x >> y >> s >> a;
      warehouses.emplace_back(K::Point_2(x, y), s, a);
    }

    std::vector<std::tuple<K::Point_2, int, int>> stadiums;

    for (int s = 0; s < m; s++) {
      int x, y, d, u;
      std::cin >> x >> y >> d >> u;
      stadiums.emplace_back(K::Point_2(x, y), d, u);
    }

    std::vector<std::vector<int>> revenues(n, std::vector<int>(m));

    for (int w = 0; w < n; w++) {
      for (int s = 0; s < m; s++) {
        std::cin >> revenues[w][s];
      }
    }

    std::vector<std::pair<K::Point_2, int>> lines;

    for (int l = 0; l < c; l++) {
      int x, y, r;
      std::cin >> x >> y >> r;
      lines.emplace_back(K::Point_2(x, y), long(r) * long(r));
    }

    std::vector<std::vector<int>> num_lines_crossed(n, std::vector<int>(m, 0));
    std::vector<std::vector<int>> w_lines(n);
    std::vector<std::vector<int>> s_lines(m);

    for (int w = 0; w < n; w++) {
      const K::Point_2 p = std::get<0>(warehouses[w]);
      for (int l = 0; l < c; l++) {
        if (CGAL::squared_distance(p, lines[l].first) < lines[l].second) {
          w_lines[w].push_back(l);
        }
      }
    }

    for (int s = 0; s < m; s++) {
      const K::Point_2 p = std::get<0>(stadiums[s]);
      for (int l = 0; l < c; l++) {
        if (CGAL::squared_distance(p, lines[l].first) < lines[l].second) {
          s_lines[s].push_back(l);
        }
      }
    }

    for (int w = 0; w < n; w++) {
      for (int s = 0; s < m; s++) {
        std::vector<int> diff;
        std::set_difference(w_lines[w].begin(), w_lines[w].end(),
                            s_lines[s].begin(), s_lines[s].end(),
                            std::back_inserter(diff));
        std::set_difference(s_lines[s].begin(), s_lines[s].end(),
                            w_lines[w].begin(), w_lines[w].end(),
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
      lp.set_b(num_ineq++, std::get<1>(warehouses[w])); // don't exceed supply
    }
    
    for (int s = 0; s < m; s++) {
      for (int w = 0; w < n; w++) {
        lp.set_a(w*m + s, num_ineq, 1);
        lp.set_a(w*m + s, num_ineq+1, -1);
        lp.set_a(w*m + s, num_ineq+2, std::get<2>(warehouses[w]));
      }
      lp.set_b(num_ineq++, std::get<1>(stadiums[s])); 
      lp.set_b(num_ineq++, -std::get<1>(stadiums[s])); // fulfill demand exactly
      lp.set_b(num_ineq++, 100 * std::get<2>(stadiums[s])); // don't exceed max alcohol
    }
    
    for (int w = 0; w < n; w++) {
      for (int s = 0; s < m; s++) {
        lp.set_c(w*m + s, -100*revenues[w][s] + num_lines_crossed[w][s]);
      }
    }
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    
    if (s.is_infeasible()) {
      std::cout << "RIOT!" << std::endl;
    } else {
      double max_profit = CGAL::to_double(-s.objective_value() / 100);
      std::cout << long(std::floor(max_profit)) << std::endl;
    }
  }
}