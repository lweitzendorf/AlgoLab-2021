#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<CGAL::Gmpz> Solution;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int n, m;
  std::cin >> n >> m;
  
  while (n && m) {
    std::vector<std::pair<int, int>> nutrients;
    
    for (int i = 0; i < n; i++) {
      int min, max;
      std::cin >> min >> max;
      nutrients.emplace_back(min, max);
    }
    
    std::vector<std::pair<int, std::vector<int>>> foods;
    
    for (int i = 0; i < m; i++) {
      int price;
      std::cin >> price;
      
      std::vector<int> food_nutrients;
      for (int j = 0; j < n; j++) {
        int nutrient_amount;
        std::cin >> nutrient_amount;
        food_nutrients.push_back(nutrient_amount);
      }
      foods.emplace_back(price, food_nutrients);
    }
    
    Program lp(CGAL::SMALLER, true, 0, false, 0);
    
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        lp.set_a(j, 2*i, -foods[j].second[i]);
        lp.set_a(j, 2*i+1, foods[j].second[i]);
      }
      lp.set_b(2*i, -nutrients[i].first);
      lp.set_b(2*i+1, nutrients[i].second);
    }
    
    for (int j = 0; j < m; j++) {
      lp.set_c(j, foods[j].first);
    }
    
    Solution s = CGAL::solve_linear_program(lp, CGAL::Gmpz());
    
    if (s.is_optimal()) {
      std::cout << std::floor(CGAL::to_double(s.objective_value())) << std::endl;
    } else {
      std::cout << "No such diet." << std::endl;
    }
    
    std::cin >> n >> m;
  }
}