#include "integrate.hpp"
#include "modelData.hpp"
#include "jacobian.hpp"
#include "linearsystemsolver.hpp"


int USE_SIMULATED_DATA = 0;

void readArgsFromCmdLine(Params &params, char *argv[]) {
  try {
    USE_SIMULATED_DATA = std::stoi(argv[1]);
    params.V0 = std::stod(argv[2]);
    params.a = std::stod(argv[3]);
    params.b = std::stod(argv[4]);
    params.c = std::stod(argv[5]);
    params.d = std::stod(argv[6]);
    params.g = std::stod(argv[7]);
  } catch (const std::invalid_argument &e) {
    std::cerr << "Error: Invalid argument" << std::endl;
    return;
  } catch (const std::out_of_range &e) {
    std::cerr << "Error: Argument out of range" << std::endl;
    return;
  }
}

  int main(int argc, char *argv[]) {
    double sumOfSquares = INFINITY;
    double prevSumOfSquares = INFINITY;


    // parameters used for simulating data
    // V0, a, b, c, d, g
    Params targetParams = {1.0, 1.2, 1.0, 1, 1.0, 1.3};
    // parameters to be optimized by Gauss-Newton
    // has to be close to targetParams used for simulating 
    // data
    // some sanity check on possible initial 
    // parameter values
    // IF max is about 500
    // b has to be less than 500
    // and c/g > 500
    // inital values
    
    Params optParams = {1.0, 1.0, 1.0, 1, 1.0, 1.0};
    if (argc == 8){
        readArgsFromCmdLine(optParams, argv);
      }
    if (1 == USE_SIMULATED_DATA) {
      optParams = {1.0, 1.0, 1.0, 1, 1.0, 1.0};
    }
    Params initialParams = optParams;

    std::vector<double> targetP;
    std::vector<double> Fvalues;
    std::vector<double> IFvalues;
    std::vector<double> time;

    if (1 == USE_SIMULATED_DATA) {
      generateTestData(
        targetParams,
        time, targetP, Fvalues,
        IFvalues, 0.1, 1000, 0, 0.0, 1.0);
    } else {
      loadDataFromFile(
        "ventilator_data2.csv",
        time, Fvalues, targetP,
         IFvalues, 0, 3600);
    }

    std::ofstream outFile("inputData.txt");
    if (!outFile.is_open()) {
      std::cerr << "Error: Can't open file for writing" << std::endl;
    } else {
      outFile << "targetP Fvalues IFvalues" << std::endl;
      for (size_t i = 0; i < targetP.size(); ++i) {
        outFile << targetP[i] << " " << Fvalues[i] << " " << IFvalues[i]
                << std::endl;
      }
      outFile.close();
    }

    size_t gaussNewtonIterations = 1000;
    Params prevStepParams = optParams;
    for (size_t ic = 0; ic < gaussNewtonIterations; ++ic) {
      
      prevSumOfSquares = sumOfSquares;
      Matrix resid = residual(
        targetP, optParams, Fvalues, IFvalues, sumOfSquares)
      ;
      std::cout << std::endl;
      std::cout << "Итерация " << ic << std::endl;
      if (1 == USE_SIMULATED_DATA){
          std::cout << "Цель" << std::endl;
          targetParams.print();
      }

      std::cout << std::endl;
      std::cout << "Подобранные значения" << std::endl;
      optParams.print();
      std::cout << std::endl;
        if (1 == USE_SIMULATED_DATA){
            std::cout << "Сумма квадратов: " << sumOfSquares
                      << std::endl;
        }


      
      if (
       (std::isnan(sumOfSquares) || (sumOfSquares > 1e50))) {
        std::cout << "Сумма квадратов слишком велика или NaN"
        << std::endl;
        optParams = prevStepParams;
        break;
      }    
      Matrix jac = jacobianRemainder(
              optParams, Fvalues, IFvalues
      );
    
      Matrix jacTransposed = transpose(jac);
      Matrix jacTMulByRes;
      multiplyMatrices(jacTransposed, resid, jacTMulByRes);

      Matrix jacTByJac;
      multiplyMatrices( jacTransposed, jac, jacTByJac);
      multiplyMatrixByNumber(jacTMulByRes, -1.0);
      // solve linear system jacTByJac * x = jacTMulByRes
      int n = jacTMulByRes.rows();
      Matrix deltaParams(n, 1);
      solveLinearSystemQR( jacTByJac, jacTMulByRes, deltaParams);
      prevStepParams = optParams;
      // update parameters
      optParams.V0 += deltaParams.matrix[0][0];
      optParams.a += deltaParams.matrix[1][0];
      optParams.b += deltaParams.matrix[2][0];
      optParams.c += deltaParams.matrix[3][0];
      optParams.d += deltaParams.matrix[4][0];
      optParams.g += deltaParams.matrix[5][0];  
    }

   //calculate P based on optimized parameters
   std::vector<double> optP;
   P(Fvalues, IFvalues, optP, optParams);
   std::vector<double> initialGuessP;
   P(Fvalues, IFvalues, initialGuessP, initialParams);

   std::ofstream outputFile("output.txt");
    if (outputFile.is_open()) {
      targetParams.print(outputFile);
      optParams.print(outputFile);
      initialParams.print(outputFile);
      for (int i = 0; i < Fvalues.size(); ++i) {
        outputFile << time[i] << " " << Fvalues[i] << " " << targetP[i] 
        <<" " << optP[i] << " " << initialGuessP[i] << " "<< IFvalues[i] << std::endl;
      }
      outputFile.close();
    } else {
      std::cerr << "Unable to open file for writing" << std::endl;
    }

  return 1;
}