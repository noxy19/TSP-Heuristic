#include "headers.h"

#ifndef __LIN_KERN__
#define __LIN_KERN__

bool is_a_tour(VP(int)& best_solution);
double travel_cost(VP(int)& best_solution, MATRIX(double)& TSP);
void heuristic_lin_kernighan(VP(int)& best_solution, MATRIX(double)& TSP);

#endif
