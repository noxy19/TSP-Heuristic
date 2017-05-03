#include "tsp_read.h"
#include "lin_kern.h"

using namespace std;

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

    //generate random solution
    VP(int) best_solution(N);
    for(int i = 0; i < N; ++i)
    {
        if(i == 0){
            best_solution[i].first = N-1;
            best_solution[i].second = i+1;
        }else if(i == N - 1){
            best_solution[i].first = i-1;
            best_solution[i].second = 0;
        }else{
            best_solution[i].first = i-1;
            best_solution[i].second = i+1;
        }
    }

    int previos_cost = travel_cost(best_solution, TSP);
    cout << "TRAVEL COST BEFORE LIN KERNIGHAN = " << previos_cost;
    cout << endl;

    heuristic_lin_kernighan(best_solution, TSP);

    if(!is_a_tour(best_solution)) cout << "WRONG IS NOT A TOUR" <<endl;
    int after_cost = travel_cost(best_solution, TSP);
    cout << "TRAVEL COST AFTER = " << after_cost;
    cout << endl;
    while(after_cost < previos_cost)
    {
        previos_cost = after_cost;
        heuristic_lin_kernighan(best_solution, TSP);
        after_cost = travel_cost(best_solution, TSP);
        cout << "TRAVEL COST AFTER = " << after_cost;
        cout << endl;
    }
}
