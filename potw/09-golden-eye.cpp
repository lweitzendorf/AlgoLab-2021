#include <iostream>
#include <vector>
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
typedef std::tuple<Index,Index,K::FT> Edge;

void solve() {
  int n, m;
  long p;
  std::cin >> n >> m >> p;

  std::vector<std::pair<K::Point_2, Index>> jammers;

  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;

    jammers.emplace_back(K::Point_2(x, y), i);
  }

  Delaunay t;
  t.insert(jammers.begin(), jammers.end());

  std::vector<Edge> edges;

  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }

  Index n_components = n + 2*m;
  boost::disjoint_sets_with_storage<> uf(n_components);
  std::vector<std::unordered_set<int>> mission_idx(n_components);

  for (int i = 0; i < m; i++) {
    int x0, y0, x1, y1;
    std::cin >> x0 >> y0 >> x1 >> y1;

    K::Point_2 src(x0, y0), trg(x1, y1);

    K::Point_2 source(x0, y0);
    auto v_s = t.nearest_vertex(source);
    K::FT s_dist = CGAL::squared_distance(v_s->point(), source);
    edges.emplace_back(v_s->info(), n+2*i, 4*s_dist);
    mission_idx[n+2*i].insert(i);

    K::Point_2 target(x1, y1);
    auto v_t = t.nearest_vertex(target);
    K::FT t_dist = CGAL::squared_distance(v_t->point(), target);
    edges.emplace_back(v_t->info(), n+2*i+1, 4*t_dist);
    mission_idx[n+2*i+1].insert(i);
  }

  std::sort(edges.begin(), edges.end(),
            [](const Edge& e1, const Edge& e2) -> bool {
              return std::get<2>(e1) < std::get<2>(e2);
            });

  std::vector<K::FT> mission_power(m);

  for (auto e = edges.begin(); e != edges.end(); ++e) {
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));

    if (c1 != c2) {
      uf.link(c1, c2);
      Index c_new = uf.find_set(std::get<0>(*e));
      Index c_old = (c_new == c1 ? c2 : c1);

      for (int mission : mission_idx[c_old]) {
        if (mission_idx[c_new].find(mission) != mission_idx[c_new].end()) {
          mission_idx[c_new].erase(mission);
          mission_power[mission] = std::get<2>(*e);
        } else {
          mission_idx[c_new].insert(mission);
        }
      }

      if (--n_components == 1)
        break;
    }
  }

  K::FT a = 0, b = 0;

  for (K::FT power : mission_power) {
    a = std::max(a, power);

    if (power <= p) {
      std::cout << 'y';
      b = std::max(b, power);
    } else {
      std::cout << 'n';
    }
  }

  std::cout << std::endl;
  std::cout << long(std::ceil(a)) << std::endl;
  std::cout << long(std::ceil(b)) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}