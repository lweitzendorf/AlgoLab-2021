#include <iostream>
#include <vector>
#include <unordered_set>
#include <limits>

typedef struct  {
  long selected;
  long covered;
  long not_covered;
} min_cost;

min_cost dfs(const std::vector<std::unordered_set<int>>& edges, 
             const std::vector<int>& cost, int i) {
  min_cost i_cost = { cost.at(i), 0, 0 };
               
  if (edges.at(i).empty()) {
    i_cost.covered = i_cost.selected;
  } else {
    long min_select_diff = std::numeric_limits<long>::max();
    
    for (int j : edges.at(i)) {
      min_cost j_cost = dfs(edges, cost, j);
      
      i_cost.selected += std::min(j_cost.selected, std::min(j_cost.not_covered, j_cost.covered));
      i_cost.covered += std::min(j_cost.selected, j_cost.covered);
      i_cost.not_covered += j_cost.covered;
      
      min_select_diff = std::min(min_select_diff, j_cost.selected - j_cost.covered);
    }
    
    i_cost.covered = std::max(i_cost.covered, i_cost.covered + min_select_diff);
  }
  
  return i_cost;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    int n;
    std::cin >> n;
    
    std::vector<std::unordered_set<int>> edges(n);
    std::vector<int> cost;

    for (int e = 0; e < n-1; e++) {
      int i, j;
      std::cin >> i >> j;
      edges.at(i).insert(j);
    }
    
    for (int i = 0; i < n; i++) {
      int c;
      std::cin >> c;
      cost.push_back(c);
    }
    
    min_cost final_cost = dfs(edges, cost, 0);
    std::cout << std::min(final_cost.selected, final_cost.covered) << std::endl;
  }
}