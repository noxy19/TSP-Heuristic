#include "tsp_read.h"
#include <math.h>

using namespace std;

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
