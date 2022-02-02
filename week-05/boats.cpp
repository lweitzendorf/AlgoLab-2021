#include <iostream>
#include <vector>
#include <algorithm>

void solve() {
  int n;
  std::cin >> n;

  std::vector<std::pair<int, int>> boats;

  for (int i = 0; i < n; i++) {
    int l, p;
    std::cin >> l >> p;

    boats.emplace_back(p, l);
  }

  std::sort(boats.begin(), boats.end());

  int num_boats = 1;
  int last_end = boats[0].first;
  int potential_end = std::numeric_limits<int>::max();

  for (int i = 1; i < n; i++) {
    int p = boats[i].first, l = boats[i].second;

    if (potential_end <= p) {
      num_boats++;
      last_end = potential_end;
      potential_end = std::numeric_limits<int>::max();
    }

    int end = std::max(last_end + l, p);
    potential_end = std::min(potential_end, end);
  }

  if (last_end <= boats.back().first) {
    num_boats++;
  }

  std::cout << num_boats << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}