#include <iostream>
#include <vector>
#include <algorithm>

int main() {
  int t;
  std::cin >> t;

  while (t--) {
    int n, m;
    std::cin >> n >> m;

    long a, b;
    std::cin >> a >> b;

    long P, H, W;
    std::cin >> P >> H >> W;

    std::vector<std::pair<long, long>> potions_a;
    std::vector<int> potions_b;

    for (int i = 0; i < n; i++) {
      long p, h;
      std::cin >> p >> h;
      potions_a.emplace_back(p, h);
    }

    for (int i = 0; i < m; i++) {
      long w;
      std::cin >> w;
      potions_b.push_back(w);
    }
    
    std::vector<std::vector<long>> max_power(n, std::vector<long>(1025, -1));
    
    std::vector<std::vector<bool>> potions_used(1025, std::vector<bool>(n, false));
    std::vector<long> first_potions(1025, -1);
    
    for (int i = 0; i < n; i++) {
      long p = potions_a[i].first;
      long h = potions_a[i].second;
      if (p > max_power[0][h]) {
        max_power[0][h] = p;
        first_potions[h] = i;
      }
    }
    
    for (int h = 0; h <= 1024; h++) {
      if (first_potions[h] != -1) {
        potions_used[h][first_potions[h]] = true;
      }
    }
    
    for (int k = 1; k < n; k++) {
      std::vector<std::pair<int, long>> new_potions(1025, { -1, -1 });
      
      for (int h1 = 0; h1 <= 1024; h1++) {
        for (int i = 0; i < n; i++) {
          if (!potions_used[h1][i] && max_power[k-1][h1] != -1) {
            long p = max_power[k-1][h1] + potions_a[i].first;
            long h = std::min(1024l, h1 + potions_a[i].second);
            if (p > max_power[k][h]) {
              max_power[k][h] = p;
              new_potions[h] = std::make_pair(i, h1);
            }
          }
        }
      }
      
      std::vector<std::vector<bool>> next_potions_used(1025, std::vector<bool>(n, false));
      
      for (int h = 0; h <= 1024; h++) {
        if (new_potions[h].first != -1) {
          int potion_idx = new_potions[h].first;
          long old_h = new_potions[h].second;
          
          for (int i = 0; i < n; i++) {
            next_potions_used[h][i] = potions_used[old_h][i];
          }
          next_potions_used[h][potion_idx] = true;
        }
      }
      potions_used = next_potions_used;
    }
    
    std::vector<long> max_eligible_power(n, -1);
    for (int k = 0; k < n; k++) { 
      for (int h = H; h <= 1024; h++) {
        max_eligible_power[k] = std::max(max_eligible_power[k], max_power[k][h]);
      }
    }
    
    std::sort(potions_b.begin(), potions_b.end(), std::greater<>());
    int total_min = std::numeric_limits<int>::max();
    
    for (int k_a = 0; k_a < n; k_a++) {
      int num_a = k_a + 1;
      if (max_eligible_power[k_a] != -1) {
        long power = max_eligible_power[k_a];
        long wit = -a * num_a;
        
        for (int k_b = 0; k_b < m; k_b++) {
          int num_b = k_b + 1;
          wit += potions_b[k_b];
          power -= b;
          
          if (power < P) {
            break;
          } else if (wit >= W) {
            total_min = std::min(total_min, num_a + num_b);
            break;
          }
        }
      }
    }

    if (total_min == std::numeric_limits<int>::max()) {
      std::cout << -1 << std::endl;
    } else {
      std::cout << total_min << std::endl;
    }
  }
}