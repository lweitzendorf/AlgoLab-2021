#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

void fill_ancestor_list(int species, 
                        const std::vector<int>& age,
                        const std::vector<std::vector<int>>& tree,
                        std::vector<int>& last_offspring,
                        std::vector<std::vector<std::pair<int, int>>>& ancestors,
                        std::vector<std::pair<int, int>>& current_ancestors) {
                          
  current_ancestors.emplace_back(species, age[species]);
  size_t depth = current_ancestors.size();
  
  if (tree[species].empty()) {
    ancestors[species] = current_ancestors;
    last_offspring[species] = species;
  } else {
    for (int offspring : tree[species]) {
      fill_ancestor_list(offspring, age, tree, last_offspring, ancestors, current_ancestors);
      last_offspring[species] = last_offspring[offspring];
      current_ancestors.resize(depth);
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    int n, q;
    std::cin >> n >> q;
    
    std::unordered_map<std::string, int> index;
    std::vector<std::string> species_list(n);
    
    std::vector<int> age(n);
    std::vector<std::vector<int>> tree(n);
    
    for (int i = 0; i < n; i++) {
      std::cin >> species_list[i] >> age[i];
      index[species_list[i]] = i;
    }
    
    std::vector<bool> has_ancestor(n, false);
    
    for (int i = 0; i < n-1; i++) {
      std::string offspring, ancestor;
      std::cin >> offspring >> ancestor;
      tree[index[ancestor]].push_back(index[offspring]);
      has_ancestor[index[offspring]] = true;
    }
    
    int root = -1;
    
    for (int i = 0; i < n; i++) {
      if (!has_ancestor[i]) {
        root = i;
        break;
      }
    }
    
    std::vector<int> last_offspring(n, -1);
    std::vector<std::vector<std::pair<int, int>>> ancestors(n);
    std::vector<std::pair<int, int>> current_ancestors(0);
    
    fill_ancestor_list(root, age, tree, last_offspring, ancestors, current_ancestors);
    
    for (int i = 0; i < q; i++) {
      std::string species;
      int b;
      std::cin >> species >> b;
      
      auto& anc = ancestors[last_offspring[index[species]]];
      auto bound = std::upper_bound(anc.begin(), anc.end(), b, 
                                    [](int p1, const auto p2) { return p1 >= p2.second; });
      
      std::cout << species_list[bound->first] << " ";
    }
    
    std::cout << std::endl;
  }
}