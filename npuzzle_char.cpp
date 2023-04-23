#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <locale>
#include <ostream>
#include <set>
#include <sys/types.h>
#include <vector>

using namespace std;


// Prints the string representation of the grid
void print_grid(const string &a, unsigned int rows, unsigned int cols) {
  for (unsigned int i = 0; i < rows; i++) {
    for(unsigned int j = 0; j < cols; j++) {
      if((int)a[i*cols + j] == -1) {
        cout << 0 << "\t";
      } else {
        cout << (int)a[i*cols + j] << "\t";
      }
    }
    cout << endl;
  }
}

// Generates a random integer orientation (for solution and start)
void generate_random(string &a, unsigned int r, unsigned int c) {
  // PROCESS: Generate lists of coordinates and numbers, shuffle, and then assign numbers to coordinates
  // Vectors for shuffling and initialization
  vector<char> nums;
  vector<pair<int, int>> coordinates;
  vector<char> temp;
  vector<vector<char>> temp_grid;
  for (unsigned int i = 0; i < r; i++) {
    temp_grid.push_back(temp);
    for (unsigned int j = 0; j < c; j++) {
      // Generate coordinates
      coordinates.push_back(make_pair(i, j));
      // -1 is "0" to avoid null terminator
      temp_grid[i].push_back(-1);
    }
  }
  // Create list of numbers (nonzero)
  for(unsigned int i = 1; i < r*c; i++) {
    nums.push_back(i);
  }
  // Shuffle both vectors
  random_shuffle(nums.begin(), nums.end());
  random_shuffle(coordinates.begin(), coordinates.end());
  // Assign
  for (unsigned int i = 0; i < nums.size(); i++) {
    temp_grid[coordinates[i].first][coordinates[i].second] = nums[i];
  }
  // Turn into the string
  a = "";
  for(unsigned int i = 0; i < temp_grid.size(); i++) {
    for(unsigned int j = 0; j < temp_grid[i].size(); j++) {
      a += temp_grid[i][j];
    }
  }
}


// Depth-First Search Solution with Memory to Prevent Repetition
bool DFS(string &n, const string &sol, set<string> &visited, int &count, unsigned int rows, unsigned int cols) {
  // Base Case & Counters
  count++;
  if (n == sol) {
    return true;
  } else if (!visited.insert(n).second) {
    return false;
  }
  // Find Hole
  unsigned int r_e;
  unsigned int c_e;
  bool b = false;
  for (unsigned int i = 0; i < rows; i++) {
    for (unsigned int j = 0; j < cols; j++) {
      if (n[i*cols + j] == -1) {
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
  // Check bounds
  if (r_e > 0) {
    // Swap
    n[cols*r_e + c_e] = n[cols*(r_e - 1) + c_e];
    n[cols*(r_e - 1) + c_e] = -1;
    // Recurse
    ret = DFS(n, sol, visited, count, rows, cols);
    // Swap back
    n[cols*(r_e - 1) + c_e] = n[cols*r_e + c_e];
    n[cols*r_e + c_e] = -1;
    // Check win
    if (ret) {
      return true;
    }
  }
  if (r_e < rows - 1) {
    n[cols*r_e + c_e] = n[cols*(r_e + 1) + c_e];
    n[cols*(r_e + 1) + c_e] = -1;

    ret = DFS(n, sol, visited, count, rows, cols);

    n[cols*(r_e + 1) + c_e] = n[cols*r_e + c_e];
    n[cols*r_e + c_e] = -1;
    if (ret) {
      return true;
    }
  }
  if (c_e > 0) {
    n[cols*r_e + c_e] = n[cols*r_e + c_e - 1];
    n[cols*r_e + c_e - 1] = -1;

    ret = DFS(n, sol, visited, count, rows, cols);

    n[cols*r_e + c_e - 1] = n[cols*r_e + c_e];
    n[cols*r_e + c_e] = -1;
    if (ret) {
      return true;
    }
  }
  if (c_e < cols - 1) {
    n[cols*r_e + c_e] = n[cols*r_e + c_e + 1];
    n[cols*r_e + c_e + 1] = -1;

    ret = DFS(n, sol, visited, count, rows, cols);

    n[cols*r_e + c_e + 1] = n[cols*r_e + c_e];
    n[cols*r_e + c_e] = -1;
    if (ret) {
      return true;
    }
  }
  return false;
}

// Breadth-First-Search Solution with Memory to Prevent Repetition
bool BFS(const string &n, const string &sol, set<string> &visited, int &count, unsigned int rows, unsigned int cols) {
  // Check if at soln first:
  count++;
  visited.insert(n);
  if (n == sol) {
    return true;
  }
  // Vectors for current and previous layer of BFS tree
  vector<string> current;
  vector<string> previous;
  previous.push_back(n);
  for (; previous.size() > 0;) {
    // loop over previous and add each
    for (unsigned int i = 0; i < previous.size(); i++) {
        string temp = previous[i];
      // Get Zero Location
      unsigned int r_e;
      unsigned int c_e;
      bool b = false;
      for (unsigned int j = 0; j < rows; j++) {
        for (unsigned int k = 0; k < cols; k++) {
          if (temp[j*cols + k] == -1) {
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
      // Check bound
      if (r_e > 0) {
        // Swap
        temp[cols*r_e + c_e] = temp[cols*(r_e - 1) + c_e];
        temp[cols*(r_e - 1) + c_e] = -1;
        count++;
        // Check soln
        if (temp == sol) {
          return true;
        }
        // Push into next row
        if(visited.insert(temp).second) {
          current.push_back(temp); 
        }
        // Swap back
        temp[cols*(r_e - 1) + c_e] = temp[cols*r_e + c_e];
        temp[cols*r_e + c_e] = -1;
      }
      if (r_e < rows - 1) {
        temp[cols*r_e + c_e] = temp[cols*(r_e + 1) + c_e];
        temp[cols*(r_e + 1) + c_e] = -1;
        count++;
        if (temp == sol) {
          return true;
        }
        // Push into next row
        if(visited.insert(temp).second) {
          current.push_back(temp); 
        }
        temp[cols*(r_e + 1) + c_e] = temp[cols*r_e + c_e];
        temp[cols*r_e + c_e] = -1;
      }
      if (c_e > 0) {
        temp[cols*r_e + c_e] = temp[cols*r_e + c_e - 1];
        temp[cols*r_e + c_e - 1] = -1;
        count++;
        if (temp == sol) {
          return true;
        }
        // Push into next row
        if(visited.insert(temp).second) {
          current.push_back(temp); 
        }
        temp[cols*r_e + c_e - 1] = temp[cols*r_e + c_e];
        temp[cols*r_e + c_e] = -1;
      }
      if (c_e < cols - 1) {
        temp[cols*r_e + c_e] = temp[cols*r_e + c_e + 1];
        temp[cols*r_e + c_e + 1] = -1;
        count++;
        if (temp == sol) {
          return true;
        }
        // Push into next row
        if(visited.insert(temp).second) {
          current.push_back(temp); 
        }
        temp[cols*r_e + c_e + 1] = temp[cols*r_e + c_e];
        temp[cols*r_e + c_e] = -1;
      }
    }
    // Rearrange the vectors
    previous = current;
    current.clear();
  }
  return false;
}

// Console-Playable Version of Game
bool UserPlay(const string &n, const string &sol, unsigned int rows, unsigned int cols) {
  cout << "FORMAT: type the NUMBER you want to move. QUIT: -1" << endl;
  int count = 0; 
  // Copy string
  string t(n);
  // Main game loop:
  for(;t != sol;count++) {
    cout << "CURRENT: " << endl;
    print_grid(t,rows,cols);
    cout << "GOAL: " << endl;
    print_grid(sol,rows,cols);
    cout << endl;

    int num;
    cin >> num;
    // Quit condition
    if(num == -1) {
      return false;
    }
    // Find number
    int n_r = -1;
    int n_c = -1;
    bool b = false;
    for(unsigned int i = 0 ; i < rows; i++) {
      for(unsigned int j = 0; j < cols; j++) {
        if((int)t[i*cols + j] == num) {
          n_r = i;
          n_c = j;
          b = true;
          break;
        }
      }
      if(b) {
        break;
      }
    }
    // If no number was found error and continue
    if(n_c == -1 && n_r == -1) {
      cout << "ERROR: Number not found, pick a different number." << endl;
      count--;
      continue;
    }

    // Check for zero bordering
    int z_r = -1;
    int z_c = -1;
    if(n_r > 0 && t[cols*(n_r - 1) + n_c] == -1) {
      z_r = n_r - 1;
      z_c = n_c;
    } else if(unsigned(n_r) < rows - 1 && t[cols*(n_r + 1) + n_c] == -1) {
      z_r = n_r + 1;
      z_c = n_c;
    } else if(n_c > 0 && t[cols*n_r + n_c - 1] == -1) {
      z_r = n_r;
      z_c = n_c - 1;
    } else if(unsigned(n_c) < cols - 1 && t[cols*n_r+ n_c + 1] == -1) {
      z_r = n_r;
      z_c = n_c + 1;
    }

    // If no zero was found error and continue
    if(z_c == -1 && z_r == -1) {
      cout << "ERROR: Number cannot be moved, pick a different number." << endl;
      count--;
      continue;
    }
    
    // Valid move, execute it.
    t[cols*z_r + z_c] = t[cols*n_r + n_c];
    t[cols*n_r + n_c] = -1;
  }
  cout << "WIN!: Total Moves: " << count << endl;
  return true;
}


int main() {
  // Random seed
  srand(unsigned(time(0)));
  unsigned int rows, cols;
  cout << "Puzzle width: ";
  cin >> cols;
  cout << "Puzzle height: ";
  cin >> rows;

  string solution;
  string start;
  set<string> visited;

  generate_random(solution, rows, cols);
  generate_random(start, rows, cols);

  cout << "START: " << endl;
  print_grid(start,rows,cols);
  cout << "FINISH: " << endl;
  print_grid(solution,rows,cols);


  UserPlay(start,solution,rows,cols);
  int BFS_Count = 0;
  int DFS_Count = 0;
  visited.clear();
  DFS(start, solution, visited, DFS_Count, rows, cols);
  visited.clear();
  BFS(start, solution, visited, BFS_Count, rows, cols);
  cout << "DFS: " << DFS_Count << endl << "BFS: " << BFS_Count << endl;
}