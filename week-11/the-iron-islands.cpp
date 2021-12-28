#include <iostream> 
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <tuple>

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    int n, k, w;
    std::cin >> n >> k >> w;
    
    std::vector<int> men_required(n);
    
    for (int i = 0; i < n; i++) {
      std::cin >> men_required[i];
    }
    
    std::vector<std::vector<int>> waterway_sums(w);
    std::unordered_map<int, std::tuple<int, int, int>> num_islands;
    
    for (int i = 0; i < w; i++) {
      int l;
      std::cin >> l;
      
      int sum = 0;
      
      for (int j = 0; j < l; j++) {
        int r;
        std::cin >> r;
        
        sum += men_required[r];
        waterway_sums[i].push_back(sum);
        
        if (sum < k) {
          int sum_no_pyke = sum - waterway_sums[i][0];

          if (num_islands.find(sum_no_pyke) == num_islands.end()) {
            num_islands[sum_no_pyke] = { i, j, -1 };
          } else if (j >= std::get<1>(num_islands[sum_no_pyke])) {
            num_islands[sum_no_pyke] = { i, j, std::get<1>(num_islands[sum_no_pyke]) };
          }
        }
      }
    }
    
    int max_islands = 0;
    
    for (int i = 0; i < w; i++) {
      int a = 0, b = 0;
      const int l = waterway_sums[i].size();
      
      while (b < l) {
        int sum = waterway_sums[i][b] - (a > 0 ? waterway_sums[i][a-1] : 0);
        
        if (sum < k) {
          if (a == 0) {
            int rem_men = k - sum;
            if (num_islands.find(rem_men) != num_islands.end()) {
              auto tup = num_islands[rem_men];
              int other_islands = std::get<0>(tup) == i ? std::get<2>(tup) : std::get<1>(tup);
              
              if (other_islands != -1)
                max_islands = std::max(max_islands, b-a+1+other_islands);
            }
          }
          b++;
        } else if (sum == k) {
          max_islands = std::max(max_islands, b-a+1);
          b++;
        } else {
          a++;
        }
      }
    }
    
    std::cout << max_islands << std::endl;
  }
}