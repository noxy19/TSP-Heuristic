#include "lin_kern.h"

using namespace std;

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

int search_improvement(int city1, int city2, MATRIX(double)& TSP, VP(int) best_solution)
{
    int actual_cost = city1 < city2 ? TSP[city1][city2] : TSP[city2][city1];
    for(int i = 0; i < TSP.size(); ++i)
    {
        //not the same city
        if(i == city1 or i == city2) continue;
        //ignore if it's already a link
        if(i == best_solution[city2].first or i == best_solution[city2].second) continue;
        if(city1 < i and TSP[city1][i] < actual_cost or TSP[i][city1] < actual_cost)
            return i;
    }
    return -1;
}

bool is_a_tour(VP(int)& best_solution)
{
    vector<bool> visit(best_solution.size());
    int city1 = 0;
    int city2 = best_solution[city1].first;
    visit[city1] = visit[city2] = true;


    //keep traveling until we found a repited vertex
    while(1)
    {
        int city_aux = city1;
        city1 = city2;
        city2 = best_solution[city1].first == city_aux ? best_solution[city1].second : best_solution[city1].first;

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
        if(visit[city2]){
            cout << "REPITED CITY" << endl;
            cout << endl;
            return false;
        }

        visit[city2] = true;
    }
    return false;
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

VP(int) create_changes(vector<int>& t, VP(int) best_solution)
{
    //t1 -/- t2,  t2 - t3, t3 -/- t4, t4 - t5, etc

    //cout << "create changes t = " << endl;
    //for(int i = 0; i < t.size(); ++i){
        //cout << " " << t[i] << " , " ;
    //}
    //cout << endl;

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

bool already_exist(int ti, vector<int> t)
{
    for(int i = 0; i < t.size()-1; ++i)
    {
        if(ti == t[i]) return true;
    }
    return false;
}

void heuristic_lin_kernighan(VP(int)& best_solution, MATRIX(double)& TSP)
{
    double G = 0; //best improvement made so far.
    double actual_cost = travel_cost(best_solution, TSP);

    //6E) Select a new t1 OK
    for(int i = 0, odds = 0; i < best_solution.size();)
    {

        VP(int) best_aux_solution = best_solution;

        //6C) y1 are selected in order to increase length. OK
        for(int t3 = 0; t3 < best_solution.size(); t3++)
        {
            vector<int> t;
            VP(int) x, y;
            VP(int) aux_solution;
            int aux_cost;

            //inicialitzation
            t.push_back(i);

            //6D) Try another x1 OK
            if(odds == 0){
                t.push_back(best_solution[t[0]].second);
                odds = 1;
            } else {
                t.push_back(best_solution[t[0]].first);
                odds = 0;
                ++i;
            }

            //x1
            x.push_back(make_pair(t[0], t[1]));

            //3) search y1
            //int t3 = search_improvement(t[0], t[1], TSP, best_solution);
            //if(t3 < 1) continue;
            if(t3 == t[1]) continue;
            t.push_back(t3);
            y.push_back(make_pair(t[1], t[2]));

            //4) chose xi and yi
            while(1)
            {
                int cost_edge = edge_cost(x.back().first, x.back().second);
                bool found = false;

                //4A) check if we join t1 with the last tX(taux) the result is a tour
                //search for the good ti (we have 2 options first,second node from ti-1)

                int ti = best_solution[t.back()].first;
                t.push_back(ti);
                aux_solution = create_changes(t, best_solution);
                if(already_exist(ti, t) or !is_a_tour(aux_solution)){
                    t.pop_back();
                    ti = best_solution[t.back()].second;
                    t.push_back(ti);
                    aux_solution = create_changes(t, best_solution);

                    //if not a tour break
                    if(already_exist(ti, t) or !is_a_tour(aux_solution)){
                        t.pop_back();
                        break;
                    }
                }

                aux_cost = travel_cost(aux_solution, TSP);

                if(aux_cost < actual_cost){
                    best_aux_solution = aux_solution;
                    actual_cost = aux_cost;
                }

                //edge xi
                pair<int, int> xi = make_pair(t[t.size()-1], t.back());

                //find yi start point is the endpoint of xi
                for(int j = 0; j < TSP.size(); ++j)
                {
                    //4D) Check the gain
                    //4F) Check if the value is better than the previos y
                    if(edge_cost(j, x.back().second) < cost_edge)
                    {
                        //4B) find yi that satisfies 4c 4d 4e else go to step5
                        //4C) xi cannot be a link previously joined and yi not a link previously broken
                        pair<int, int> yi = make_pair(xi.second, j);

                        if(!check_links(xi, yi, x, y) or already_exist(j, t))
                        {
                            continue;
                        }


                        //4E) The yi chosen must permit the breaking of xi+1

                        int t_size = t.size();
                        t.push_back(j);

                        //to test
                        ti = best_solution[t.back()].first;
                        t.push_back(ti);
                        aux_solution = create_changes(t, best_solution);
                        //6B) Try another x2 in 4B OK
                        if(already_exist(ti, t) or !is_a_tour(aux_solution)){
                            t.pop_back();
                            ti = best_solution[t.back()].second;
                            t.push_back(ti);
                            aux_solution = create_changes(t, best_solution);

                            //if not a tour break
                            if(already_exist(ti, t) or !is_a_tour(aux_solution)){
                                t.pop_back();
                                t.pop_back();
                                continue;
                            }
                        }
                        t.pop_back();

                        found = true;
                        y.push_back( make_pair(t[t_size-2], t[t_size-1]) );
                        break;
                    }
                }

                //5) Keep doing this until no improvement found
                if(!found){
                    //cout << i <<"NOT FOUND" << endl;
                    t.pop_back();
                    break;
                }

            }
        }


        //6) Backtracking:
        //************************************************//
        //6A) Choose y2 can increase length while g1 + g2 > 0.
        //6B) Try another x2 in 4B OK
        //6C) y1 are selected in order to increase length. OK
        //6D) Try another x1 OK
        //6E) Select a new t1 OK

        //7) We terminate when we have tried all t1 without profit.

        best_solution = best_aux_solution;

    }
}
