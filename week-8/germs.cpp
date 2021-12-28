#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_vertices_iterator Vertex_iterator;

long radius_to_time(K::FT r) {
  return std::ceil(std::sqrt(CGAL::to_double(r - 0.5)));
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int n;
  std::cin >> n;

  while(n > 0) {
    int l, b, r, t;
    std::cin >> l >> b >> r >> t;

    std::vector<K::Point_2> bacteria;

    for (int i = 0; i < n; i++) {
      int x, y;
      std::cin >> x >> y;
      bacteria.emplace_back(x, y);
    }

    Triangulation triangulation;
    triangulation.insert(bacteria.begin(), bacteria.end());

    std::vector<K::FT> distances;

    for (Vertex_iterator v = triangulation.finite_vertices_begin(); v != triangulation.finite_vertices_end(); ++v) {
      K::Point_2 p = v->point();
      K::FT max_radius = std::min(std::min(p.x()-l, r-p.x()), std::min(p.y()-b, t-p.y()));

      Triangulation::Vertex_handle nearest_v = CGAL::nearest_neighbor(triangulation, v);
      if (nearest_v != NULL) {
        K::FT neighbor_radius = std::sqrt(CGAL::squared_distance(v->point(), nearest_v->point())) / 2;
        max_radius = std::min(max_radius, neighbor_radius);
      }

      distances.push_back(max_radius);
    }

    std::sort(distances.begin(), distances.end());

    long first = radius_to_time(distances.front());
    long middle = radius_to_time(distances.at(distances.size() / 2));
    long last = radius_to_time(distances.back());

    std::cout << first << " " << middle << " " << last << std::endl;
    std::cin >> n;
  }
}