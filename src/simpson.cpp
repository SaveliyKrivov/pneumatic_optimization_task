#include "integrate.hpp"
#include <iostream>

int main() {
    
    
    std::vector<double> Fvalues;
    auto count = 11;
    auto step  = M_PI / count;
    for (size_t i = 0; i < count; ++i) {
      Fvalues.push_back(std::sin(i*step));
    }
    std::vector<double> IFvalues;
    simpsonIntegrate(Fvalues, IFvalues, step);
    for (auto v : IFvalues) {
        std::cout << v << std::endl;
    }

    return 0;
    
}