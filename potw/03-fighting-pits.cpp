#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <list>

int n, k, m;

int max_excitement(const std::vector<int>& fighters, int f, bool north,
                   std::list<int> north_gate, int p,
                   std::list<int> south_gate, int q,
                   std::vector<std::vector<std::vector<std::vector<int>>>>& cache) {
  if (f == n) {
    return 0;
  }
    
  int next_fighter = fighters[f];

  if (north) {
    p++;
    north_gate.push_back(next_fighter);
    if (north_gate.size() > unsigned(m)) 
      north_gate.pop_front();
  } else {
    q++;
    south_gate.push_back(next_fighter);
    if (south_gate.size() > unsigned(m)) 
      south_gate.pop_front();
  }

  int north_unique = 0, south_unique = 0;
  int north_status = 0, south_status = 0;

  int base = 1;
  std::vector<bool> already_seen(k, false);

  for (auto it = south_gate.begin(); it != south_gate.end(); it++) {
    int fighter = *it;
    
    if (!already_seen[fighter]) {
      already_seen[fighter] = true;
      south_unique++;
    }
    
    if (it != south_gate.begin()) {
      south_status += fighter * base;
      base *= k;
    }
  }

  base = 1;
  already_seen = std::vector<bool>(k, false);

  for (auto it = north_gate.begin(); it != north_gate.end(); it++) {
    int fighter = *it;
    
    if (!already_seen[fighter]) {
      already_seen[fighter] = true;
      north_unique++;
    }
    
    if (it != north_gate.begin()) {
      north_status += fighter * base;
      base *= k;
    }
  }

  int imbalance = std::abs(p - q);
  int round_value = 1000 * (north ? north_unique : south_unique) - (1 << imbalance);

  if (round_value < 0) { 
    return std::numeric_limits<int>::min();
  }

  int status1 = p > q ? north_status : south_status;
  int status2 = p > q ? south_status : north_status;

  bool full = (p >= m && q >= m);

  if (full && cache[f][status1][status2][imbalance] != -1) {
    return round_value + cache[f][status1][status2][imbalance];
  }

  int south_excitement = max_excitement(fighters, f+1, false, north_gate, p, south_gate, q, cache);
  int north_excitement = max_excitement(fighters, f+1, true, north_gate, p, south_gate, q, cache);
  int excitement = std::max(south_excitement, north_excitement);

  if (full) {
    cache[f][status1][status2][imbalance] = excitement;
  }
  return round_value + excitement;
}

void solve() {
  std::cin >> n >> k >> m;

  std::vector<int> fighters(n);

  for (int i = 0; i < n; i++) {
    std::cin >> fighters[i];
  }

  int num_states = std::pow(k, m-1);

  std::vector<std::vector<std::vector<std::vector<int>>>> cache(n,
    std::vector<std::vector<std::vector<int>>>(num_states,
      std::vector<std::vector<int>>(num_states,
        std::vector<int>(12, -1))));

  int excitement = max_excitement(fighters, 0, false, {}, 0, {}, 0, cache);
  std::cout << excitement << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}