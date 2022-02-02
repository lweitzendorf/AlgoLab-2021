#include <iostream>
#include <vector>
#include <stack>
#include <CGAL/Gmpq.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

struct Biker {
  int i;
  long y0;
  K::Ray_2 route;
  CGAL::Gmpq abs_slope;
};

void solve() {
  int n;
  std::cin >> n;

  std::vector<Biker> bikers(n);

  for (int i = 0; i < n; i++) {
    long y0, x1, y1;
    std::cin >> y0 >> x1 >> y1;

    bikers[i].i = i;
    bikers[i].y0 = y0;
    bikers[i].route = K::Ray_2(K::Point_2(0, y0), K::Point_2(x1, y1));
    bikers[i].abs_slope = CGAL::Gmpq(std::abs(y1 - y0)) / x1;
  }

  std::sort(bikers.begin(), bikers.end(), [](auto& b1, auto &b2) { return b1.y0 < b2.y0; });

  std::vector<bool> survives(n, true);
  std::stack<int> still_riding;

  for (int i = 0; i < n; i++) {
    while (!still_riding.empty()) {
      int j = still_riding.top();

      if (CGAL::do_intersect(bikers[i].route, bikers[j].route)) {
        if (bikers[i].abs_slope < bikers[j].abs_slope) {
          survives[bikers[j].i] = false;
          still_riding.pop();
        } else {
          survives[bikers[i].i] = false;
          break;
        }
      } else {
        break;
      }
    }

    if (survives[bikers[i].i]) {
      still_riding.push(i);
    }
  }

  for (int i = 0; i < n; i++) {
    if (survives[i]) {
      std::cout << i << " ";
    }
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