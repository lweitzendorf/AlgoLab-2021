#include <iostream>
#include <vector>
#include <limits>

void solve() {
  int n, k;
  std::cin >> n >> k;

  std::vector<int> cards_sum(n);

  int sum = 0;

  for (int i = 0; i < n; i++) {
    int v;
    std::cin >> v;

    sum += v;
    cards_sum[i] = sum;
  }

  int i = -1, j = -1;
  int a = 0, b = 0;
  int min_diff = std::numeric_limits<int>::max();

  while (b < n) {
    int value = cards_sum[b] - (a > 0 ? cards_sum[a-1] : 0);
    int diff = std::abs(k - value);

    if (diff < min_diff) {
      min_diff = diff;
      i = a;
      j = b;
    }

    if (value <= k) {
      b++;
    } else {
      a++;
    }
  }

  std::cout << i << " " << j << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}