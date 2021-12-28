#include <iostream>
#include <vector>

using namespace std;

int main() {
  int t;
  cin >> t;
  
  while (t--) {
    int n;
    cin >> n;
    
    int num_pairs = 0;
    
    int odd_pairs = 0;
    int even_pairs = 0;
    
    for (int i=0; i < n; i++) {
      int x;
      cin >> x;
      
      even_pairs++;
      
      if (x % 2)
        swap(odd_pairs, even_pairs);
      
      num_pairs += even_pairs;
    }

    cout << num_pairs << endl;
  }
} 