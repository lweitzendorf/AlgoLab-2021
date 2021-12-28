#include <iostream>

using namespace std;

int main() {
  int t;
  cin >> t;
  
  while (t--) {
    int n;
    cin >> n;
    
    int sum = 0;
    
    while (n--) {
      int x;
      cin >> x;
      sum += x;
    }
    
    cout << sum << endl;
  }
}