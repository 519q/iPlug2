#pragma once
#include "IPlugConstants.h"
#include <cmath>
#include <vector>
#include "projects/DelayLine.h"
//#include "projects/PhaseUnwrapper.h"

class FIR_HilbertTransform
{
  std::vector<double> firDelayLine;
  std::vector<double> coefficients;
  size_t position = 0;
  //DelayLine delayLine{};
  //PhaseUnwrapper phaseUnwrapper{};

public:
  FIR_HilbertTransform(int order = 12);

  double getImaginary(double input);

  // a method to query the delay introduced by the filter. This is useful for aligning the original signal with the Hilbert-transformed signal
  //size_t getDelay() const;
  
  double getMagnitude(double input);

  double getPhase(double input);

  double getReal(double input);
};
