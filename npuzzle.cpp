#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <set>
#include <sys/types.h>
#include <vector>

using namespace std;
typedef vector<vector<int>> Grid;

// Checks for equalling solution
bool vec_equals(const Grid &a, const Grid &b) {
  for (unsigned int i = 0; i < a.size(); i++) {
    for (unsigned int j = 0; j < a[i].size(); j++) {
      if (a[i][j] != b[i][j]) {
        return false;
      }
    }
  }
  return true;
}

void print_grid(const Grid &a) {
  for (unsigned int i = 0; i < a.size(); i++) {
    for (unsigned int j = 0; j < a[i].size(); j++) {
      cout << a[i][j] << "\t";
    }
    cout << endl;
  }
}

string stringify(const Grid &a) {
    string ret = "";
    for(unsigned int i = 0; i < a.size(); i++) {
        for(unsigned int j = 0; j < a[i].size(); j++) {
            ret += to_string(a[i][j]) + "|";
        }
        ret += "|";
    }
    return ret;
}

// Generates a random integer orientation (for solution and start)
void generate_random(Grid &a, unsigned int r, unsigned int c) {
  vector<int> nums;
  vector<pair<int, int>> coordinates;
  vector<int> temp;
  for (unsigned int i = 0; i < r; i++) {
    a.push_back(temp);
    for (unsigned int j = 0; j < c; j++) {
      a[i].push_back(0);
      // Generate coordinates
      coordinates.push_back(make_pair(i, j));
    }
  }
  for(unsigned int i = 1; i < r*c; i++) {
    nums.push_back(i);
  }
  // Shuffle both vectors
  random_shuffle(nums.begin(), nums.end());
  random_shuffle(coordinates.begin(), coordinates.end());
  // Assign
  for (unsigned int i = 0; i < nums.size(); i++) {
    a[coordinates[i].first][coordinates[i].second] = nums[i];
  }
}

bool DFS(Grid &n, const Grid &sol, set<string> &visited, int &count) {
  // Base Case & Counters
  if (vec_equals(n, sol)) {
    return true;
  } else if (!visited.insert(stringify(n)).second) {
    return false;
  }
  // Find Hole
  unsigned int r_e;
  unsigned int c_e;
  bool b = false;
  for (unsigned int i = 0; i < n.size(); i++) {
    for (unsigned int j = 0; j < n[i].size(); j++) {
      if (n[i][j] == 0) {
        r_e = i;
        c_e = j;
        b = true;
        break;
      }
    }
    if (b) {
      break;
    }
  }
  // DFS Part
  bool ret;
  // Check dimensions and make recursive call
  if (r_e > 0) {
    n[r_e][c_e] = n[r_e - 1][c_e];
    n[r_e - 1][c_e] = 0;

    ret = DFS(n, sol, visited, count);

    n[r_e - 1][c_e] = n[r_e][c_e];
    n[r_e][c_e] = 0;
    if (ret) {
      return true;
    }
  }
  if (r_e < n.size() - 1) {
    n[r_e][c_e] = n[r_e + 1][c_e];
    n[r_e + 1][c_e] = 0;

    ret = DFS(n, sol, visited, count);

    n[r_e + 1][c_e] = n[r_e][c_e];
    n[r_e][c_e] = 0;
    if (ret) {
      return true;
    }
  }
  if (c_e > 0) {
    n[r_e][c_e] = n[r_e][c_e - 1];
    n[r_e][c_e - 1] = 0;

    ret = DFS(n, sol, visited, count);

    n[r_e][c_e - 1] = n[r_e][c_e];
    n[r_e][c_e] = 0;
    if (ret) {
      return true;
    }
  }
  if (c_e < n[0].size() - 1) {
    n[r_e][c_e] = n[r_e][c_e + 1];
    n[r_e][c_e + 1] = 0;

    ret = DFS(n, sol, visited, count);

    n[r_e][c_e + 1] = n[r_e][c_e];
    n[r_e][c_e] = 0;
    if (ret) {
      return true;
    }
  }
  return false;
}

bool BFS(const Grid &n, const Grid &sol, set<string> &visited, int &count) {
  // Check if at soln first:
  count++;
  visited.insert(stringify(n));
  if (vec_equals(n, sol)) {
    return true;
  }

  vector<Grid> current;
  vector<Grid> previous;
  previous.push_back(n);
  for (; previous.size() > 0;) {
    // loop over previous and add each
    for (unsigned int i = 0; i < previous.size(); i++) {
        Grid temp = previous[i];
      // Get Zero Location
      unsigned int r_e;
      unsigned int c_e;
      bool b = false;
      for (unsigned int j = 0; j < temp.size(); j++) {
        for (unsigned int k = 0; k < temp[j].size(); k++) {
          if (temp[j][k] == 0) {
            r_e = j;
            c_e = k;
            b = true;
            break;
          }
        }
        if (b) {
          break;
        }
      }
      // BFS Part: Store each new board in current row
      if (r_e > 0) {
        temp[r_e][c_e] = temp[r_e - 1][c_e];
        temp[r_e - 1][c_e] = 0;
        count++;
        if (vec_equals(temp, sol)) {
          return true;
        }
        // Push into next row
        if(visited.find(stringify(temp)) == visited.end()) {
            current.push_back(temp);
            visited.insert(stringify(temp));
        }
        temp[r_e - 1][c_e] = temp[r_e][c_e];
        temp[r_e][c_e] = 0;
      }
      if (r_e < temp.size() - 1) {
        temp[r_e][c_e] = temp[r_e + 1][c_e];
        temp[r_e + 1][c_e] = 0;
        count++;
        if (vec_equals(temp, sol)) {
          return true;
        }
        if(visited.find(stringify(temp)) == visited.end()) {
            current.push_back(temp);
            visited.insert(stringify(temp));
        }
        temp[r_e + 1][c_e] = temp[r_e][c_e];
        temp[r_e][c_e] = 0;
      }
      if (c_e > 0) {
        temp[r_e][c_e] = temp[r_e][c_e - 1];
        temp[r_e][c_e - 1] = 0;
        count++;
        if (vec_equals(temp, sol)) {
          return true;
        }
        if(visited.find(stringify(temp)) == visited.end()) {
            current.push_back(temp);
            visited.insert(stringify(temp));
        }
        temp[r_e][c_e - 1] = temp[r_e][c_e];
        temp[r_e][c_e] = 0;
      }
      if (c_e < temp[0].size()) {
        temp[r_e][c_e] = temp[r_e][c_e + 1];
        temp[r_e][c_e + 1] = 0;
        count++;
        if (vec_equals(temp, sol)) {
          return true;
        }
        if(visited.find(stringify(temp)) == visited.end()) {
            current.push_back(temp);
            visited.insert(stringify(temp));
        }
        temp[r_e][c_e + 1] = temp[r_e][c_e];
        temp[r_e][c_e] = 0;
      }
    }
    // Rearrange the vectors
    previous = current;
    current.clear();
  }
  return false;
}

int main() {

  srand(unsigned(time(0)));
  unsigned int rows, cols;
  cout << "Puzzle width: ";
  cin >> cols;
  cout << "Puzzle height: ";
  cin >> rows;

  Grid solution;
  Grid start;
  set<string> visited;

  generate_random(solution, rows, cols);
  generate_random(start, rows, cols);

  cout << "START: " << endl;
  print_grid(start);
  cout << "FINISH: " << endl;
  print_grid(solution);

  int BFS_Count = 0;
  int DFS_Count = 0;
  visited.clear();
  DFS(start, solution, visited, DFS_Count);
  visited.clear();
  BFS(start, solution, visited, BFS_Count);
  cout << "DFS: " << DFS_Count << endl << "BFS: " << BFS_Count << endl;
}