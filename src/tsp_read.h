#include "headers.h"
#include <string>
#include <fstream>

#ifndef __TSP_READ__
#define __TSP_READ__

void read(ifstream& myfile, VP(double)& cities);
void generateMatrix(VP(double)& cities, MATRIX(double)& TSP);

#endif
