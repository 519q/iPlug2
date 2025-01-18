#include "IIR_HilbertTransform.h"
#include <cmath>

IIR_HilbertTransform::IIR_HilbertTransform()
{
  // Coefficients for 90° phase shift
  allpassCoeffs = {0.6923878, 0.9360654322959, 0.9882295226860, 0.9971486762419, 0.9992646999119, 0.9998117571563};
  // Initialize states
  for (int i = 0; i < 6; ++i)
  {
    x1[i] = x2[i] = y1[i] = y2[i] = 0.0;
  }

  // Calculate total group delay
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

IIR_HilbertTransform::Magn_Phas_Output IIR_HilbertTransform::getMagintude_Phase(double input)
{
  auto r_i_out = getReal_Imag(input);
  double magnitude = std::sqrt(r_i_out.real * r_i_out.real + r_i_out.imag * r_i_out.imag);
  double phase = std::atan2(r_i_out.imag, r_i_out.real);
  return {magnitude, phase};
}

IIR_HilbertTransform::Real_Imag_Output IIR_HilbertTransform::getReal_Imag(double input)
{
  // Get imaginary (quadrature) component
  double imag = processQuadrature(input);

  // Delay the real component to match
  iirDelayLine[delayPos] = input;
  delayPos = (delayPos + 1) % iirDelayLine.size();
  double real = iirDelayLine[(delayPos + iirDelayLine.size() - static_cast<size_t>(totalDelay / 2)) % iirDelayLine.size()];

  return {real, imag};
}

double IIR_HilbertTransform::processQuadrature(double input)
{
  double output = input;
  for (int i = 0; i < 6; ++i)
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