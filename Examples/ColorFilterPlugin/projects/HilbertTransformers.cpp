#include "HilbertTransformers.h"
#include <cmath>

IIR_HilbertTransform::IIR_HilbertTransform()
{
  PrecomputeCoefficients();
  initializeDelayLine();
};
std::vector<double>& IIR_HilbertTransform::GetCoefficients(int order) { return IIRPrecomputedCoeffs[order - 1]; }
void IIR_HilbertTransform::PrecomputeCoefficients()
{
  for (int order = 1; order <= IIR_MAX_ORDER; order++)
  {
    IIRPrecomputedCoeffs[order - 1].resize(order);
    for (int i = 0; i < order; ++i)
    {
      IIRPrecomputedCoeffs[order - 1][i] = 0.9 + 0.1 * (i / static_cast<double>(order));
    }
  }
}
void IIR_HilbertTransform::initializeDelayLine()
{ // Calculate total group delay
  for (int d = 0; d < IIR_MAX_ORDER; ++d)
  {
    totalDelayArr[d] = 0;
    for (double a : GetCoefficients(d))
    {
      // Group delay of each allpass section at center frequency
      totalDelayArr[d] += (1 - a * a) / (1 + a * a);
    }
  }
}

Magn_Phas_Output IIR_HilbertTransform::getMagnitude_Phase(double input, int order)
{
  //int order = mapParamToOrder(orderControl, IIR_MAX_ORDER, 1, 1);
  auto r_i_out = getReal_Imag(input, order);
  double magnitude = std::sqrt(r_i_out.real * r_i_out.real + r_i_out.imag * r_i_out.imag);
  double phase = std::atan2(r_i_out.imag, r_i_out.real);
  return {magnitude, phase};
}

Real_Imag_Output IIR_HilbertTransform::getReal_Imag(double input, int order)
{
  double imag = getImaginary(input, order);
  iirDelayLine[delayPos] = input;
  delayPos = (delayPos + 1) % order;
  double real = iirDelayLine[(delayPos + order - static_cast<size_t>(totalDelayArr[order] / 2)) % order];
  return {real, imag};
}

double IIR_HilbertTransform::getImaginary(double input, int order)
{
  double output = input;
  for (int i = 0; i < order; ++i)
  {
    double a = IIRPrecomputedCoeffs[order - 1][i];
    double w = output - a * a * x2[i] + a * y1[i];
    double y = a * w + y1[i];

    // normalization
    y /= (1.0 + std::abs(a));

    y2[i] = y1[i];
    y1[i] = y;
    x2[i] = x1[i];
    x1[i] = w;

    output = y;
  }
  return output;
}

FIR_HilbertTransform::FIR_HilbertTransform() { PrecomputeCoefficients(); }
std::vector<double>& FIR_HilbertTransform::GetCoefficients(int order) { return FIRPrecomputedCoeffs[order - 1]; }
void FIR_HilbertTransform::PrecomputeCoefficients()
{
  for (int order = 1; order <= FIR_MAX_ORDER; order++)
  {
    FIRPrecomputedCoeffs[order - 1].resize(order);
    for (int i = 0; i < order; i++)
    {
      if (i == order / 2)
      {
        FIRPrecomputedCoeffs[order - 1][i] = 0.0f;
      }
      else
      {
        double n = i - order / 2.;
        FIRPrecomputedCoeffs[order - 1][i] = 2.0 / (iplug::PI * n) * (1.0 - std::cos(iplug::PI * n)); // Window function
      }
    }
  }
}
double FIR_HilbertTransform::getImaginary(double input, int order)
{
  firDelayLine[position] = input;
  double output = 0.0;

  int index = position;
  for (size_t i = 0; i < GetCoefficients(order).size(); i++)
  {
    output += firDelayLine[index] * GetCoefficients(order)[i];
    index = (index - 1 + firDelayLine.size()) % firDelayLine.size();
  }

  position = (position + 1) % firDelayLine.size();
  return output;
}

Magn_Phas_Output FIR_HilbertTransform::getMagnitude_Phase(double input, int order)
{
  double ImaginaryOut = getImaginary(input, order);
  double delayedInput = input;
  double magnitude = std::sqrt(delayedInput * delayedInput + ImaginaryOut * ImaginaryOut);
  double phase = std::atan2(ImaginaryOut, delayedInput);
  return {magnitude, phase};
}

double FIR_HilbertTransform::getMagnitude(double input, int order)
{
  double ImaginaryOut = getImaginary(input, order);
  double delayedInput = input;
  return std::sqrt(delayedInput * delayedInput + ImaginaryOut * ImaginaryOut);
}

double FIR_HilbertTransform::getPhase(double input, int order)
{
  double ImaginaryOut = getImaginary(input, order);
  double delayedInput = input;
  return std::atan2(ImaginaryOut, delayedInput);
}

double FIR_HilbertTransform::getReal(double input, int order) { return getMagnitude(input, order) * std::cos(getPhase(input, order)); }

LatticeHilbertTransform::LatticeHilbertTransform()
{
  PrecomputeCoefficients();
}

void LatticeHilbertTransform::PrecomputeCoefficients()
{
  for (int order = 1; order <= LATTICE_MAX_ORDER; order++)
  {
    latticePrecomputedCoeffs[order - 1].resize(order);
    for (int n = 0; n < order; n++)
    {
      double freq = (n + 0.5) / order;
      double omega = iplug::PI * freq;
      latticePrecomputedCoeffs[order - 1][n] = (sin(omega) - 1.0) / (sin(omega) + 1.0);
    }
  }
}

std::vector<double>& LatticeHilbertTransform::GetCoefficients(int order) { return latticePrecomputedCoeffs[order - 1]; }

Real_Imag_Output LatticeHilbertTransform::getRealImag(double input, int order, std::vector<double>& coeffs)
{
  double vReal = input;
  double vImag = input;

  for (int n = 0; n < order; n++)
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

Magn_Phas_Output LatticeHilbertTransform::getMagnitude_Phase(double input, int order)
{
  //int order = mapParamToOrder(orderControl, LATTICE_MAX_ORDER, 1, 1);
  Real_Imag_Output r_i = getRealImag(input, order, GetCoefficients(order));
  double magnitude = std::sqrt(r_i.real * r_i.real + r_i.imag * r_i.imag);
  double phase = std::atan2(r_i.imag, r_i.real);
  return {magnitude, phase};
}

 Magn_Phas_Output Hilbert_Transformer::ProcessIIR(double input, double controlParam)
{
  return IIRmixer.process(
    input, controlParam, IIR_MAX_ORDER, 1, 1, [this](IIR_HilbertTransform& filter, double sample, int size) -> Magn_Phas_Output { return filter.getMagnitude_Phase(sample, size); });
}
Magn_Phas_Output Hilbert_Transformer::ProcessFIR(double input, double controlParam, int offset)
{
  return FIRmixer.process(
    input, controlParam, FIR_MAX_ORDER, 1, 2, [this](FIR_HilbertTransform& filter, double sample, int size) -> Magn_Phas_Output { return filter.getMagnitude_Phase(sample, size); }, offset);
}
Magn_Phas_Output Hilbert_Transformer::ProcessLATTICE(double input, double controlParam)
{
  return LATTICEmixer.process(
    input, controlParam, LATTICE_MAX_ORDER, 1, 1, [this](LatticeHilbertTransform& filter, double sample, int size) -> Magn_Phas_Output { return filter.getMagnitude_Phase(sample, size); });
}