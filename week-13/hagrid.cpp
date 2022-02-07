#include <iostream>
#include <vector>
#include <algorithm>

struct Tunnel {
  int chamber;
  int length;
};

struct Chamber {
  int n = 0;
  int t = 0;
  int galleons = 0;
  std::vector<Tunnel> tunnels;
};

void find_size(std::vector<Chamber>& chambers, int c) {
  for (auto& tunnel : chambers[c].tunnels) {
    find_size(chambers, tunnel.chamber);
    chambers[c].n += chambers[tunnel.chamber].n + 1;
    chambers[c].t += chambers[tunnel.chamber].t + tunnel.length;
  }
  
  std::sort(chambers[c].tunnels.begin(), chambers[c].tunnels.end(),
    [&chambers](auto& t1, auto& t2) -> bool {
      const long c1 = chambers[t1.chamber].t + t1.length;
      const long n1 = chambers[t1.chamber].n + 1;
      const long c2 = chambers[t2.chamber].t + t2.length;
      const long n2 = chambers[t2.chamber].n + 1; 
      return c1*n2 < c2*n1; 
    });
}

long get_galleons(const std::vector<Chamber>& chambers, int c, long t) {
  long galleons = chambers[c].galleons - t;
  for (auto& tunnel : chambers[c].tunnels) {
    galleons += get_galleons(chambers, tunnel.chamber, t + tunnel.length);
    t += 2 * (chambers[tunnel.chamber].t + tunnel.length);
  }
  return galleons;
}

void solve() {
  int n;
  std::cin >> n;
  
  std::vector<Chamber> chambers(n+1);

  for (int i = 1; i <= n; i++) {
    std::cin >> chambers[i].galleons;
  }
  
  for (int i = 0; i < n; i++) {
    int u, v, l;
    std::cin >> u >> v >> l;
    chambers[u].tunnels.push_back(Tunnel { .chamber = v, .length = l });
  }
  
  find_size(chambers, 0);
  std::cout << get_galleons(chambers, 0, 0) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    solve();
  }
} 