#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h> 
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits; 
typedef CGAL::Min_circle_2<Traits> Min_circle;

typedef K::Point_2 Point;

using namespace std;

double ceil_to_double(const K::FT& x)
{
  double a = ceil(CGAL::to_double(x));
  while (a >= x+1) a -= 1;
  while (a < x) a += 1;
  return a;
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int n;
  cin >> n;
  
  while (n > 0) {
    vector<Point> homes;
    
    for (int i = 0; i < n; i++) {
      long x, y;
      cin >> x >> y;
      homes.push_back(Point(x, y));
    }
    
    Min_circle mc(homes.begin(), homes.end(), true);
    Traits::Circle c = mc.circle();
    K::FT radius = CGAL::sqrt(c.squared_radius());
    
    cout << fixed << setprecision(0) << ceil_to_double(radius) << endl;
    
    cin >> n;
  }
}