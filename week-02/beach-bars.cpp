#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int main() {
  int t;
  cin >> t;
  
  while (t--) {
    int n;
    cin >> n;
    
    vector<int> coordinates;
    
    for (int i=0; i < n; i++) {
      int x;
      cin >> x;
      coordinates.push_back(x);
    }
    
    sort(coordinates.begin(), coordinates.end());
    
    int max_parasols = 0;
    int min_max_distance = 101;
    vector<pair<int, int>> solutions;

    int i = 0, j = 0;

    while (j < n) {
      int distance = coordinates.at(j) - coordinates.at(i);
      int num_parasols = j - i + 1;
      
      int half_dist = (distance + 1) / 2;
      
      if (distance <= 200) {
        if (num_parasols >= max_parasols) {
          if (num_parasols > max_parasols || half_dist < min_max_distance) {
            max_parasols = num_parasols;
            min_max_distance = half_dist;
            solutions.clear();
          }
        }
        
        if (num_parasols == max_parasols && half_dist == min_max_distance) {
          int pos_1 = coordinates.at(j) - half_dist;
          int pos_2 = coordinates.at(i) + half_dist;
          
          if (pos_2 > pos_1) {
            solutions.push_back(make_pair(pos_1, half_dist));
          }
          solutions.push_back(make_pair(pos_2, half_dist));
        }
        j++;
      } else {
        i++;
      }
    }
    
    struct less_than_pair {
        inline bool operator()(pair<int, int> a, pair<int, int> b) { 
          return a.second < b.second; 
        }
    };
    
    sort(solutions.begin(), solutions.end(), less_than_pair());
    
    cout << max_parasols << " " << solutions.front().second << endl;
    
    for (auto const& pair : solutions) {
      cout << pair.first << " ";
    }
    cout << endl;
  }
}
