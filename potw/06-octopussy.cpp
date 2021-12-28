#include <iostream>
#include <vector>
#include <algorithm>

int defuse(std::vector<bool>& defused, int i, int n) {
  if (i >= n || defused[i])
    return 0;
  
  defused[i] = true;
  return 1 + defuse(defused, 2*i+1, n) + defuse(defused, 2*i+2, n);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--) {
    int n;
    std::cin >> n;
    
    std::vector<std::pair<int, int>> detonation_time;
    for (int i = 0; i < n; i++) {
      int t;
      std::cin >> t;
      detonation_time.emplace_back(t, i);
    }
    
    std::sort(detonation_time.begin(), detonation_time.end());
    std::vector<bool> defused(n, false);
    
    bool will_detonate = false;
    int total_defuse_time = 0;
    
    for (auto pair : detonation_time) {
      int t = pair.first, i = pair.second;
      total_defuse_time += defuse(defused, i, n);
      if (t < total_defuse_time) {
        will_detonate = true;
        break;
      }
    }
    
    std::cout << (will_detonate ? "no" : "yes") << std::endl;
  }
}