#ifndef EQUATIONSYSTEM_H
#define EQUATIONSYSTEM_H

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef vector<vector<double>> vvd;
typedef vector<double> vd;

class EquationSystem
{
private:
    const double eps = 1e-9;

    vvd A;
    vd B;
    size_t n;

    void init(size_t);

    vvd getMinor(vvd, size_t, size_t);
    double getDet(const vvd&);

public:
    EquationSystem();
    EquationSystem(size_t);
    EquationSystem(const vvd&, const vd&);
    EquationSystem(QFile*);

    void read();
    void read(QTextStream);
    int solve(const vvd&, const vd&, vd&);
    int solve(vd&);
    void printDebug();
};

#endif // EQUATIONSYSTEM_H
