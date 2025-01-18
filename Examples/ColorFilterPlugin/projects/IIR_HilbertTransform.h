#pragma once
#include <vector>
#include "DelayLine.h"
//#include "projects/PhaseUnwrapper.h"
  
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

  Magn_Phas_Output getMagintude_Phase(double input);

  Real_Imag_Output getReal_Imag(double input);

  double processQuadrature(double input);

  //double getDelay() const;

private:
  std::vector<double> allpassCoeffs;
  double x1[6], x2[6], y1[6], y2[6];
  std::vector<double> iirDelayLine;
  size_t delayPos;
  double totalDelay;
  //DelayLine DelayLine{};
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