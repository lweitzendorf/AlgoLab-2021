#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

long FT_ceil(const K::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a >= x+1) a -= 1;
  return a;
}

void solve(int n) {
  std::vector<K::Point_2> citizens;

  for (int i = 0; i < n; i++) {
    long x, y;
    std::cin >> x >> y;

    citizens.emplace_back(x, y);
  }

  Min_circle mc(citizens.begin(), citizens.end(), true);
  K::FT r = CGAL::sqrt(mc.circle().squared_radius());
  std::cout << FT_ceil(r) << std::endl;
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