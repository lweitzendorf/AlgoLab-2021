#include <iostream>
#include <vector>
#include <algorithm>

int best_score(int moves, int x_down, int x_up, const int n,
               const std::vector<std::vector<int>> &coins,
               std::vector<std::vector<std::vector<int>>> &cache) {
  int y_down = moves - x_down;
  int y_up = moves - x_up;
                 
  if (x_down >= n || y_down >= n || x_up >= n || y_up >= n) {
    return 0;
  } else if (cache[moves][x_down][x_up] != -1) {
    return cache[moves][x_down][x_up];
  } else {
    int max_subscore = 
      std::max({ best_score(moves+1, x_down,   x_up,   n, coins, cache),
                 best_score(moves+1, x_down+1, x_up,   n, coins, cache),
                 best_score(moves+1, x_down,   x_up+1, n, coins, cache),
                 best_score(moves+1, x_down+1, x_up+1, n, coins, cache) });
               
    int score = max_subscore + coins[x_down][y_down] + 
                               coins[x_up][y_up] * (x_down != x_up);
    
    cache[moves][x_down][x_up] = score;
    return score;
  }
}

void solve() {
  int n;
  std::cin >> n;
  
  std::vector<std::vector<int>> coins(n, std::vector<int>(n, 0));
  
  for (int y = 0; y < n; y++) {
    for (int x = 0; x < n; x++) {
      std::cin >> coins[x][y];
    }
  }
  
  const int MAX_MOVES = 2*(n-1);
  
  std::vector<std::vector<std::vector<int>>> cache(MAX_MOVES+1, 
    std::vector<std::vector<int>>(n, std::vector<int>(n, -1)));
  
  std::cout << best_score(0, 0, 0, n, coins, cache) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    solve();
  }
}