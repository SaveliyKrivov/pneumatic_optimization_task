#ifndef JACOBIAN_H
#define JACOBIAN_H
#include "integrate.hpp"
#include "matrix.hpp"
#include <sstream>


Matrix jacobianRemainder(Params const &params,
                         std::vector<double> const &Fvalues,
                         std::vector<double> const &IFvalues

) {
  Matrix result(Fvalues.size(), PARAM_COUNT);
  std::vector<double> Pvalues;
  for (size_t row = 0; row < Fvalues.size(); ++row) {
    double F = Fvalues[row];
    double IF = IFvalues[row];
    double V0 = params.V0;
    double a = params.a;
    double b = params.b;
    double c = params.c;
    double d = params.d;
    double g = params.g;
    // derivatives has to be multiplied by -1 as it is a residual
    result.matrix[row][0] = -(-F*F*g + (1 - exp(-IF - V0 + b))/a 
      + (IF + V0)*exp(-IF - V0 + b)/a);
    result.matrix[row][1] = -(-(1 - exp(-IF - V0 + b))*(IF + V0)/(a*a));
    result.matrix[row][2] = -(-(IF + V0)*exp(-IF - V0 + b)/a);
    result.matrix[row][3] = -F*F;
    result.matrix[row][4] = -F*F*F;
    result.matrix[row][5] = F*F*(IF + V0);
  }
  return result;
}

#endif