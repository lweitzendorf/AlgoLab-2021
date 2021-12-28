#include <iostream>
#include <vector>
#include <algorithm>

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    int n;
    std::cin >> n;
    
    std::vector<std::pair<int, int>> boats;
    
    for (int i = 0; i < n; i++) {
      int l, p;
      std::cin >> l >> p;
      boats.emplace_back(p, l);
    }
    
    std::sort(boats.begin(), boats.end());
    int num_boats = 1;
    int last_boat_end = boats.front().first;
    
    for (int i = 1; i < n; i++) {
      int p = boats[i].first;
      int l = boats[i].second;
      
      if (last_boat_end > p)
        continue;
      
      int boat_end = std::max(last_boat_end + l, p);
      
      bool choose_boat = true;
      
      for (int j = i+1; j < n && boats[j].first < boat_end; j++) {
        int next_p = boats[j].first;
        int next_l = boats[j].second;
        int next_boat_end = std::max(last_boat_end + next_l, next_p);
        if (next_boat_end < boat_end) {
          choose_boat = false;
          break;
        }
      }
      
      if (choose_boat) {
        num_boats++;
        last_boat_end = boat_end;
      } 
    }
    
    std::cout << num_boats << "\n";
  }
}