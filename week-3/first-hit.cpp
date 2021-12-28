#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Ray_2 Ray;
typedef K::Segment_2 Segment;

using namespace std;

double floor_to_double(const K::FT& x)
{
  double a = floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int main() {
  ios_base::sync_with_stdio(false);

  int n;
  cin >> n;

  while (n > 0) {
    long x, y, a, b;
    cin >> x >> y >> a >> b;

    Point origin(x, y);
    Ray photon(origin, Point(a, b));
    Segment photon_limit(origin, Point(a, b));

    bool intersection = false;
    vector<Segment> obstacles;

    for (int i=0; i<n; i++) {
      long r, s, t, u;
      cin >> r >> s >> t >> u;
      obstacles.push_back(Segment(Point(r, s), Point(t, u)));
    }

    random_shuffle(obstacles.begin(), obstacles.end());

    for (const Segment obstacle : obstacles) {
      if (!intersection) {
        if (CGAL::do_intersect(photon, obstacle)) {
          intersection = true;
          auto intersection = CGAL::intersection(photon, obstacle);

          if (const Point* p = boost::get<Point>(&*intersection)) {
            photon_limit = Segment(origin, *p);
          } else if (const Segment* s = boost::get<Segment>(&*intersection)) {
            K::FT d1 = CGAL::squared_distance(origin, s->source());
            K::FT d2 = CGAL::squared_distance(origin, s->target());

            if (d1 <= d2) {
              photon_limit = Segment(origin, s->source());
            } else {
              photon_limit = Segment(origin, s->target());
            }
          }
        }
      } else if (CGAL::do_intersect(photon_limit, obstacle)) {
        auto intersection = CGAL::intersection(photon_limit, obstacle);

        if (const Point* p = boost::get<Point>(&*intersection)) {
          photon_limit = Segment(photon.source(), *p);
        } else if (const Segment* s = boost::get<Segment>(&*intersection)) {
          K::FT d1 = CGAL::squared_distance(origin, s->source());
          K::FT d2 = CGAL::squared_distance(origin, s->target());

          if (d1 <= d2) {
            photon_limit = Segment(origin, s->source());
          } else {
            photon_limit = Segment(origin, s->target());
          }
        }
      }
    }

    if (intersection) {
      Point closest_point = photon_limit.target();
      cout << setprecision(0) << fixed;
      cout << floor_to_double(closest_point.x()) << " ";
      cout << floor_to_double(closest_point.y()) << endl;
    } else {
      cout << "no" << endl;
    }

    cin >> n;
  }
}