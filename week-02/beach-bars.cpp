#include <iostream>
#include <vector>
#include <algorithm>

void solve() {
  int n;
  std::cin >> n;

  std::vector<int> parasols(n);

  for (int i = 0; i < n; i++) {
    std::cin >> parasols[i];
  }

  std::sort(parasols.begin(), parasols.end());

  int a = 0, b = 0;
  int max_parasols = 0, min_distance = 0;

  std::vector<int> locations;

  while (b < n) {
    int span = parasols[b] - parasols[a];

    if (span <= 200) {
      int num_parasols = b - a + 1;
      int location = (parasols[b] + parasols[a]) / 2;

      int max_walking_distance = (span + 1) / 2;
      bool optimal = true;

      if (num_parasols > max_parasols) {
        max_parasols = num_parasols;
        min_distance = max_walking_distance;
        locations.clear();
      } else if (num_parasols == max_parasols && max_walking_distance <= min_distance) {
        if (max_walking_distance < min_distance) {
          min_distance = max_walking_distance;
          locations.clear();
        }
      } else {
        optimal = false;
      }

      if (optimal) {
        if (span % 2) {
          if (location < 0) {
            locations.push_back(location - 1);
            locations.push_back(location);
          } else {
            locations.push_back(location);
            locations.push_back(location + 1);
          }
        } else {
          locations.push_back(location);
        }
      }

      b++;
    } else {
      a++;
    }
  }

  std::cout << max_parasols << " " << min_distance << std::endl;

  for (int l : locations) {
    std::cout << l << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}