#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/connected_components.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
typedef std::size_t                                           Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K>                    Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb>          Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                Triangulation;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;

bool in_range(const K::Point_2& a, const K::Point_2& b, long r) {
  return (CGAL::squared_distance(a, b) <= CGAL::square(r));
}

void solve() {
  int n, m, r;
  std::cin >> n >> m >> r;
  
  std::vector<std::pair<K::Point_2, Index>> stations;
  std::vector<std::pair<K::Point_2, K::Point_2>> clues;
  
  std::vector<bool> feasible(m, false);

  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;
    stations.emplace_back(K::Point_2(x, y), i);
  }
  
  for (int i = 0; i < m; i++) {
    int ax, ay, bx, by;
    std::cin >> ax >> ay >> bx >> by;
    clues.emplace_back(K::Point_2(ax, ay), K::Point_2(bx, by));
  }
  
  Triangulation t;
  t.insert(stations.begin(), stations.end());
  
  graph network(n);
  
  for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) {
    auto c1 = t.incident_vertices(v);
    do {
      if (!t.is_infinite(c1) && in_range(v->point(), c1->point(), r)) {
        boost::add_edge(v->info(), c1->info(), network);
        
        auto c2 = t.incident_vertices(c1);
        do {
          if (!t.is_infinite(c2) && v->info() != c2->info()) {
            if (in_range(v->point(), c2->point(), r)) {
              boost::add_edge(v->info(), c2->info(), network);
            }
          }
        } while (++c2 != t.incident_vertices(c1));
      }
    } while (++c1 != t.incident_vertices(v));
  }
  
  if (boost::is_bipartite(network)) {
    std::vector<int> component_map(n);
    boost::connected_components(network, 
      boost::make_iterator_property_map(component_map.begin(), 
        boost::get(boost::vertex_index, network))); 
    
    for (int i = 0; i < m; i++) {
      if (in_range(clues[i].first, clues[i].second, r)) {
        feasible[i] = true;
      } else {
        auto station_a = t.nearest_vertex(clues[i].first);
        auto station_b = t.nearest_vertex(clues[i].second);
        feasible[i] = in_range(clues[i].first, station_a->point(), r) &&
                      in_range(clues[i].second, station_b->point(), r) &&
                      (component_map[station_a->info()] == component_map[station_b->info()]);
      }
    }
  }
  
  for (bool f : feasible) {
    std::cout << (f ? 'y' : 'n');
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