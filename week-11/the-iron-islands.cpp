#include <iostream>
#include <vector>
#include <unordered_map>

struct Plan {
  int waterway;
  int num_islands;
  int next_best = -1;
};

void solve() {
  int n, k, w;
  std::cin >> n >> k >> w;

  std::vector<int> men_required(n);

  for (int i = 0; i < n; i++) {
    std::cin >> men_required[i];
  }

  std::vector<std::vector<int>> waterway_sums(w);
  std::unordered_map<int, Plan> max_islands;

  for (int i = 0; i < w; i++) {
    int l;
    std::cin >> l;

    int sum = 0;

    for (int j = 0; j < l; j++) {
      int r;
      std::cin >> r;

      sum += men_required[r];
      waterway_sums[i].push_back(sum);

      Plan plan;
      plan.waterway = i;
      plan.num_islands = j;

      int sum_no_pyke = sum - men_required[0];

      if (max_islands.find(sum_no_pyke) == max_islands.end()) {
        max_islands[sum_no_pyke] = plan;
      } else if (plan.num_islands >= max_islands[sum_no_pyke].num_islands) {
        plan.next_best = max_islands[sum_no_pyke].num_islands;
        max_islands[sum_no_pyke] = plan;
      } else if (plan.num_islands > max_islands[sum_no_pyke].next_best) {
        max_islands[sum_no_pyke].next_best = plan.num_islands;
      }
    }
  }

  int num_islands = 0;

  for (int i = 0; i < w; i++) {
    int l = waterway_sums[i].size();
    int a = 0, b = 0;

    while (b < l) {
      int men_used = waterway_sums[i][b] - (a > 0 ? waterway_sums[i][a-1] : 0);

      if (men_used == k) {
        num_islands = std::max(num_islands, b-a+1);
        b++;
      } else if (men_used > k) {
        a++;
        b += (a > b);
      } else {
        if (a == 0 && max_islands.find(k-men_used) != max_islands.end()) {
          Plan other_plan = max_islands[k-men_used];
          int max_other_islands = (other_plan.waterway == i ? other_plan.next_best : other_plan.num_islands);

          if (max_other_islands != -1) {
            num_islands = std::max(num_islands, b-a+1+max_other_islands);
          }
        }
        b++;
      }
    }
  }

  std::cout << num_islands << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}