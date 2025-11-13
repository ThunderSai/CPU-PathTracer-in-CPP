#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <cmath>

static inline double rand01() { 
    return rand() / (RAND_MAX + 1.0); 
}

static inline double clampd(double x, double a, double b) { 
    return x < a ? a : (x > b ? b : x); 
}

#endif
