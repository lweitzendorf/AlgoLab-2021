#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

void solve() {
  int n, m;
  std::cin >> n >> m;

  std::vector<int> strengths(n);

  for (int i = 0; i < n; i++) {
    std::cin >> strengths[i];
  }

  std::sort(strengths.begin(), strengths.end());
  std::vector<int> exclusive_boxes(n, 0);

  bool possible = true;

  for (int i = 0; i < m; i++) {
    int w;
    std::cin >> w;

    if (possible &= (w <= strengths.back())) {
      auto upper = std::upper_bound(strengths.begin(), strengths.end(), w-1);
      exclusive_boxes[upper - strengths.begin()]++;
    }
  }

  if (!possible) {
    std::cout << "impossible" << std::endl;
    return;
  }

  int max_boxes = 0;
  int num_boxes = 0;

  for (int i = n-1; i >= 0; i--) {
    num_boxes += exclusive_boxes[i];
    int local_max = std::ceil(double(num_boxes) / double(n-i));
    max_boxes = std::max(max_boxes, local_max);
  }

  std::cout << (3 * max_boxes - 1) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}