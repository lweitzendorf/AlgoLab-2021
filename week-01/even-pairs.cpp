#include <iostream>

void solve() {
  int n;
  std::cin >> n;

  int num_even = 0, num_odd = 0;
  int num_even_pairs = 0;

  for (int i = 0; i < n; i++) {
    int x;
    std::cin >> x;

    num_even++;

    if (x % 2) {
      std::swap(num_even, num_odd);
    }

    num_even_pairs += num_even;
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