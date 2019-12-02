#include "putils.h"

#include <iostream>
using namespace std;
bool withinBounds(double * x, double * bounds){
    if (*x <= bounds[0]){
        *x = bounds[0] + .02;
        return false;
    }
    if(*x >= bounds[1]){
        *x = bounds[1] - .02;
        return false;
    }
    return true;
}