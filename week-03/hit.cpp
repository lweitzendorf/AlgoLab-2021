#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

void solve(int n) {
  long x, y, a, b;
  std::cin >> x >> y >> a >> b;

  K::Ray_2 photon(K::Point_2(x, y), K::Point_2(a, b));
  bool intersect = false;

  for (int i = 0; i < n; i++) {
    long r, s, t, u;
    std::cin >> r >> s >> t >> u;

    if (!intersect) {
      K::Segment_2 obstacle(K::Point_2(r, s), K::Point_2(t, u));
      intersect = CGAL::do_intersect(photon, obstacle);
    }
  }

  std::cout << (intersect ? "yes" : "no") << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int n;
  std::cin >> n;

  while (n > 0) {
    solve(n);
    std::cin >> n;
  }
}