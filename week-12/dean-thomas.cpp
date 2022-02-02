#include <iostream>
#include <vector>
#include <queue>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

void solve() {
  int n, m;
  long d;
  std::cin >> n >> m >> d;
  
  std::vector<K::Point_2> dementors;
  
  for (int i = 0; i < n; i++) {
    long x, y;
    std::cin >> x >> y;
    
    dementors.emplace_back(x, y);
  }
  
  Triangulation t;
  t.insert(dementors.begin(), dementors.end());
  
  std::priority_queue<std::pair<K::FT, Triangulation::Face_handle>> q;
  
  for (auto f = t.all_faces_begin(); f != t.all_faces_end(); f++) {
    f->info() = 0;
    
    if (t.is_infinite(f)) {
      q.emplace(std::numeric_limits<K::FT>::max(), f);
    } else {
      K::FT r = CGAL::squared_radius(f->vertex(0)->point(), f->vertex(1)->point(), f->vertex(2)->point());
      q.emplace(r, f);
    }
  }
  
  while (!q.empty()) {
    K::FT d = q.top().first;
    Triangulation::Face_handle f = q.top().second;
    q.pop();
    
    if (d > f->info()) {
      f->info() = d;
      
      for (int i = 0; i < 3; i++) {
        K::FT new_d = std::min(d, t.segment(f, i).squared_length());
        Triangulation::Face_handle new_f = f->neighbor(i);
        
        if (new_d > new_f->info()) {
          q.emplace(new_d, new_f);
        }
      }
    }
  }
  
  for (int i = 0; i < m; i++) {
    long x, y, s;
    std::cin >> x >> y >> s;
    
    bool safe_spot = false;
    
    K::Point_2 pos = K::Point_2(x, y);
    K::Point_2 nearest_dementor = t.nearest_vertex(pos)->point();
    K::FT min_dist = CGAL::squared_distance(nearest_dementor, pos);
    
    K::FT clearance = s + d;
    
    if (min_dist >= CGAL::square(clearance)) {
      if (t.locate(pos)->info() >= CGAL::square(2*clearance)) {
        safe_spot = true;
      }
    }
    
    std::cout << (safe_spot ? 'y' : 'n');
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