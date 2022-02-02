#include <iostream>
#include <vector>

void solve() {
  int n, m, k;
  long x;
  std::cin >> n >> m >> x >> k;
  
  std::vector<std::vector<std::pair<int, int>>> canals(n);
  
  for (int i = 0; i < m; i++) {
    int u, v, p;
    std::cin >> u >> v >> p;
    
    canals[u].emplace_back(v, p);
  }
  
  std::vector<long> old_pts(n, -1);
  std::vector<long> new_pts(n, -1);
  old_pts[0] = 0;

  for (int moves = 1; moves <= k; moves++) {
    for (int i = 0; i < n; i++) {
      if (old_pts[i] != -1) {
        int canal_index = canals[i].empty() ? 0 : i;

        for (auto &canal: canals[canal_index]) {
          int move_pos = canal.first;
          long move_pts = old_pts[i] + canal.second;

          if (move_pts >= x) {
            std::cout << moves << std::endl;
            return;
          }

          new_pts[move_pos] = std::max(new_pts[move_pos], move_pts);
        }
      }
    }
    std::swap(old_pts, new_pts);
  }
  
  std::cout << "Impossible" << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    solve();
  }
}