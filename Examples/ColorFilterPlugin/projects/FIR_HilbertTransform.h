#pragma once
#include "IPlugConstants.h"
#include <cmath>
#include <vector>
//#include "projects/DelayLine.h"
#include "projects/FilterParameters.h"
//#include "projects/PhaseUnwrapper.h"

class FIR_HilbertTransform
{
  std::vector<double> firDelayLine;
  std::vector<double> coefficients;
  size_t position = 0;
  int m_Order{4};
  //DelayLine delayLine{};
  //PhaseUnwrapper phaseUnwrapper{};

public:
  FIR_HilbertTransform();

  double getImaginary(double input, int order);

  // a method to query the delay introduced by the filter. This is useful for aligning the original signal with the Hilbert-transformed signal
  //size_t getDelay() const;
  
  double getMagnitude(double input, int order);

  double getPhase(double input, int order);

  double getReal(double input, int order);

  void resize(int order);

  void calculateCoefficients();
};
