#include <iostream>

void solve() {
  int n;
  std::cin >> n;

  int sum = 0;

  for (int i = 0; i < n; i++) {
    int x;
    std::cin >> x;

    sum += x;
  }

  std::cout << sum << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}