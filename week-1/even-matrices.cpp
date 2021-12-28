#include <iostream>
#include <vector>

using namespace std;

int main() {
  int t;
  cin >> t;
  
  while (t--) {
    int n;
    cin >> n;
    
    vector<vector<int>> row_sums;
    
    for (int row=0; row < n; row++) {
      row_sums.push_back(vector<int>());
      
      int sum = 0;
      
      for (int col=0; col < n; col++) {
        int x;
        cin >> x;
        sum += x;
        row_sums.at(row).push_back(sum);
      }
    }
    
    int num_pairs = 0;
    
    for (int width=1; width <= n; width++) {
      for (int col=0; col <= n - width; col++) {
        int even = 0;
        int odd = 0;
        
        for (int row=0; row < n; row++) {
          int prev_sum = col ? row_sums.at(row).at(col - 1) : 0;
          int x = row_sums.at(row).at(col + width - 1) - prev_sum;
          
          even++;
          if (x % 2)
            swap(even, odd);
            
          num_pairs += even;
        }
      }
    }
    
    cout << num_pairs << endl;
  }
}