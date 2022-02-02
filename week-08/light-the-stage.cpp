#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

void find_survivors(const std::vector<std::pair<K::Point_2, long>>& participants,
                    const std::vector<K::Point_2>& lamps, int round, long h,
                    std::vector<int>& survivors, bool find_all) {
  Triangulation t;
  t.insert(lamps.begin(), lamps.begin() + round);

  for (int i = 0; i < int(participants.size()); i++) {
    K::Point_2 p = participants[i].first;
    long d = participants[i].second;

    auto nearest = t.nearest_vertex(p)->point();

    if (CGAL::squared_distance(p, nearest) >= CGAL::square(h + d)) {
      survivors.push_back(i);

      if (!find_all)
        break;
    }
  }
}

void solve() {
  int m, n;
  std::cin >> m >> n;

  std::vector<std::pair<K::Point_2, long>> participants;
  std::vector<K::Point_2> lamps;

  for (int i = 0; i < m; i++) {
    int x, y, r;
    std::cin >> x >> y >> r;

    participants.emplace_back(K::Point_2(x, y), r);
  }

  long h;
  std::cin >> h;

  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;

    lamps.emplace_back(x, y);
  }

  int min = 1, max = n;
  std::vector<int> survivors;

  while (min < max) {
    int round = (min + max + 1) / 2;
    find_survivors(participants, lamps, round, h, survivors, false);

    if (survivors.empty()) {
      max = round - 1;
    } else {
      min = round;
      survivors.clear();
    }
  }

  find_survivors(participants, lamps, max, h, survivors, true);

  for (int winner : survivors) {
    std::cout << winner << " ";
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