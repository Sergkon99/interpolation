#include "polynom.h"

Polynom::Polynom(){
}

Polynom::Polynom(const std::vector<double>& v){
    deg = static_cast<int>(v.size()) - 1;

    arr = v;
}

double Polynom::operator()(double x) const{
    double ans = 0;
    for(size_t i = 0; i < arr.size(); i++)
        ans += arr[i] * pow(x,i);

    return ans;
}

void Polynom::setValue(const std::vector<double>& v){
    deg = static_cast<int>(v.size()) - 1;

    arr = v;
}
