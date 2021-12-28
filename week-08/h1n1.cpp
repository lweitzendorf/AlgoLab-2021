#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <queue>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

int main() {
  std::ios_base::sync_with_stdio(false);

  int n;
  std::cin >> n;

  while (n > 0) {
    std::vector<K::Point_2> infected_people;

    for (int i = 0; i < n; i++) {
      int x, y;
      std::cin >> x >> y;
      infected_people.emplace_back(x, y);
    }
    
    int m;
    std::cin >> m;
    
    std::vector<std::pair<K::Point_2, long>> users;
    
    for (int i = 0; i < m; i++) {
      int x, y;
      long d;
      std::cin >> x >> y >> d;
      users.emplace_back(K::Point_2(x, y), d);
    }

    Triangulation t;
    t.insert(infected_people.begin(), infected_people.end());
    
    std::unordered_map<Triangulation::Face_handle, long> max_escape_radius;
    std::priority_queue<std::pair<long, Triangulation::Face_handle>> remaining_faces;

    for (auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
      max_escape_radius[f] = 0;
      
      if (t.is_infinite(f))
        remaining_faces.emplace(std::numeric_limits<long>::max(), f);
    }
    
    while (!remaining_faces.empty()) {
      long incoming_width = remaining_faces.top().first;
      Triangulation::Face_handle f = remaining_faces.top().second;
      remaining_faces.pop();
      
      if (incoming_width <= max_escape_radius[f])
        continue;
        
      max_escape_radius[f] = incoming_width;
      
      for (int i = 0; i < 3; i++) {
        Triangulation::Face_handle nb_face = f->neighbor(i);
        Triangulation::Vertex_handle v1 = f->vertex((i+1) % 3);
        Triangulation::Vertex_handle v2 = f->vertex((i+2) % 3);
        long path_width = CGAL::squared_distance(v1->point(), v2->point()) / 4;
        
        remaining_faces.emplace(std::min(path_width, incoming_width), nb_face);
      }
    }
    
    for (const auto &user: users) {
      K::Point_2 p = user.first;
      long d = user.second;
      
      bool can_escape = false;

      if (CGAL::squared_distance(p, t.nearest_vertex(p)->point()) >= d) {
        Triangulation::Face_handle face = t.locate(p);
        can_escape = (d <= max_escape_radius[face]);
      }

      std::cout << (can_escape ? 'y' : 'n');
    }

    std::cout << std::endl;
    std::cin >> n;
  }
}
