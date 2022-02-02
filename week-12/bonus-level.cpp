#include <iostream>
#include <vector>
#include <algorithm>

int max_score(int i, int x1, int x2, const std::vector<std::vector<int>>& coins,
              std::vector<std::vector<std::vector<int>>>& cache) {
int n = coins.size();
int y1 = i-x1, y2 = i-x2;

if (x1 >= n || y1 >= n || x2 >= n || y2 >= n) {
return 0;
} else if (cache[i][x1][x2] == -1) {
int new_coins = coins[x1][y1] + (x1 != x2)*coins[x2][y2];
int sub_score = std::max({ max_score(i+1, x1, x2, coins, cache),
                           max_score(i+1, x1+1, x2, coins, cache),
                           max_score(i+1, x1, x2+1, coins, cache),
                           max_score(i+1, x1+1, x2+1, coins, cache) });
cache[i][x1][x2] = new_coins + sub_score;
}

return cache[i][x1][x2];
}

void solve() {
  int n;
  std::cin >> n;

  std::vector<std::vector<int>> coins(n, std::vector<int>(n));

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      std::cin >> coins[i][j];
    }
  }

  std::vector<std::vector<std::vector<int>>> cache(2*n-1,
                                                   std::vector<std::vector<int>>(n, std::vector<int>(n, -1)));

  std::cout << max_score(0, 0, 0, coins, cache) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}