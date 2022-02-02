#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

long FT_floor(const K::FT& x)
{
  double a = floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void solve(int n) {
  long x, y, a, b;
  std::cin >> x >> y >> a >> b;

  K::Point_2 start(x, y);
  K::Ray_2 photon_ray(start, K::Point_2(a, b));
  K::Segment_2 photon_seg;

  bool intersect = false;
  std::vector<K::Segment_2> obstacles;

  for (int i=0; i<n; i++) {
    long r, s, t, u;
    std::cin >> r >> s >> t >> u;

    obstacles.emplace_back(K::Point_2(r, s), K::Point_2(t, u));
  }

  std::random_shuffle(obstacles.begin(), obstacles.end());

  for (const K::Segment_2 obstacle : obstacles) {
    if (intersect) {
      if (CGAL::do_intersect(photon_seg, obstacle)) {
        auto o = CGAL::intersection(photon_seg, obstacle);
        if (const K::Point_2* op = boost::get<K::Point_2>(&*o)) {
          photon_seg = K::Segment_2(start, *op);
        } else if (const K::Segment_2* os = boost::get<K::Segment_2>(&*o)) {
          K::FT d1 = CGAL::squared_distance(start, os->source());
          K::FT d2 = CGAL::squared_distance(start, os->target());
          K::Point_2 p = (d1 < d2) ? os->source() : os->target();
          photon_seg = K::Segment_2(start, p);
        }
      }
    } else {
      if (intersect |= CGAL::do_intersect(photon_ray, obstacle)) {
        auto o = CGAL::intersection(photon_ray, obstacle);
        if (const K::Point_2* op = boost::get<K::Point_2>(&*o)) {
          photon_seg = K::Segment_2(start, *op);
        } else if (const K::Segment_2* os = boost::get<K::Segment_2>(&*o)) {
          K::FT d1 = CGAL::squared_distance(start, os->source());
          K::FT d2 = CGAL::squared_distance(start, os->target());
          K::Point_2 p = (d1 < d2) ? os->source() : os->target();
          photon_seg = K::Segment_2(start, p);
        }
      }
    }
  }

  if (intersect) {
    K::FT x = photon_seg.target().x();
    K::FT y = photon_seg.target().y();
    std::cout << FT_floor(x) << " " << FT_floor(y) << std::endl;
  } else {
    std::cout << "no" << std::endl;
  }
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