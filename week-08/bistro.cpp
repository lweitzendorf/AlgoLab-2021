#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int n;
  std::cin >> n;
  
  while(n > 0) {
    std::vector<K::Point_2> existing_restaurants;
    
    for (int i = 0; i < n; i++) {
      int x, y;
      std::cin >> x >> y;
      existing_restaurants.emplace_back(x, y);
    }
    
    int m;
    std::cin >> m;
    
    std::vector<K::Point_2> possible_locations;
    
    for (int i = 0; i < m; i++) {
      int x, y;
      std::cin >> x >> y;
      possible_locations.emplace_back(x, y);
    }
    
    Triangulation t;
    t.insert(existing_restaurants.begin(), existing_restaurants.end());
    
    for (const K::Point_2 &loc : possible_locations) {
      K::Point_2 nearest_restaurant = t.nearest_vertex(loc)->point();
      K::FT distance = CGAL::squared_distance(loc, nearest_restaurant);
      std::cout << long(CGAL::to_double(distance))  << std::endl;;
    }

    std::cin >> n;
  }
}