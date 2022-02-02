#include <iostream>
#include <vector>
#include <limits>

int best_strategy(int a, int m, const std::vector<std::vector<int>>& segments,
                  std::vector<std::vector<int>>& cache) {
  if (m == 0) {
    return 0;
  } else if (size_t(a) >= segments.size()) {
    return std::numeric_limits<int>::min();
  } else if (cache[a][m] == -1) {
    cache[a][m] = best_strategy(a+1, m, segments, cache);

    for (int b : segments[a]) {
      cache[a][m] = std::max(cache[a][m], (b-a+1) + best_strategy(b+1, m-1, segments, cache));
    }
  }

  return cache[a][m];
}

void solve() {
  int n, m, k;
  std::cin >> n >> m >> k;

  std::vector<int> defenders(n);

  for (int i = 0; i < n; i++) {
    std::cin >> defenders[i];
  }
  defenders.push_back(0);

  std::vector<std::vector<int>> valid_segments(n);

  int a = 0, b = 0;
  int defense_value = defenders[0];

  while (b < n) {
    if (defense_value == k) {
      valid_segments[a].push_back(b);
      defense_value += defenders[++b];
    } else if (defense_value < k) {
      defense_value += defenders[++b];
    } else {
      defense_value -= defenders[a++];
      if (a > b) {
        defense_value += defenders[++b];
      }
    }
  }

  std::vector<std::vector<int>> cache(n, std::vector<int>(m+1, -1));
  int max_strategy_value = best_strategy(0, m, valid_segments, cache);

  if (max_strategy_value < 0) {
    std::cout << "fail" << std::endl;
  } else {
    std::cout << max_strategy_value << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}