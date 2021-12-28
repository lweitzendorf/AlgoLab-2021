#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<Index, Index, K::FT> Edge;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    int n, m, k;
    long s;
    std::cin >> n >> m >> s >> k;
    
    std::vector<std::pair<K::Point_2, Index>> trees;
    
    for (int i = 0; i < n; i++) {
      int x, y;
      std::cin >> x >> y;
      trees.emplace_back(K::Point_2(x, y), i);
    }
    
    std::vector<K::Point_2> bones;
    
    for (int i = 0; i < m; i++) {
      int x, y;
      std::cin >> x >> y;
      bones.emplace_back(x, y);
    }
    
    Delaunay t;
    t.insert(trees.begin(), trees.end());

    std::vector<Edge> edges;
    
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
      Index i1 = e->first->vertex((e->second+1)%3)->info();
      Index i2 = e->first->vertex((e->second+2)%3)->info();
      // ensure smaller index comes first
      if (i1 > i2) std::swap(i1, i2);
      edges.emplace_back(i1, i2, t.segment(e).squared_length());
    }
    
    for (int i = 0; i < m; i++) {
      auto v = t.nearest_vertex(bones[i]);
      K::FT dist = CGAL::squared_distance(bones[i], v->point());
      edges.emplace_back(v->info(), n+i, 4*dist);
    }
    
    std::sort(edges.begin(), edges.end(),
        [](const Edge& e1, const Edge& e2) -> bool {
          return std::get<2>(e1) < std::get<2>(e2);
              });
              
    int a = 0;
    double q = std::numeric_limits<double>::max();
    
    boost::disjoint_sets_with_storage<> uf(n + m);
    Index n_components = n + m;
    
    std::vector<int> num_bones(n_components, 0);
    for (int i = 0; i < m; i++) {
      num_bones[n+i] = 1;
    }
    
    for (auto e = edges.begin(); e != edges.end(); ++e) {
      Index c1 = uf.find_set(std::get<0>(*e));
      Index c2 = uf.find_set(std::get<1>(*e));
      
      if (c1 != c2) {
        uf.link(c1, c2);
        
        Index c_new = uf.find_set(std::get<0>(*e));
        num_bones[c_new] = num_bones[c1] + num_bones[c2];

        if (std::get<2>(*e) <= s) {
          a = std::max(a, num_bones[c_new]);
        }
        
        if (num_bones[c_new] >= k) {
          q = std::min(q, std::get<2>(*e));
        }

        if (--n_components == 1) break;
      }
    }
    
    std::cout << a << " " << long(std::ceil(q)) << std::endl;
  }
}