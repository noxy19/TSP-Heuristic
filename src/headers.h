#include <iostream>
#include <vector>

using namespace std;

#define MATRIX(T) vector<vector<T> >
#define VP(T) vector<pair<T, T> >

#define edge_cost(city1, city2) city1 < city2 ? TSP[city1][city2] : TSP[city2][city1]
