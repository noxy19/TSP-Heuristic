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
        if(i == city1) continue;
        if(city1 < i and TSP[city1][i] < actual_cost or TSP[i][city1] < actual_cost)
            return i;
    }
    return -1;
}

bool is_a_tour(VP(int)& best_solution)
{
    vector<bool> visit(best_solution.size());
    int city1 = 0;
    int city2 = best_solution[city1].first == city1 ? best_solution[city1].second : best_solution[city1].first;
    visit[city1] = visit[city2] = true;

    //keep traveling until we found a repited vertex
    while(1)
    {
        city1 = city2;
        city2 = best_solution[city1].first == city1 ? best_solution[city1].second : best_solution[city1].first;

        //if we arrive to the first city we have to check if we've visited all the cities
        if(city2 == 0)
        {
            //int count = 0;
            for(int i = 0; i < visit.size(); ++i)
            {
                //if(!visit[i]) count++;
                if(!visit[i]) return false;
            }
            //cout << "COUNT NOT VISITED = " << count << endl;
            //if(count > 0) return false;
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
    //t1 -/- t2,  t2 - t3, t3 -/- t4, t4 - t5, etc

    cout << "create changes t = " << endl;
    for(int i = 0; i < t.size(); ++i){
        cout << " " << t[i] << " , " ;
    }
    cout << endl;

    for(int i = 0; i < t.size(); ++i)
    {
        int actual = t[i];
        if(i == t.size()-1){
            if(best_solution[actual].first == t[i-1])
                best_solution[actual].first = t[0];
            else best_solution[actual].second = t[0];
        }else if(i == 0){
            if(best_solution[actual].first == t[1])
                best_solution[actual].first = t.back();
            else best_solution[actual].second = t.back();
        }else if(i%2 == 0){
            if(best_solution[actual].first == t[i+1])
                best_solution[actual].first = t[i-1];
            else best_solution[actual].second = t[i-1];
        }else{
            if(best_solution[actual].first == t[i-1])
                best_solution[actual].first = t[i+1];
            else best_solution[actual].second = t[i+1];
        }
    }
    return best_solution;
}

bool equal_pair(pair<int,int> p1, pair<int,int> p2)
{
    if(p1.first == p2.first and p1.second == p2.second or
            p1.second == p2.first and p1.first == p2.second)
        return true;
    return false;
}

bool check_links(pair<int,int> xi, pair<int,int> yi, VP(int) x, VP(int) y)
{
    //xi not in y
    //yi not in x

    for(int i = 0; i < x.size(); i++)
    {
        if(equal_pair(yi, x[i]))
            return false;
    }

    for(int i = 0; i < y.size(); i++)
    {
        if(equal_pair(xi, y[i]))
            return false;
    }
    return true;
}

void print_vp(VP(int) vp)
{
    for(int i = 0; i < vp.size(); ++i)
    {
        cout << i << " = [" << vp[i].first << " , " << vp[i].second << "]";
        cout << endl;
    }
    cout << endl << endl;
}

void heuristic_lin_kernighan(VP(int)& best_solution, MATRIX(double)& TSP)
{
    double G = 0; //best improvement made so far.

    for(int i = 0; i < best_solution.size(); ++i)
    {
        cout << "***************************" <<endl;
        cout << "ROUND = " << i << endl;

        vector<int> t;
        VP(int) x, y;

        //inicialitzation
        t.push_back(i);
        t.push_back(best_solution[t[0]].first);
        x.push_back(make_pair(t[0], t[1]));

        //3) search y1
        int t3 = search_improvement(t[0], t[1], TSP);
        if(t3 < 1) continue;
        t.push_back(t3);
        y.push_back(make_pair(t[1], t[2]));

        //4) chose xi and yi
        while(1)
        {

            int actual_cost = edge_cost(x.back().first, x.back().second);
            bool found = false;
            //4A) check if we join t1 with the last tX(taux) the result is a tour
            //search for the good ti (we have 2 options first,second node from ti-1)

            int ti = best_solution[t.back()].first;
            t.push_back(ti);
            VP(int) aux_solution = create_changes(t, best_solution);
            if(!is_a_tour(aux_solution)){
                t[t.size()-1] = best_solution[t.back()].second;
                aux_solution = create_changes(t, best_solution);
                if(!is_a_tour(aux_solution)){
                    t.pop_back();
                    cout <<"NO X" << endl;
                    break;
                }
            }
            //edge xi
            pair<int, int> xi = make_pair(t[t.size()-1], t.back());

            //find yi start point is the endpoint of xi
            for(int j = 0; j < TSP.size(); ++j)
            {
                if(edge_cost(j, x.back().second) < actual_cost)
                {
                    //4B) find yi that satisfies 4c 4d 4e else go to step5
                    //4C) xi cannot be a link previously joined and yi not a link previously broken
                    pair<int, int> yi = make_pair(xi.second, j);

                    if(!check_links(xi, yi, x, y))
                    {
                        continue;
                    }

                    //4D) Check the gain

                    //4E) The yi chosen must permit the breaking of xi+1

                    //int tnext1 = best_solution[j].first;
                    //int tnext2 = best_solution[j].second;

                    //pair<int, int> xn1 = make_pair(j, tnext1);
                    //pair<int, int> xn2 = make_pair(j, tnext2);
                    //VP(int) aux(0);

                    //if(!check_links(xn1, yi, aux, y) and !check_links(xn2, yi, aux, y))
                    //{
                        //continue;
                    //}

                    //4F) Check if the value is better than the previos y

                    int t_size = t.size();
                    t.push_back(j);
                    found = true;
                    y.push_back( make_pair(t[t_size-2], t[t_size-1]) );
                    break;
                }
            }

            //5) Keep doing this until no improvement found
            if(!found){
                cout << i <<"NOT FOUND" << endl;
                t.pop_back();
                break;
            }
        }


        //6) Backtracking:
        //************************************************//
        //6A) Choose y2 can increase length while g1 + g2 > 0.
        //6B) Try another x2 in 4B
        //6C) y1 are selected in order to increase length.
        //6D) Try another x1
        //6E) Select a new t1

        //7) We terminate when we have tried all t1 without profit.

        VP(int) auxVP= create_changes(t, best_solution);
        cout << "AUX VP = " << endl;
        print_vp(auxVP);
        if(is_a_tour(auxVP)){
            best_solution = auxVP;
            print_vp(best_solution);
        }
    }

    //best_solution = create_changes(t, best_solution);
}

int main()
{
    string line;
    ifstream myfile ("../data/proba1.tsp");

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

    cout << "TRAVEL COST BEFORE LIN KERNIGHAN = " << travel_cost(best_solution, TSP);
    cout << endl;

    heuristic_lin_kernighan(best_solution, TSP);

    if(!is_a_tour(best_solution)) cout << "WRONG IS NOT A TOUR" <<endl;
    cout << "TRAVEL COST AFTER = " << travel_cost(best_solution, TSP);
    cout << endl;
}
