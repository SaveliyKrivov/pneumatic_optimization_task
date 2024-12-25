#ifndef MODELDATA_H
#define MODELDATA_H
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <istream>
#include <sstream>
#include "integrate.hpp"

void generateTestData(
  Params const & targetParams,
  std::vector<double>& time,
  std::vector<double>& targetP,
  std::vector<double>& Fvalues,
  std::vector<double>& IFvalues,
  double timeStep,
  size_t testDataSize,
  double noiseLevel,
  double minF, 
  double maxF
) {
    targetP.resize(testDataSize);
    Fvalues.resize(testDataSize);
    time.resize(testDataSize);
    IFvalues.resize(testDataSize);
    for (size_t i = 0; i < testDataSize; ++i) {
      time[i] = i*timeStep;
    }
    
    double fStep = (maxF - minF)/testDataSize;
    for (size_t i = 0; i < testDataSize; ++i) {
      Fvalues[i] = fStep*i;
    }
    simpsonIntegrate(Fvalues, IFvalues, timeStep);
    // generate target P values
    P(Fvalues, IFvalues, targetP, targetParams);

    // add noise to target P values
    for (size_t i = 0; i < targetP.size(); ++i) {
      targetP[i] += noiseLevel * (std::rand() / static_cast<double>(RAND_MAX) - 2.0);
    }
}

// load data from file with 3 columns
// and return number of rows and number of columns
// returns part of the data from a file between
// startId and endId
void loadDataFromFile(const std::string & filename,
                      std::vector<double> & time,
                      std::vector<double> & F,
                      std::vector<double> & P,
                      std::vector<double> & IF, 
                      size_t startId, 
                      size_t endId) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Can't open file " << filename << std::endl;
    return;
  }

  std::string line;
  // Skip the first line
  if (!std::getline(file, line)) {
    std::cerr << "Error: File is empty or can't read first line " << filename << std::endl;
    return;
  }
  size_t rows = 0;
  while (std::getline(file, line)) {
    ++rows;
    std::istringstream iss(line);
    double t, f, pValue;
    if (!(iss >> t >> pValue >> f)) {
      std::cerr << "Error: Can't parse line " << line << std::endl;
      return;
    }
    time.push_back(t);
    F.push_back(f);
    P.push_back(pValue);
  }
  simpsonIntegrate(F, IF, time[1] - time[0]);

  if (startId >= endId || endId > time.size()) {
    std::cerr << "Error: Invalid startId or endId" << std::endl;
    return;
  }

  time = std::vector<double>(time.begin() + startId, time.begin() + endId);
  F = std::vector<double>(F.begin() + startId, F.begin() + endId);
  P = std::vector<double>(P.begin() + startId, P.begin() + endId);
  IF = std::vector<double>(IF.begin() + startId, IF.begin() + endId);

}

#endif