#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct Movement {
  long d, t;
};

long max_coverable_distance(const std::vector<Movement>& moves, const long t) {
  auto upper = std::upper_bound(moves.begin(), moves.end(), t,
                                [](long t, auto& m) { return m.t > t; });
  return (upper == moves.begin()) ? -1 : (upper-1)->d;
}

void max_distance_by_time(std::vector<std::vector<Movement>>& in,
                          std::vector<std::vector<Movement>>& out) {
  out[0].push_back({ 0, 0 });

  for (int k = 1; k < int(in.size()); k++) {
    if (in[k].empty())
      continue;

    std::sort(in[k].begin(), in[k].end(),
              [](auto& m1, auto& m2) -> bool { return m1.t < m2.t; });
    Movement cur_move = in[k].front();

    for (auto& m : in[k]) {
      if (m.d > cur_move.d) {
        if (m.t > cur_move.t) {
          out[k].push_back(cur_move);
          cur_move.t = m.t;
        }
        cur_move.d = m.d;
      }
    }

    out[k].push_back(cur_move);
  }
}

void find_move_combos(int i, int n, Movement m, const long T,
                      const std::vector<Movement>& in,
                      std::vector<std::vector<Movement>>& out) {
  if (m.t >= T) {
    return;
  } else if (n > 0) {
    out[n].push_back(m);
  }

  if (i < int(in.size())) {
    find_move_combos(i+1, n, m, T, in, out);
    find_move_combos(i+1, n+1, { m.d + in[i].d, m.t + in[i].t }, T, in, out);
  }
}

void solve() {
  int n, m;
  long D, T;
  std::cin >> n >> m >> D >> T;

  int n1 = n - (n/2), n2 = (n/2);
  std::vector<Movement> movements_1(n1), movements_2(n2);
  std::vector<long> potion_boost(m);

  for (int i = 0; i < n1; i++) {
    std::cin >> movements_1[i].d >> movements_1[i].t;
  }

  for (int i = 0; i < n2; i++) {
    std::cin >> movements_2[i].d >> movements_2[i].t;
  }

  for (int i = 0; i < m; i++) {
    std::cin >> potion_boost[i];
  }

  std::vector<std::vector<Movement>> combos_1(n1+1), combos_2(n2+1);
  find_move_combos(0, 0, { 0, 0 }, T, movements_1, combos_1);
  find_move_combos(0, 0, { 0, 0 }, T, movements_2, combos_2);

  std::vector<std::vector<Movement>> best_moves_1(n1+1), best_moves_2(n2+1);
  max_distance_by_time(combos_1, best_moves_1);
  max_distance_by_time(combos_2, best_moves_2);

  std::vector<long> best_distance(n+1, 0);

  for (int k1 = 0; k1 <= n1; k1++) {
    for (int k2 = 0; k2 <= n2; k2++) {
      for (auto& m1 : best_moves_1[k1]) {
        long d2 = max_coverable_distance(best_moves_2[k2], T-m1.t-1);
        if (d2 != -1) {
          int k = k1 + k2;
          best_distance[k] = std::max(best_distance[k], m1.d + d2);
        }
      }
    }
  }

  int min_gulps = std::numeric_limits<int>::max();

  for (int k = 1; k <= n; k++) {
    if (best_distance[k] >= D) {
      min_gulps = 0;
      break;
    } else if (best_distance[k] > 0) {
      long needed_boost = std::ceil(double(D - best_distance[k]) / double(k));
      auto lower = std::lower_bound(potion_boost.begin(), potion_boost.end(), needed_boost);

      if (lower != potion_boost.end()) {
        min_gulps = std::min(min_gulps, int(lower - potion_boost.begin() + 1));
      }
    }
  }

  if (min_gulps <= m) {
    std::cout << min_gulps << std::endl;
  } else {
    std::cout << "Panoramix captured" << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}