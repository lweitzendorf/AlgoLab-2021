#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

int main() {
  int t;
  std::cin >> t;
  
  while (t--) {
    int n, m;
    std::cin >> n >> m;
    
    std::vector<int> strengths;
    std::vector<int> box_weights;
    
    for (int i = 0; i < n; i++) {
      int s;
      std::cin >> s;
      strengths.push_back(s);
    }
    strengths.push_back(0);
    
    for (int i = 0; i < m; i++) {
      int w;
      std::cin >> w;
      box_weights.push_back(w);
    }
    
    std::sort(strengths.begin(), strengths.end(), std::greater<int>());
    std::sort(box_weights.begin(), box_weights.end(), std::greater<int>());
    
    if (box_weights[0] > strengths[0]) {
      std::cout << "impossible" << std::endl;
      continue;
    }
    
    std::vector<int> carries_n_boxes(n, 0);
    
    int cur_friend = 0;

    for (int i = 0; i < m; i++) {
      if (box_weights[i] > strengths[cur_friend+1]) {
        carries_n_boxes[cur_friend]++;
      } else {
        carries_n_boxes[++cur_friend]++;
      }
    }
    
    int opt_max = 0;
    int sum = 0;
    
    for (int i = 0; i < n; i++) {
      sum += carries_n_boxes[i];
      int local_opt_max = std::ceil(double(sum) / double(i+1));
      opt_max = std::max(opt_max, local_opt_max);
    }
    
    int time_taken = 3 * opt_max - 1;
    std::cout << time_taken << std::endl;
  }
}