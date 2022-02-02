#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

struct RepairCost {
  long repaired, covered, not_covered;
};

RepairCost dfs(int i, const std::vector<std::vector<int>>& stages,
               const std::vector<int>& repair_cost) {
  RepairCost cost = { repair_cost[i], 0, 0 };

  if (stages[i].empty()) {
    cost.covered = cost.repaired;
  } else {
    long min_repair_diff = std::numeric_limits<long>::max();

    for (int j : stages[i]) {
      auto sub_cost = dfs(j, stages, repair_cost);
      cost.not_covered += sub_cost.covered;
      cost.covered += std::min(sub_cost.repaired, sub_cost.covered);
      cost.repaired += std::min({ sub_cost.repaired, sub_cost.covered, sub_cost.not_covered });

      min_repair_diff = std::min(min_repair_diff, sub_cost.repaired - sub_cost.covered);
    }

    cost.covered += std::max(0l, min_repair_diff);
  }

  return cost;
}

void solve() {
  int n;
  std::cin >> n;

  std::vector<std::vector<int>> stages(n);
  std::vector<int> repair_cost(n);

  for (int s = 1; s < n; s++) {
    int i, j;
    std::cin >> i >> j;

    stages[i].push_back(j);
  }

  for (int i = 0; i < n; i++) {
    std::cin >> repair_cost[i];
  }

  auto cost = dfs(0, stages, repair_cost);
  long min_valid_cost = std::min(cost.repaired, cost.covered);

  std::cout << min_valid_cost << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}