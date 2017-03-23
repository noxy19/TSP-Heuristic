#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

#define MATRIX(T) vector<vector<T> >
#define VP(T) vector<pair<T, T> >

#define edge_cost(city1, city2) city1 < city2 ? TSP[city1][city2] : TSP[city2][city1]

unsigned int split(const string &txt, vector<string> &strs, char ch)
{
    unsigned int pos = txt.find(ch);
    unsigned int initialPos = 0;
    unsigned int size = txt.size();
    strs.clear();

    // Decompose statement
    while( pos < size )
    {
        strs.push_back(txt.substr(initialPos, pos - initialPos + 1));
        initialPos = pos + 1;

        pos = txt.find(ch, initialPos);
    }

    // Add the last one
    strs.push_back(txt.substr(initialPos, min(pos, size) - initialPos + 1 ));

    return strs.size();
}

void read(ifstream& myfile, VP(double)& cities)
{
    string line;

    //Read all the cities
    for(int i = 0; i < cities.size(); ++i)
    {
        getline(myfile, line);
        vector<string> city;
        split(line, city, ' ');

        cities[i].first = atoi(city[1].c_str());
        cities[i].second = atoi(city[2].c_str());
    }
}

void generateMatrix(VP(double)& cities, MATRIX(double)& TSP)
{
    //Create the distances between cities
    for(int i = 0; i < cities.size(); ++i)
    {
        double x1, y1;

        x1 = cities[i].first;
        y1 = cities[i].second;

        for(int j = i+1; j < cities.size(); ++j)
        {
            double x2, y2, x, y;

            x2 = cities[j].first;
            y2 = cities[j].second;

            x = x1 - x2;
            y = y1 - y2;

            x *= x;
            y *= y;

            TSP[i][j] = sqrt( x + y );
//            cout << "TSP[" << i << "][" << j << "] = " << TSP[i][j] << endl;
        }
    }
}

void swap(int a, int b)
{
    int c = a;
    a = b;
    b = c;
}

double travel_cost(VP(int)& best_solution, MATRIX(double)& TSP)
{
    double total = 0;
    int city1 = 0;
    int city2 = best_solution[0].first;
    int cityaux = city1;
    while(1)
    {
        //add travel cost
        if(city2 < city1)
            total += TSP[city2][city1];
        else total += TSP[city1][city2];

        //travel complete
        if(city2 == 0) break;

        //search next city
        city1 = city2;
        if(best_solution[city1].first == cityaux)
            city2 = best_solution[city1].second;
        else city2 = best_solution[city1].first;
        cityaux = city1;

    }
    return total;
}

int search_improvement(int city1, int city2, MATRIX(double)& TSP)
{
    int actual_cost = city1 < city2 ? TSP[city1][city2] : TSP[city2][city1];
    for(int i = 0; i < TSP.size(); ++i)
    {
        if(city1 < i and TSP[city1][i] < actual_cost or TSP[i][city1] < actual_cost)
            return i;
    }
    return -1;
}

bool is_a_tour(VP(int)& best_solution)
{
    vector<bool> visit(TSP.size());
    int city1 = 0;
    int city2 = best_solution[city1].first == city1 ? best_solution[city1].second : best_solution[city1].first;
    visit[city1] = visit[city2] = true;

    //keep traveling until we found a repited vertex
    while(1)
    {
        city1 = city2;
        city2 = best_solution[city1].first == city ? best_solution[city1].second : best_solution[city1].first;

        //if we arrive to the first city we have to check if we've visited all the cities
        if(city2 == 0)
        {
            for(int i = 0; i < visit.size(); ++i)
                if(!visit[i]) return false;
            return true;
        }

        //check if it's the first time in this city
        if(visit[city2]) return false;

        visit[city2] = true;
    }
    return false;
}

VP(int) create_changes(vector<int>& t, VP(int) best_solution)
{
    for(int i = 0; i < t.size(); ++i)
    {
        if(i == t.size()-1){
            best_solution[t[i]].first = t[i-1];
            best_solution[t[i]].second = t[0];
        }else if(i == 0){
            best_solution[t[i]].first = t[t.size()-1];
            best_solution[t[i]].second = t[i+1];
        }else{
            best_solution[t[i]].first = t[i-1];
            best_solution[t[i]].second = t[i+1];
        }
    }
    return best_solution;
}

void heuristic_lin_kernighan(VP(int)& best_solution, MATRIX(double)& TSP)
{
    double G = 0; //best improvement made so far.
    vector<int> t;
    VP(int) x, y;

    //inicialitzation
    t.push_back(0);
    t.push_back(best_solution[t[0]].first);
    x.push_back(make_pair(t[0], t[1]));

    //3) search y1
    int t3 = search_improvement(t[0], t[1], TSP);
    if(t3 < 1) return best_solution;
    t.push_back(t3);
    y.push_back(make_pair(t[1], t[2]));

    //4) chose xi and yi
    while(1)
    {

        //int t3 = search_improvement(t[0], t[1], TSP);

        int actual_cost = edge_cost(x.back().first, x.back().second);
        int previos_size = t.size();

        //4A) check if we join t1 with the last tX(taux) the result is a tour
        //search for the good ti (we have 2 options first,second node from ti-1)
        int ti = best_solution[t.back()].first;
        t.push_back(ti);
        VP(int)& aux_solution = create_changes(t, best_solution);
        if(!is_a_tour(aux_solution)){
            t[t.size()-1] = best_solution[t.back()].second;
            aux_solution = create_changes(t, best_solution);
            if(!is_a_tour(aux_solution)){
                t.pop_back();
                continue;
            }
        }

        for(int i = 0; i < TSP.size(); ++i)
        {
            if(edge_cost(i, x.back().second) < actual_cost)
            {
                t.push_back(i);
                //int taux = best_solution[t3].first;


                //4B) find yi that satisfies 4c 4d 4e else go to step5
                //4C) xi cannot be a link previously joined and yi not a link previously broken

                //4D) Check the gain

                //4E) The yi chosen must permit the breaking of xi+1

                //4F) Check if the value is better than the previos y

                int t_size() = t.size();
                y.push_back( make_pair(t[t_size-2], t[t_size-1]) );
                break;
            }
        }

        //5) Keep doing this until no improvement found
        if(previos_size == t.size()) break;
    }


    //6) Backtracking:
    //************************************************//
    //6A) Choose y2 can increase length while g1 + g2 > 0.
    //6B) Try another x2 in 4B
    //6C) y1 are selected in order to increase length.
    //6D) Try another x1
    //6E) Select a new t1

    //7) We terminate when we have tried all t1 without profit.
}

int main()
{
    string line;
    ifstream myfile ("../data/ar9152.tsp");

    getline(myfile, line);

    int N = atoi(line.c_str());
    MATRIX(double) TSP(N, vector<double>(N));
    VP(double) cities(N);

    read(myfile, cities);

    myfile.close();

    generateMatrix(cities, TSP);

    //generate random solution
    VP(int) best_solution(N-1);
    for(int i = 0; i < N-1; ++i)
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

//    heuristic_lin_kernighan(best_solution, TSP);

    cout << "TRAVEL COST = " << travel_cost(best_solution, TSP);
    cout << endl;
}
