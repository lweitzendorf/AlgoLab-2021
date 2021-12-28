#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--) {
    int n, m, h, w;
    std::cin >> n >> m >> h >> w;

    std::vector<std::pair<long, long>> free_nails(n);
    std::vector<std::pair<long, long>> occupied_nails(m);
    
    for (int i = 0; i < n; i++) {
      std::cin >> free_nails[i].first;
      std::cin >> free_nails[i].second;
    }
    
    for (int i = 0; i < m; i++) {
      std::cin >> occupied_nails[i].first;
      std::cin >> occupied_nails[i].second;
    }
    
    Program lp(CGAL::SMALLER, true, 1, false, 0);

    int num_ineq = 0;

    for (int i = 0; i < n; i++) {
      long x = free_nails[i].first;
      long y = free_nails[i].second;
      
      for (int j = i+1; j < n; j++) {
        auto nail = free_nails[j];
        
        long distance_x = 2 * std::abs(nail.first - x);
        long distance_y = 2 * std::abs(nail.second - y);
        
        CGAL::Gmpq max_scale_x(distance_x, long(w));
        CGAL::Gmpq max_scale_y(distance_y, long(h));
        
        lp.set_a(i, num_ineq, 1); 
        lp.set_a(j, num_ineq, 1);
        lp.set_b(num_ineq++, std::max(max_scale_x, max_scale_y));
      }
      
      CGAL::Gmpq min_max_scale(std::numeric_limits<long>::max());

      for (int j = 0; j < m; j++) {
        auto nail = occupied_nails[j];
        
        long distance_x = 2 * std::abs(nail.first - x);
        long distance_y = 2 * std::abs(nail.second - y);
        
        CGAL::Gmpq max_scale_x(distance_x, long(w));
        CGAL::Gmpq max_scale_y(distance_y, long(h));
        
        min_max_scale = std::min(std::max(max_scale_x, max_scale_y), min_max_scale);
      }
      
      lp.set_a(i, num_ineq, 1); 
      lp.set_b(num_ineq++, min_max_scale - 1);
      
      lp.set_c(i, -1);
    }
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    
    long max_perimeter = std::ceil(CGAL::to_double(-s.objective_value() * 2 * (h + w)));
    std::cout << max_perimeter << std::endl;
  }
}