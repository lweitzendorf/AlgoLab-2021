#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Line_2 Line;

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);

  int c;
  cin >> c;

  while (c--) {
    int m, n;
    cin >> m >> n;

    vector<Point> route;

    for (int i = 0; i < m; i++) {
      int x, y;
      cin >> x >> y;
      route.push_back(Point(x, y));
    }

    vector<array<Line, 3>> map_parts;

    for (int i = 0; i < n; i++) {
      int x1, y1, x2, y2;

      cin >> x1 >> y1 >> x2 >> y2;
      Point p0(x1, y1), p1(x2, y2);

      cin >> x1 >> y1 >> x2 >> y2;
      Point p2(x1, y1), p3(x2, y2);

      cin >> x1 >> y1 >> x2 >> y2;
      Point p4(x1, y1), p5(x2, y2);

      if (CGAL::right_turn(p0, p1, p2))
        swap(p0, p1);

      if (CGAL::right_turn(p2, p3, p4))
        swap(p2, p3);

      if (CGAL::right_turn(p4, p5, p0))
        swap(p4, p5);

      Line e0 = Line(p0, p1), e1 = Line(p2, p3), e2 = Line(p4, p5);
      map_parts.push_back({e0, e1, e2});
    }

    vector<vector<bool>> legs_covered(n, vector<bool>(m-1, false));

    for (int j = 0; j < n; j++) {
      for (int k = 0; k < m-1; k++) {
        legs_covered[j][k] =
          (!map_parts[j][0].has_on_negative_side(route[k]) &&
           !map_parts[j][1].has_on_negative_side(route[k]) &&
           !map_parts[j][2].has_on_negative_side(route[k]) &&
           !map_parts[j][0].has_on_negative_side(route[k+1]) &&
           !map_parts[j][1].has_on_negative_side(route[k+1]) &&
           !map_parts[j][2].has_on_negative_side(route[k+1]));
      }
    }

    vector<int> coverage_count(m-1, 0);
    int missing_coverage = m-1;

    int min_cost = n;
    int b = 0, e = 0;

    for (int k = 0; k < m-1; k++) {
      if (legs_covered[e][k]) {
        coverage_count[k]++;
        missing_coverage--;
      }
    }

    while (e < n) {
      if (!missing_coverage) {
        min_cost = min(min_cost, e-b+1);
        if (e == b) break;

        for (int k = 0; k < m-1; k++) {
          if (legs_covered[b][k]) {
            if (!--coverage_count[k]) {
              missing_coverage++;
            }
          }
        }
        b++;
      } else if (++e < n) {
        for (int k = 0; k < m-1; k++) {
          if (legs_covered[e][k]) {
            if (!coverage_count[k]++) {
              missing_coverage--;
            }
          }
        }
      }
    }
    
    cout << min_cost << endl;
  }
}