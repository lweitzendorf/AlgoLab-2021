#include <iostream>
#include <vector>

int m, k;

int max_score(int a, int b, int turn, const std::vector<int>& coins,
              std::vector<std::vector<int>>& cache) {
  if (a > b) {
    return 0;
  } else if (cache[a][b] == -1) {
    if ((turn % m) == k) {
      cache[a][b] = std::max(coins[a] + max_score(a+1, b, turn+1, coins, cache),
                             coins[b] + max_score(a, b-1, turn+1, coins, cache));
    } else {
      cache[a][b] = std::min(max_score(a+1, b, turn+1, coins, cache),
                             max_score(a, b-1, turn+1, coins, cache));
    }
  }

  return cache[a][b];
}

void solve() {
  int n;
  std::cin >> n >> m >> k;

  std::vector<int> coins(n);

  for (int i = 0; i < n; i++) {
    std::cin >> coins[i];
  }

  std::vector<std::vector<int>> cache(n, std::vector<int>(n, -1));

  std::cout << max_score(0, n-1, 0, coins, cache) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}