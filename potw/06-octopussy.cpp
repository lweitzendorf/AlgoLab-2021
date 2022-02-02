#include <iostream>
#include <vector>
#include <algorithm>

long defuse(int b, std::vector<bool>& defused, const std::vector<std::pair<long, int>>& bombs) {
  if (unsigned(b) >= defused.size() || defused[b]) {
    return 0;
  } else {
    defused[b] = true;
    return 1 + defuse(2*b+1, defused, bombs) + defuse(2*b+2, defused, bombs);
  }
}

void solve() {
  int n;
  std::cin >> n;

  std::vector<std::pair<long, int>> bombs;

  for (int i = 0; i < n; i++) {
    long t;
    std::cin >> t;

    bombs.emplace_back(t, i);
  }

  std::sort(bombs.begin(), bombs.end());
  std::vector<bool> defused(n, false);

  long total_defuse_time = 0;
  bool success = true;

  for (auto& b : bombs) {
    total_defuse_time += defuse(b.second, defused, bombs);

    if (total_defuse_time > b.first) {
      success = false;
      break;
    }
  }

  std::cout << (success ? "yes" : "no") << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}