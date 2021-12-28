#include <stack>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Ray_2 Ray;

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    int n;
    std::cin >> n;

    std::vector<bool> intersects(n, false);
    std::vector<std::pair<long, int>> starting_points;
    std::vector<Ray> routes;
    std::vector<K::FT> abs_slopes;

    for (int i = 0; i < n; i++) {
      long y0, x1, y1;
      std::cin >> y0 >> x1 >> y1;

      starting_points.push_back(std::make_pair(y0, i));
      routes.push_back(Ray(Point(0, y0), Point(x1, y1)));

      K::FT delta_x = x1;
      K::FT delta_y = std::abs(y1 - y0);
      abs_slopes.push_back(delta_y / delta_x);
    }

    std::sort(starting_points.begin(), starting_points.end());
    std::stack<int> still_riding;

    for (int i = 0; i < n; i++) {
      int biker = starting_points[i].second;
      Ray route = routes[biker];

      while (!still_riding.empty() && CGAL::do_intersect(route, routes[still_riding.top()])) {
        if (abs_slopes[biker] < abs_slopes[still_riding.top()]) {
          intersects[still_riding.top()] = true;
          still_riding.pop();
        } else {
          intersects[biker] = true;
          break;
        }
      }

      if (!intersects[biker])
        still_riding.push(biker);
    }

    for (int i = 0; i < n; i++)
      if (!intersects.at(i))
        std::cout << i << " ";

    std::cout << "\n";
  }
}