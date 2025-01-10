#include "DomeShaper.h"
#include "IPlugConstants.h"
#include <iostream>
#include <sstream>
#include <windows.h>

void DomeShaper::ProcessBuffer(std::array<double, RingBuffer::getChunkSize()>& ChunkBuffer)
{
  // Process the chunk
  auto analyticSignal = computeAnalyticSignal(ChunkBuffer);
  std::array<double, RingBuffer::getChunkSize()> magnitude{}, phase{};
  extractMagnitudeAndPhase(analyticSignal, magnitude, phase);
  reconstructSignal(magnitude, phase, ChunkBuffer);
}

std::vector<std::complex<double>> DomeShaper::computeAnalyticSignal(const std::array<double, RingBuffer::getChunkSize()>& ringBuffer)
{
  // Copy input to fftIn
  std::copy(ringBuffer.begin(), ringBuffer.end(), fftIn);

  // Perform FFT
  kiss_fftr(fftCfg, fftIn, fftOut);

  // Construct analytic signal
  std::vector<std::complex<double>> analyticSignal(RingBuffer::getChunkSize());
  for (int i = 0; i <= RingBuffer::getChunkSize() / 2; ++i)
  {
    if (i == 0 || i == RingBuffer::getChunkSize() / 2)
    {
      analyticSignal[i] = std::complex<double>(fftOut[i].r, fftOut[i].i);
    }
    else
    {
      analyticSignal[i] = 2.0 * std::complex<double>(fftOut[i].r, fftOut[i].i);
    }
  }

  // Mirror for negative frequencies
  for (int i = 1; i < RingBuffer::getChunkSize() / 2; ++i)
  {
    int neg_i = RingBuffer::getChunkSize() - i;
    analyticSignal[neg_i] = std::complex<double>(0.0, 0.0);
  }

  return analyticSignal;
}

void DomeShaper::extractMagnitudeAndPhase(const std::vector<std::complex<double>>& analyticSignal,
                                          std::array<double, RingBuffer::getChunkSize()>& magnitude,
                                          std::array<double, RingBuffer::getChunkSize()>& phase)
{
  for (int i = 0; i < RingBuffer::getChunkSize(); ++i)
  {
    magnitude[i] = std::abs(analyticSignal[i]);
    phase[i] = std::arg(analyticSignal[i]);
  }
}

std::vector<double> DomeShaper::generateCosineLUT()
{
  std::vector<double> cosineLUT(lutResolution);
  for (int i = 0; i < lutResolution; ++i)
  {
    double degrees = static_cast<double>(i) * (360.0 / lutResolution);
    double radians = degrees * iplug::PI / 180.0;
    cosineLUT[i] = std::cos(radians);
  }
  return cosineLUT;
}

double DomeShaper::getCosineFromLUT(double phase) const
{
  phase = fmod(phase, 2.0 * iplug::PI);
  if (phase < 0)
    phase += 2.0 * iplug::PI;

  double degrees = phase * 180.0 / iplug::PI;
  int index = static_cast<int>(degrees * (3600 / 360.0)) % 3600;
  return CosineLUT[index];
}

void DomeShaper::reconstructSignal(std::array<double, RingBuffer::getChunkSize()>& magnitude,
                                   std::array<double, RingBuffer::getChunkSize()>& phase,
                                   std::array<double, RingBuffer::getChunkSize()>& ringBuffer) const
{
  std::vector<double> reconstructedSignal(RingBuffer::getChunkSize());
  for (int i = 0; i < RingBuffer::getChunkSize(); ++i)
  {
    double cosineValue = getCosineFromLUT(phase[i]);
    ringBuffer[i] = magnitude[i] * cosineValue;
  }
}