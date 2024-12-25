#ifndef FUNCTION_H
#define FUNCTION_H
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <istream>
#include <sstream>
#include "matrix.hpp"

#define PARAM_COUNT   6


struct Params {
  double V0, a, b, c, d, g;
  void print() {
    std::cout << V0 << " " << a << " " << b << " " 
    << c << " " << d << " " << g << std::endl;
  }
  void print(std::ofstream & stream) {
    stream << V0 << " " << a << " " << b << " " 
    << c << " " << d << " " << g << std::endl;
  }
};

// integrate from 0 step to upperStep
// considering equal time interval between 
// FValues
// retunr values of integrated function in IFValues
void simpsonIntegrate(
  std::vector<double> const & Fvalues,
  std::vector<double> & IFvalues,
  double intergrationTimeStep) {
    double IF = 0.0;
    size_t upperStep = Fvalues.size();
    IFvalues.resize(upperStep);
    for (size_t i = 0; i < upperStep - 1; i+=2) {
      IF += intergrationTimeStep / 3 * (Fvalues[i] + 4*Fvalues[i+1] + Fvalues[i+2]);
      IFvalues[i] = IF;
    }
    for (size_t i = 1; i < upperStep - 1; i+=2) {
      IFvalues[i] = 0.5 * (IFvalues[i-1] + IFvalues[i+1]);
    }
    IFvalues[upperStep-1] = IF;
}

void P(
  
  std::vector<double> const & Fvalues,
  std::vector<double> const & IFvalues,
  std::vector<double> & Pvalues,
  Params const & params) {
  
    Pvalues.resize(Fvalues.size());
    for (size_t i = 0; i < Fvalues.size(); ++i) {
      
    double F = Fvalues[i];
    double IF = IFvalues[i];

    Pvalues[i] = F*(F*F*params.d + F*(params.c - params.g*(IF + params.V0))) + 
          (1 - exp(-IF - params.V0 + params.b))*(IF + params.V0)/params.a;
    }
  }


Matrix residual(
  std::vector<double> const &Ptarget,
  Params const &params,
  std::vector<double> const &Fvalues,
  std::vector<double> const &IFvalues,
  double& sumOfSquares) {
  
  Matrix result(Ptarget.size(), 1);
  std::vector<double> Pvalues;
  P(Fvalues, IFvalues, Pvalues, params);
  sumOfSquares = 0.0;
  for (size_t i = 0; i < Ptarget.size(); ++i) {
    result.matrix[i][0] = Ptarget[i] - Pvalues[i];
    sumOfSquares += result.matrix[i][0] * result.matrix[i][0];
  }
  return result;
}

#endif