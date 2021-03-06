#include <iostream>
#include <vector>
#include <bitset>
#include <limits>

struct Beverage {
  int i;
  int c;
  int v;
};

struct Mix {
  int cost = std::numeric_limits<int>::max();
  std::bitset<100> components;
  
  Mix operator+(const Beverage &b) {
    Mix m = { cost + b.c, components };
    m.components.set(b.i);
    return m;
  }
  
  bool operator>(const Mix &other) const {
    return cost < other.cost || 
      (cost == other.cost && components.count() > other.components.count());  
  }
};

void solve() {
  int n, k;
  std::cin >> n >> k;
  
  std::vector<Beverage> beverages(n);

  for (int i = 0; i < n; i++) {
    Beverage &b = beverages[i];
    std::cin >> b.c >> b.v;
    b.i = i;
  }
  
  std::vector<Mix> best_mix(k+1);
  best_mix[0].cost = 0;
  
  for (int vol = 0; vol < k; vol++) {
    if (best_mix[vol].cost < std::numeric_limits<int>::max()) {
      for (const Beverage &b : beverages) {
        int new_vol = std::min(vol + b.v, k);
        Mix new_mix = best_mix[vol] + b;
        
        if (new_mix > best_mix[new_vol]) {
          best_mix[new_vol] = new_mix;
        }
      }
    }
  }

  std::cout << best_mix[k].cost << " ";
  std::cout << best_mix[k].components.count() << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    solve();
  }
}