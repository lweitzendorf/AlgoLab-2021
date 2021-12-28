#include <iostream>
#include <vector>

using namespace std;

int main() 
{
  int t;
  cin >> t;
  
  for (int tc=0; tc < t; tc++) 
  {
    int n, k;
    cin >> n >> k;
    
    vector<int> values;
    
    for (int card=0; card < n; card++)
    {
      int value;
      cin >> value;
      values.push_back(value);
    }
    
    int i = 0, j = 0;
    int best_i = 0, best_j = 0;
    
    int min_dev = 2147483647;
    int sum = values.at(0);
    
    while (j < n)
    {
      if (sum == k) {
        break;
      } else if (sum > k && i < j) {
        sum -= values.at(i++);
      } else if (j < n-1) {
        sum += values.at(++j);
      } else {
        break;
      }
      
      int dev = abs(k - sum);
      
      if (dev < min_dev) {
        min_dev = dev;
        best_i = i;
        best_j = j;
      }
    }
    cout << best_i << " " << best_j << endl;
  }
  
  return 0;
}
