#include <iostream>
#include <vector>
#include <algorithm>

long max_coverable_distance(std::vector<std::pair<long, long>>& time_grouped_distances, long max_time) {
  auto comp_pair = std::make_pair(max_time-1, std::numeric_limits<long>::max());
  auto upper = std::upper_bound(time_grouped_distances.begin(), 
                                time_grouped_distances.end(), comp_pair);

  if (upper != time_grouped_distances.begin()) {
    return (upper-1)->second;
  }
  return -1;
}

void group_by_time(std::vector<std::vector<std::pair<long, long>>>& in,
                   std::vector<std::vector<std::pair<long, long>>>& out) {
  int n = in.size()-1;
  for (int k = 1; k <= n; k++) {
    auto &combos = in[k];

    if (combos.size() == 0)
      continue;

    std::sort(combos.begin(), combos.end());

    int num_combos = combos.size();
    long cur_time = combos.front().first;
    long cur_dist = combos.front().second;

    for (int i = 1; i < num_combos; i++) {
      long t = combos[i].first;
      long d = combos[i].second;

      if (d > cur_dist) {
        if (t > cur_time) {
          out[k].emplace_back(cur_time, cur_dist);
          cur_time = t;
        }
        cur_dist = d;
      }
    }
    
    out[k].emplace_back(cur_time, cur_dist);
  }
}

void get_move_combos(std::vector<std::pair<long, long>>& movements,
                     std::vector<std::vector<std::pair<long, long>>>& combos,
                     long T, unsigned i, long cur_time, long cur_dist,
                     int num_movements) {
  if (i < movements.size()) {
    get_move_combos(movements, combos, T, i+1, cur_time, cur_dist, num_movements);
    get_move_combos(movements, combos, T, i+1, cur_time + movements[i].first,
                    cur_dist + movements[i].second, num_movements+1);
  } else if (num_movements > 0 && cur_time < T) {
    combos[num_movements].emplace_back(cur_time, cur_dist);
  }
}

int required_gulps(std::vector<long>& gulp_movement, long rem_d, int k) {
  if (rem_d <= 0)
    return 0;

  long bound = (rem_d-1) / k;
  auto upper = std::upper_bound(gulp_movement.begin(), gulp_movement.end(), bound);
  return (upper == gulp_movement.end()) ? -1 : int(upper - gulp_movement.begin() + 1);
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int tc;
  std::cin >> tc;

  while(tc--) {
    int n, m;
    long D, T;
    std::cin >> n >> m >> D >> T;

    std::vector<std::pair<long, long>> movements_1;
    std::vector<std::pair<long, long>> movements_2;

    for (int i = 0; i < n; i++) {
      long d, t;
      std::cin >> d >> t;

      if (i < (n+1)/2) {
        movements_1.emplace_back(t, d);
      } else {
        movements_2.emplace_back(t, d);
      }
    }

    std::vector<long> gulp_movement;

    for (int i = 0; i < m; i++) {
      long s;
      std::cin >> s;
      gulp_movement.push_back(s);
    }
    
    std::vector<long> best_distance_by_move(n+1, -1);

    int n1 = movements_1.size();
    std::vector<std::vector<std::pair<long, long>>> move_combos_1(n1+1);
    get_move_combos(movements_1, move_combos_1, T, 0, 0, 0, 0);

    std::vector<std::vector<std::pair<long, long>>> best_dist_by_time_1(n1+1);
    group_by_time(move_combos_1, best_dist_by_time_1);
    
    for (int k1 = 1; k1 <= n1; k1++) {
      long d = max_coverable_distance(best_dist_by_time_1[k1], T);
      best_distance_by_move[k1] = std::max(best_distance_by_move[k1], d);
    }
    
    int n2 = movements_2.size();
    std::vector<std::vector<std::pair<long, long>>> move_combos_2(n2+1);
    get_move_combos(movements_2, move_combos_2, T, 0, 0, 0, 0);
    
    std::vector<std::vector<std::pair<long, long>>> best_dist_by_time_2(n2+1);
    group_by_time(move_combos_2, best_dist_by_time_2);

    for (int k2 = 1; k2 <= n2; k2++) {
      long d = max_coverable_distance(best_dist_by_time_2[k2], T);
      best_distance_by_move[k2] = std::max(best_distance_by_move[k2], d);
    }

    for (int k1 = 1; k1 <= n1; k1++) {
      for (auto &combo_1 : best_dist_by_time_1[k1]) {
        for (int k2 = 1; k2 <= n2; k2++) {
          long max_dist = max_coverable_distance(best_dist_by_time_2[k2], T-combo_1.first);
          
          if (max_dist != -1) {
            int k = k1 + k2;
            long d = combo_1.second + max_dist;
            best_distance_by_move[k] = std::max(best_distance_by_move[k], d);
          }
        }
      }
    }

    bool found_solution = false;
    int min_gulps = std::numeric_limits<int>::max();

    for(int k = 0; k <= n; k++) {
      if (best_distance_by_move[k] != -1) {
        int gulps = required_gulps(gulp_movement, D-best_distance_by_move[k], k);
        if (gulps != -1) {
          found_solution = true;
          min_gulps = std::min(min_gulps, gulps);
        }
      }
    }

    if (found_solution) {
      std::cout << min_gulps << std::endl;
    } else {
      std::cout << "Panoramix captured" << std::endl;
    }
  }
}
