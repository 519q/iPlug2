#include "IIR_HilbertTransform.h"
#include <cmath>

void IIR_HilbertTransform::calculateCoefficients()
{
  allpassCoeffs.resize(m_Order);
  // Example: Simple heuristic to initialize coefficients
  for (int i = 0; i < m_Order; ++i)
  {
    allpassCoeffs[i] = 0.9 + 0.1 * (i / static_cast<double>(m_Order));
  }
}

void IIR_HilbertTransform::initializeStates()
{
  x1.resize(m_Order, 0.0);
  x2.resize(m_Order, 0.0);
  y1.resize(m_Order, 0.0);
  y2.resize(m_Order, 0.0);
  initializeDelayLine();
}

void IIR_HilbertTransform::initializeDelayLine()
{ // Calculate total group delay
  totalDelay = 0;
  for (double a : allpassCoeffs)
  {
    // Group delay of each allpass section at center frequency
    totalDelay += (1 - a * a) / (1 + a * a);
  }

  // Initialize delay line for real signal
  iirDelayLine.resize(static_cast<size_t>(std::ceil(totalDelay)));
  delayPos = 0;
}

IIR_HilbertTransform::IIR_HilbertTransform()
{
  calculateCoefficients();
  initializeStates();
}

//double IIR_HilbertTransform::getDelay() const
//{
//  // Calculate total group delay
//  double totalDelay = 0;
//  for (double a : allpassCoeffs)
//  {
//    // Group delay of each allpass section at center frequency
//    totalDelay += (1 - a * a) / (1 + a * a);
//  }
//  return totalDelay;
//}

IIR_HilbertTransform::Magn_Phas_Output IIR_HilbertTransform::getMagnitude_Phase(double input, int order)
{
  auto r_i_out = getReal_Imag(input, order);
  double magnitude = std::sqrt(r_i_out.real * r_i_out.real + r_i_out.imag * r_i_out.imag);
  double phase = std::atan2(r_i_out.imag, r_i_out.real);
  return {magnitude, phase};
}

IIR_HilbertTransform::Real_Imag_Output IIR_HilbertTransform::getReal_Imag(double input, int order)
{
  if (m_Order != order)
  {
    m_Order = order;
    calculateCoefficients();
    initializeStates();
  }
  // Get imaginary (quadrature) component
  double imag = getImaginary(input, order);

  // Delay the real component to match
  iirDelayLine[delayPos] = input;
  delayPos = (delayPos + 1) % iirDelayLine.size();
  double real = iirDelayLine[(delayPos + iirDelayLine.size() - static_cast<size_t>(totalDelay / 2)) % iirDelayLine.size()];

  return {real, imag};
}

double IIR_HilbertTransform::getImaginary(double input, int order)
{

  double output = input;
  for (int i = 0; i < m_Order; ++i)
  {
    double a = allpassCoeffs[i];
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

// double AllpassHilbertTransform::processQuadrature(double input)
//{
//   // Single allpass section for testing
//   double output = input;
//
//   // Try just first section first
//   double a = allpassCoeffs[0];
//   double w = input - a * a * x2[0] + a * y1[0];
//   double y = a * w + y1[0];
//   y /= (1.0 + std::abs(a));
//   y2[0] = y1[0];
//   y1[0] = y;
//   x2[0] = x1[0];
//   x1[0] = w;
//
//   return y;
// }