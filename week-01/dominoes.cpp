#include <iostream>
#include <vector>

void solve() {
  int n;
  std::cin >> n;

  std::vector<int> dominoes(n);

  for (int i = 0; i < n; i++) {
    std::cin >> dominoes[i];
  }

  int num_toppled = 1;

  for (int i = 0; i < std::min(n, num_toppled); i++) {
    num_toppled = std::max(num_toppled, i + dominoes[i]);
  }

  std::cout << std::min(n, num_toppled) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}