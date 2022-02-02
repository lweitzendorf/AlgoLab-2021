#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

struct PotionA {
  long p, h;
};

void solve() {
  int n, m;
  std::cin >> n >> m;

  long a, b;
  std::cin >> a >> b;

  long P, H, W;
  std::cin >> P >> H >> W;

  std::vector<PotionA> potions_a(n);
  std::vector<long> potions_b(m);

  for (int i = 0; i < n; i++) {
    std::cin >> potions_a[i].p >> potions_a[i].h;
  }

  for (int i = 0; i < m; i++) {
    std::cin >> potions_b[i];
  }

  std::vector<std::vector<bool>> used_potions_old(H+1, std::vector<bool>(n, false));
  std::vector<std::vector<bool>> used_potions_new(H+1, std::vector<bool>(n, false));
  std::vector<std::vector<long>> max_power(n+1, std::vector<long>(H+1, -1));
  max_power[0][0] = 0;

  for (int i = 1; i <= n; i++) {
    std::vector<long> best_j(H+1, -1), old_h(H+1, -1);

    for (int h = 0; h <= H; h++) {
      if (max_power[i-1][h] != -1) {
        for (int j = 0; j < n; j++) {
          if (!used_potions_old[h][j]) {
            long new_h = std::min(h + potions_a[j].h, H);
            long new_p = max_power[i-1][h] + potions_a[j].p;

            if (new_p > max_power[i][new_h]) {
              best_j[new_h] = j;
              old_h[new_h] = h;
              max_power[i][new_h] = new_p;
            }
          }
        }
      }
    }

    for (int h = 0; h <= H; h++) {
      if (best_j[h] != -1) {
        used_potions_new[h] = used_potions_old[old_h[h]];
        used_potions_new[h][best_j[h]] = true;
      } else {
        used_potions_new[h] = std::vector<bool>(n, false);
      }
    }

    std::swap(used_potions_old, used_potions_new);
  }

  int min_potions = std::numeric_limits<int>::max();
  std::sort(potions_b.begin(), potions_b.end(), std::greater<>());

  for (int num_a = 1; num_a <= n; num_a++) {
    long p = max_power[num_a][H];
    long w = -(a * num_a);

    for (int num_b = 1; num_b <= m; num_b++) {
      w += potions_b[num_b-1];
      p -= b;

      if (p < P) {
        break;
      } else if (w >= W) {
        min_potions = std::min(min_potions, num_a + num_b);
        break;
      }
    }
  }

  bool possible = (min_potions <= n + m);
  std::cout << (possible ? min_potions : -1) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}