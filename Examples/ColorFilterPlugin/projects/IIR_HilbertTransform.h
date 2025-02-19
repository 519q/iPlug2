#pragma once
#include <vector>
// #include "DelayLine.h"
// #include "projects/PhaseUnwrapper.h"

// IIR Allpass-based approach

class IIR_HilbertTransform
{
public:
  struct Real_Imag_Output
  {
    double real;
    double imag;
  };

  struct Magn_Phas_Output
  {
    double magnitude;
    double phase;
  };

  IIR_HilbertTransform();

  Magn_Phas_Output getMagnitude_Phase(double input, int order);

  Real_Imag_Output getReal_Imag(double input, int order);

  double getImaginary(double input, int order);

  void calculateCoefficients();
  void initializeStates();
  void initializeDelayLine();

  // double getDelay() const;

private:
  std::vector<double> allpassCoeffs;
  std::vector<double> x1, x2, y1, y2;
  //double x1[6], x2[6], y1[6], y2[6];
  std::vector<double> iirDelayLine;
  size_t delayPos;
  double totalDelay;
  int m_Order{6};
  // DelayLine DelayLine{};
};

/*
// Usage:
AllpassHilbertTransform hilbert;

// In your processing loop:
auto output = hilbert.getReal_Imag(inputSample);
double real = output.real;
double imag = output.imag;

or:
auto output = hilbert.getMagintude_Phase(inputSample);
double magnitude = output.magnitude;
double phase = output.phase;
*/
