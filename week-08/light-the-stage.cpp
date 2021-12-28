#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    int m, n;
    std::cin >> m >> n;

    std::vector<std::pair<K::Point_2, long>> participants;
    
    for (int i = 0; i < m; i++) {
      int x, y, r;
      std::cin >> x >> y >> r;
      participants.emplace_back(K::Point_2(x, y), r);
    }
    
    int h;
    std::cin >> h;
    
    std::vector<Triangulation::Vertex_handle> lamps;

    Triangulation t;

    for (int i = 0; i < n; i++) {
      int x, y;
      std::cin >> x >> y;
      
      auto vertex = t.insert(K::Point_2(x, y));
      lamps.push_back(vertex);
    }
    
    std::unordered_map<Triangulation::Vertex_handle, std::set<int>> nearest_lamp_map(n);
    int num_left = 0;

    for (int i = 0; i < m; i++) {
      K::Point_2 loc = participants[i].first;
      K::FT r = participants[i].second + h;
      r *= r;

      auto nearest_lamp = t.nearest_vertex(loc);
      if (CGAL::squared_distance(loc, nearest_lamp->point()) >= r) {
        std::cout << i << " ";
        num_left++;
      } else {
        nearest_lamp_map[nearest_lamp].insert(i);
      }
    }
      
    for (int i = n-1; (i >= 0) && !num_left; i--) {
      t.remove(lamps[i]);
      
      for (const auto &p : nearest_lamp_map[lamps[i]]) {
        K::Point_2 loc = participants[p].first;
        K::FT r = participants[p].second + h;
        r *= r;
        
        auto nearest_lamp = t.nearest_vertex(loc);
        if (CGAL::squared_distance(loc, nearest_lamp->point()) >= r) {
          std::cout << p << " ";
          num_left++;
        } else {
          nearest_lamp_map[nearest_lamp].insert(p);
        }
      }
    }
    
    std::cout << std::endl;
  }
}