#pragma once
#include "IPlug_include_in_plug_hdr.h"
#include <vector>

// #include "DelayLine.h"
// #include "projects/PhaseUnwrapper.h"

// IIR Allpass-based approach
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

class IIR_HilbertTransform
{
public:
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
  // double x1[6], x2[6], y1[6], y2[6];
  std::vector<double> iirDelayLine;
  size_t delayPos;
  double totalDelay;
  int m_Order{6};
  // DelayLine DelayLine{};
};

/*
// Usage:
//AllpassHilbertTransform hilbert;
//
//// In your processing loop:
//auto output = hilbert.getReal_Imag(inputSample);
//double real = output.real;
//double imag = output.imag;
//
//or:
//auto output = hilbert.getMagintude_Phase(inputSample);
//double magnitude = output.magnitude;
//double phase = output.phase;
// */

class FIR_HilbertTransform
{
  std::vector<double> firDelayLine;
  std::vector<double> coefficients;
  size_t position = 0;
  int m_Order{4};
  // DelayLine delayLine{};
  // PhaseUnwrapper phaseUnwrapper{};

public:
  FIR_HilbertTransform();

  double getImaginary(double input, int order);

  // a method to query the delay introduced by the filter. This is useful for aligning the original signal with the Hilbert-transformed signal
  // size_t getDelay() const;

  double getMagnitude(double input, int order);

  double getPhase(double input, int order);

  double getReal(double input, int order);

  void resize(int order);

  void calculateCoefficients();
};

class LatticeHilbertTransform
{
private:
  std::vector<double> coeffs;
  std::vector<double> zReal, zImag; // Delay buffers for two parallel allpass filters
  int m_Order{4};

public:
  void Setup(int order)
  {
    if (m_Order != order)
    {
      m_Order = order;
      coeffs.resize(m_Order);
      zReal.assign(m_Order, 0.0);
      zImag.assign(m_Order, 0.0);

      for (int n = 0; n < m_Order; n++)
      {
        double freq = (n + 0.5) / m_Order; // Normalized frequency
        double omega = iplug::PI * freq;
        coeffs[n] = (sin(omega) - 1.0) / (sin(omega) + 1.0);
      }
    }
  }

  Real_Imag_Output getRealImag(double input, int order)
  {
    Setup(order);
    double vReal = input;
    double vImag = input;

    for (int n = 0; n < m_Order; n++)
    {
      // First allpass filter for real part (0-degree shift)
      double tempReal = vReal - coeffs[n] * zReal[n];
      vReal = coeffs[n] * tempReal + zReal[n];
      zReal[n] = tempReal;

      // Second allpass filter for imaginary part (90-degree shift)
      double tempImag = vImag - coeffs[n] * zImag[n];
      vImag = coeffs[n] * tempImag + zImag[n];
      zImag[n] = tempImag;
    }
    return {vReal, vImag};
  }
  Magn_Phas_Output getMagnPhase(double input, int order)
  {
    Real_Imag_Output r_i = getRealImag(input, order);
    double magnitude = std::sqrt(r_i.real * r_i.real + r_i.imag * r_i.imag);
    double phase = std::atan2(r_i.imag, r_i.real);
    return {magnitude, phase};
  }
};