#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

void solve(int n) {
  int left, bottom, right, top;
  std::cin >> left >> bottom >> right >> top;

  std::vector<K::Point_2> bacteria;

  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;

    bacteria.emplace_back(x, y);
  }

  Triangulation t;
  t.insert(bacteria.begin(), bacteria.end());

  std::vector<long> time_to_death;

  for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
    K::Point_2 p = v->point();

    long dist_x = std::min(p.x()-left, right-p.x());
    long dist_y = std::min(p.y()-bottom, top-p.y());
    K::FT r = std::min(dist_x, dist_y);

    if (n > 1) {
      K::Point_2 nb = CGAL::nearest_neighbor(t, v)->point();
      r = std::min(r, std::sqrt(CGAL::squared_distance(p, nb)) / 2);
    }

    long t = std::ceil(CGAL::to_double(std::sqrt(r - 0.5)));
    time_to_death.push_back(t);
  }

  std::sort(time_to_death.begin(), time_to_death.end());

  std::cout << time_to_death[0] << " " << time_to_death[n/2] << " ";
  std::cout << time_to_death[n-1] << " " << std::endl;
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