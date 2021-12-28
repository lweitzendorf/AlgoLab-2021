#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef long IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    long x, y, n;
    std::cin >> x >> y >> n;
    
    Program lp(CGAL::SMALLER, false, 0, false, 0);
    
    const int X = 0; 
    const int Y = 1;
    const int T = 2;
    
    for (int i = 0; i < n; i++) {
      long a, b, c, v;
      std::cin >> a >> b >> c >> v;
      
      lp.set_a(T, i, std::sqrt(a*a + b*b) * v);

      if (a*x + b*y + c < 0) {
        lp.set_a(X, i, a);
        lp.set_a(Y, i, b);
        lp.set_b(i, -c);
      } else {
        lp.set_a(X, i, -a);
        lp.set_a(Y, i, -b);
        lp.set_b(i, c);
      }
    }
    
    lp.set_l(T, true, 0);
    lp.set_c(T, -1);
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    
    double max_time = CGAL::to_double(-s.objective_value());
    std::cout << long(std::floor(max_time)) << std::endl; 
  }
}