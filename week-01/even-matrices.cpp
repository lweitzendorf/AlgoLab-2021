#include <iostream>
#include <vector>

void solve() {
  int n;
  std::cin >> n;

  std::vector<std::vector<int>> prefix_sums(n, std::vector<int>(n));

  for (int i = 0; i < n; i++) {
    int row_sum = 0;

    for (int j = 0; j < n; j++) {
      int x;
      std::cin >> x;
      row_sum += x;
      prefix_sums[i][j] = row_sum;
    }
  }

  int num_even_pairs = 0;

  for (int width = 1; width <= n; width++) {
    for (int col = 0; col <= n - width; col++) {
      int num_odd = 0, num_even = 0;

      for (int height = 1; height <= n; height++) {
        num_even++;

        int prev_sum = col ? prefix_sums[height - 1][col - 1] : 0;

        if ((prefix_sums[height - 1][col + width - 1] - prev_sum) % 2) {
          std::swap(num_odd, num_even);
        }

        num_even_pairs += num_even;
      }
    }
  }

  std::cout << num_even_pairs << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}