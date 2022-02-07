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
typedef std::size_t Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;
typedef std::tuple<Index, Index, K::FT> Edge;

void solve() {
  int n, k, f0;
  long s0;
  std::cin >> n >> k >> f0 >> s0;
  
  std::vector<std::pair<K::Point_2, Index>> tents;
  
  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;
    
    tents.emplace_back(K::Point_2(x, y), i);
  }
  
  Delaunay t;
  t.insert(tents.begin(), tents.end());

  std::vector<Edge> edges;
  edges.reserve(3*n); // there can be no more in a planar graph
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
            });
            
  boost::disjoint_sets_with_storage<> uf(n);
  Index n_components = n;
  std::vector<int> num_tents(n, 1);
  std::vector<int> num_component_size(k+1, 0);
  num_component_size[1] = n;
  
  K::FT max_s = 0;
  int max_f = 1;
  int f = num_component_size[k];
  
  for (auto e = edges.begin(); e != edges.end(); ++e) {
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    
    if (c1 != c2) {
      uf.link(c1, c2);

      Index c_new = uf.find_set(std::get<0>(*e));
      Index c_old = c_new == c1 ? c2 : c1;
      
      K::FT s = std::get<2>(*e);
      
      if (f >= f0) {
        max_s = std::max(max_s, s);
      }
      
      if (s >= s0) {
        max_f = std::max(max_f, f);
      }
      
      num_component_size[std::min(k, num_tents[c_old])]--;
      num_component_size[std::min(k, num_tents[c_new])]--;
      num_component_size[std::min(k, num_tents[c_new] + num_tents[c_old])]++;
      
      num_tents[c_new] += num_tents[c_old];
      num_tents[c_old] = 0;
      
      f = num_component_size[k];
      
      switch(k) {
        case 2:
          f += num_component_size[1] / 2;
          break;
        case 3:
          f += std::min(num_component_size[1], num_component_size[2]);
          if (num_component_size[2] > num_component_size[1]) {
            f += (num_component_size[2] - num_component_size[1]) / 2;
          } else {
            f += (num_component_size[1] - num_component_size[2]) / 3;
          }
          break;
        case 4:
          f += num_component_size[2] / 2;
          f += std::min(num_component_size[1], num_component_size[3]);
          int rem = std::abs(num_component_size[3] - num_component_size[1]);
          
          if (num_component_size[3] > num_component_size[1]) {
            f += rem / 2; 
            f += ((rem % 2) && (num_component_size[2] % 2));
          } else {
            f += rem / 4;
            f += ((rem % 4) >= 2 && (num_component_size[2] % 2));
          }
          break;
      }
    
      if (--n_components == 1)
        break;
    }
  }
  
  std::cout << long(max_s) << " " << max_f << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    solve();
  }
}