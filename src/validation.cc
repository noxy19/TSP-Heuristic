#include "tsp_read.h"
#include "lin_kern.h"
#include <limits>

using namespace std;

float cost = numeric_limits<int>::max();
vector<int> act_solution;

void swap (int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void copy_array(int *a, int n, MATRIX(double) tsp)
{
    int i;
    float sum = 0;
    for(i = 0; i <= n; i++){
        int city1 = a[i % (n+1)];
        int city2 = a[(i + 1) % (n+1)];
        if(city2 < city1)
            sum += tsp[city2][city1];
        else sum += tsp[city1][city2];
    }

    if (cost > sum)
    {
        cost = sum;
        for(int j = 0; j < n+1; ++j)
        {
            act_solution[j] = a[j];
        }
    }
}

void permute(int *a, int i, int n, MATRIX(double) tsp)
{
    int j;
    if (i == n)
        copy_array(a, n, tsp);
    else
        for (j = i; j <= n; j++)
        {
            swap((a + i), (a + j));
            permute(a, i + 1, n, tsp);
            swap((a + i), (a + j));
        }
}

int generateSolution(int n, MATRIX(double) tsp)
{
    int a[n];
    for(int i = 0; i < n; ++i){
        a[i] = i;
    }
    permute(a, 0, n-1, tsp);
}

int main(int argc, const char* argv[])
{
    string path_file, file_name;
    file_name = string(argv[1]);
    path_file = "../data/" + file_name;

    ifstream myfile (path_file);

    string line;
    getline(myfile, line);

    int N = atoi(line.c_str());
    MATRIX(double) TSP(N, vector<double>(N));
    VP(double) cities(N);

    read(myfile, cities);

    myfile.close();

    generateMatrix(cities, TSP);

    act_solution.resize(N);
    int best_cost = generateSolution(N, TSP);
    cout << "cost" << cost << endl;
    for(int i = 0; i < N; ++i){
        cout << "solution[" << i << "]" << " = " << act_solution[i] << endl;
    }


    //if(!is_a_tour(best_solution)) cout << "WRONG IS NOT A TOUR" <<endl;
}
