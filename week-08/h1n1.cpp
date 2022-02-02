#include <iostream>
#include <vector>
#include <queue>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K>                Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>         Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>               Delaunay;

void solve(int n) {
  std::vector<K::Point_2> infected;

  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;

    infected.emplace_back(x, y);
  }

  Delaunay t;
  t.insert(infected.begin(), infected.end());

  std::priority_queue<std::pair<K::FT, Delaunay::Face_handle>> q;

  for (auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
    f->info() = 0;

    if (t.is_infinite(f)) {
      q.emplace(std::numeric_limits<long>::max(), f);
    }
  }

  while (!q.empty()) {
    K::FT d = q.top().first;
    auto f = q.top().second;
    q.pop();

    if (d > f->info()) {
      f->info() = d;

      for (int i = 0; i < 3; i++) {
        auto next_f = f->neighbor(i);
        K::FT next_d = std::min(d, t.segment(f, i).squared_length());

        if (next_d > next_f->info()) {
          q.emplace(next_d, next_f);
        }
      }
    }
  }

  int m;
  std::cin >> m;

  for (int i = 0; i < m; i++) {
    int x, y;
    long d;
    std::cin >> x >> y >> d;

    bool can_escape = false;

    K::Point_2 p(x, y);
    auto nearest = t.nearest_vertex(p)->point();

    if (CGAL::squared_distance(p, nearest) >= d) {
      auto f = t.locate(p);
      if (f->info() >= 4*d) {
        can_escape = true;
      }
    }

    std::cout << (can_escape ? 'y' : 'n');
  }

  std::cout << std::endl;
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