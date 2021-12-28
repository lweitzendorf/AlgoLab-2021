#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<CGAL::Gmpz> Solution;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int p;
  std::cin >> p;
  
  while (p > 0) {
    int a, b;
    std::cin >> a >> b;
    
    const int X = 0; 
    const int Y = 1;
    const int Z = 2;
    
    Solution s;
    int rounded_solution;
    
    if (p == 1) { // maximize
      Program lp(CGAL::SMALLER, true, 0, false, 0);
      
      lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);
      lp.set_a(X, 1,  4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);
      lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);
      
      lp.set_c(X, a); lp.set_c(Y, -b);
      
      s = CGAL::solve_linear_program(lp, CGAL::Gmpz());
      rounded_solution = std::floor(-CGAL::to_double(s.objective_value()));
    } else { // minimize
      Program lp(CGAL::LARGER, false, 0, true, 0);
      
      lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, -4);
      lp.set_a(X, 1,  4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a*b);
      lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, -1);
      
      lp.set_c(X, a); lp.set_c(Y, b); lp.set_c(Z, 1);
      
      s = CGAL::solve_linear_program(lp, CGAL::Gmpz());
      rounded_solution = std::ceil(CGAL::to_double(s.objective_value()));
    }
    
    if (s.is_infeasible()) {
      std::cout << "no" << std::endl;
    } else if (s.is_unbounded()) {
      std::cout << "unbounded" << std::endl;
    } else {
      std::cout << rounded_solution << std::endl;
    }
    
    std::cin >> p;
  }
}