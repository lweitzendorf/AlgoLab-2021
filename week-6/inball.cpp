#include <iostream>
#include <cmath>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<CGAL::Gmpz> Solution;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int n;
  std::cin >> n;
  
  while(n) {
    int d;
    std::cin >> d;
    
    Program lp(CGAL::SMALLER, false, 0, false, 0);
    
    const int Z = d;
    
    for (int i = 0; i < n; i++) {
      int norm = 0;
      
      for (int j = 0; j < d; j++) {
        int a;
        std::cin >> a;
        
        lp.set_a(j, i, a);
        norm += a * a;
      }
      
      norm = std::sqrt(norm);
      lp.set_a(Z, i, norm);

      int b;
      std::cin >> b;
      lp.set_b(i, b);
    }
    
    lp.set_c(Z, -1);
    
    lp.set_l(Z, true, 0);
    
    Solution s = CGAL::solve_linear_program(lp, CGAL::Gmpz());
    
    if (s.is_infeasible()) {
      std::cout << "none" << std::endl;
    } else if (s.is_unbounded()) {
      std::cout << "inf" << std::endl;
    } else {
      int radius = CGAL::to_double(-s.objective_value());
      std::cout << radius << std::endl;
    }
    
    std::cin >> n;
  }
}