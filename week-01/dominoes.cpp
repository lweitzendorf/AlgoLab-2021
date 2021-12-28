#include <iostream>

using namespace std;

int main() {
  int t;
  cin >> t;
  
  while (t--) {
    int n;
    cin >> n;
    
    int last_toppled = 1;
    
    for (int i=1; i <= n; i++) {
      int h;
      cin >> h;
      
      if (i <= last_toppled)
        last_toppled = max(last_toppled, i + h - 1);
    }
    
    last_toppled = min(last_toppled, n);
    cout << last_toppled << endl;
  }
}