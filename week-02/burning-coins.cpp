#include <iostream>
#include <vector>

int min_guaranteed(int a, int b, int turn,
                   const std::vector<int>& coins,
                   std::vector<std::vector<int>>& cache){
  if (a > b) {
    return 0;
  } else if (cache[a][b] == -1) {
    int scenario_1 = min_guaranteed(a+1, b, turn+1, coins, cache);
    int scenario_2 = min_guaranteed(a, b-1, turn+1, coins, cache);

    if (turn % 2) {
      cache[a][b] = std::min(scenario_1, scenario_2);
    } else {
      cache[a][b] = std::max(coins[a] + scenario_1, coins[b] + scenario_2);
    }
  }

  return cache[a][b];
}

void solve() {
  int n;
  std::cin >> n;

  std::vector<int> coins(n);

  for (int i = 0; i < n; i++) {
    std::cin >> coins[i];
  }

  std::vector<std::vector<int>> cache(n, std::vector<int>(n, -1));
  std::cout << min_guaranteed(0, n-1, 0, coins, cache) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}