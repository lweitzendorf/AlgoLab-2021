#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

void solve(int n) {
  std::vector<K::Point_2> restaurants;

  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;

    restaurants.emplace_back(x, y);
  }

  Triangulation t;
  t.insert(restaurants.begin(), restaurants.end());

  int m;
  std::cin >> m;

  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;

    K::Point_2 p(x, y);
    K::Point_2 r = t.nearest_vertex(p)->point();
    std::cout << long(CGAL::squared_distance(p, r)) << std::endl;
  }
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