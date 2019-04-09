#ifndef POLYNOM_H
#define POLYNOM_H

#include <vector>
#include <math.h>

class Polynom
{
public:
    Polynom();
    Polynom(const std::vector<double>&);

    double operator()(double) const;
    void setValue(const std::vector<double>&);

private:
    int deg;
    std::vector<double> arr;
};

#endif // POLYNOM_H
