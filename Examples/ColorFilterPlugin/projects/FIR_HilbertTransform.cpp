#include "FIR_HilbertTransform.h"

FIR_HilbertTransform::FIR_HilbertTransform()
{
  resize(m_Order);
  calculateCoefficients();
}

void FIR_HilbertTransform::resize(int order)
{
  firDelayLine.resize(m_Order);
  coefficients.resize(m_Order);
}

void FIR_HilbertTransform::calculateCoefficients()
{
  for (int i = 0; i < m_Order; i++)
  {
    if (i == m_Order / 2)
    {
      coefficients[i] = 0.0f;
    }
    else
    {
      double n = i - m_Order / 2.f;
      coefficients[i] = 2.0f / (iplug::PI * n) * (1.0f - std::cos(iplug::PI * n)); // Window function
    }
  }
}

double FIR_HilbertTransform::getImaginary(double input, int order)
{
  if (m_Order != order)
  {
    m_Order = order;
    resize(order);
    calculateCoefficients();
  }
  firDelayLine[position] = input;
  double output = 0.0f;

  int index = position;
  for (size_t i = 0; i < coefficients.size(); i++)
  {
    output += firDelayLine[index] * coefficients[i];
    index = (index - 1 + firDelayLine.size()) % firDelayLine.size();
  }

  position = (position + 1) % firDelayLine.size();
  return output;
}

// a method to query the delay introduced by the filter. This is useful for aligning the original signal with the Hilbert-transformed signal
//size_t FIR_HilbertTransform::getDelay() const
//{
//  return coefficients.size() / 2; // Delay is (order - 1) / 2
//}

double FIR_HilbertTransform::getMagnitude(double input, int order)
{
  double ImaginaryOut = getImaginary(input, order);
  //double delayedInput = delayLine.process(input);
  double delayedInput = input;
  return std::sqrt(delayedInput * delayedInput + ImaginaryOut * ImaginaryOut);
}

double FIR_HilbertTransform::getPhase(double input, int order)
{
  double ImaginaryOut = getImaginary(input, order);
  //double delayedInput = delayLine.process(input);
  double delayedInput = input;
  return std::atan2(ImaginaryOut, delayedInput);
  //double phase =  std::atan2(ImaginaryOut, delayedInput);
  //return phase + 2;
}

double FIR_HilbertTransform::getReal(double input, int order) { return getMagnitude(input, order) * std::cos(getPhase(input, order)); }

