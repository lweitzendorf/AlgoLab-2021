#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Ray_2 Ray;
typedef K::Segment_2 Segment;

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  
  int n;
  cin >> n;
  
  while (n > 0) {
    long x, y, a, b;
    cin >> x >> y >> a >> b;
    
    Ray photon(Point(x, y), Point(a, b));
    vector<Segment> obstacles;
    
    for (int i = 0; i < n; i++) {
      long r, s, t, u;
      cin >> r >> s >> t >> u;
      obstacles.push_back(Segment(Point(r, s), Point(t, u)));
    }
    
    random_shuffle(obstacles.begin(), obstacles.end());
    bool intersect = false;
    
    for (const Segment obstacle : obstacles) {
      if (CGAL::do_intersect(photon, obstacle)) {
        intersect = true;
        break;
      }
    }
    
    cout << (intersect ? "yes" : "no") << endl;
    cin >> n;
  }
}