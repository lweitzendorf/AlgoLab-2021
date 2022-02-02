#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

bool is_in_triangle(std::array<K::Line_2, 3>& t, K::Point_2& p) {
  return !t[0].has_on_positive_side(p) && !t[1].has_on_positive_side(p) &&
         !t[2].has_on_positive_side(p);
}

void solve() {
  int m, n;
  std::cin >> m >> n;

  std::vector<K::Point_2> path;
  std::vector<std::array<K::Line_2, 3>> map_parts;

  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;

    path.emplace_back(x, y);
  }

  for (int i = 0; i < n; i++) {
    int x1, y1, x2, y2;
    std::cin >> x1 >> y1 >> x2 >> y2;
    K::Point_2 p1(x1, y1), p2(x2, y2);

    int x3, y3, x4, y4;
    std::cin >> x3 >> y3 >> x4 >> y4;
    K::Point_2 p3(x3, y3), p4(x4, y4);

    int x5, y5, x6, y6;
    std::cin >> x5 >> y5 >> x6 >> y6;
    K::Point_2 p5(x5, y5), p6(x6, y6);

    if (CGAL::left_turn(p1, p2, p3))
      std::swap(p1, p2);

    if (CGAL::left_turn(p3, p4, p5))
      std::swap(p3, p4);

    if (CGAL::left_turn(p5, p6, p1))
      std::swap(p5, p6);

    map_parts.push_back({ K::Line_2(p1, p2), K::Line_2(p3, p4), K::Line_2(p5, p6) });
  }

  std::vector<std::vector<int>> legs_contained(n);

  for (int map = 0; map < n; map++) {
    for (int leg = 0; leg < m-1; leg++) {
      bool start_contained = is_in_triangle(map_parts[map], path[leg]);
      bool end_contained = is_in_triangle(map_parts[map], path[leg+1]);

      if (start_contained && end_contained) {
        legs_contained[map].push_back(leg);
      }
    }
  }

  int min_cost = n;
  int b = 0, e = 0;

  int missing_coverage = m - 1;
  std::vector<int> leg_count(missing_coverage, 0);

  for (int l : legs_contained[0]) {
    leg_count[l]++;
    missing_coverage--;
  }

  while (e < n) {
    if (missing_coverage == 0) {
      min_cost = std::min(min_cost, e-b+1);

      if (min_cost == 0)
        break;

      for (int l : legs_contained[b]) {
        if (--leg_count[l] == 0) {
          missing_coverage++;
        }
      }
      b++;
    } else if (++e < n) {
      for (int l : legs_contained[e]) {
        if (leg_count[l]++ == 0) {
          missing_coverage--;
        }
      }
    }
  }

  std::cout << min_cost << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int c;
  std::cin >> c;

  while (c--) {
    solve();
  }
}