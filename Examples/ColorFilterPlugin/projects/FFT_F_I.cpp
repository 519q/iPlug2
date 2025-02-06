#include "FFT_F_I.h"
#include "IPlugConstants.h"
#include <iostream>
#include <sstream>
#include <windows.h>

void FFT_F_I::ProcessBuffer(std::array<double, RingBuffer::getChunkSize()>& ChunkBuffer, bool chunkRead, FilterParameters& params, double sampleRate)
{
  if (chunkRead)
  {
    // Process the chunk
    auto analyticSignal = computeAnalyticSignal(ChunkBuffer);
    std::array<double, RingBuffer::getChunkSize()> magnitude{}, phase{};
    extractMagnitudeAndPhase(analyticSignal, magnitude, phase);
    // for a while all processing is here
    reconstructSignal(magnitude, phase, ChunkBuffer, sampleRate, params);
  }
}

void FFT_F_I::fwd_fft(std::vector<std::complex<double>>& data)
{
  constexpr size_t N = RingBuffer::getChunkSize();

  // Bit-reversal permutation
  for (size_t i = 1, j = 0; i < N; ++i)
  {
    size_t bit = N >> 1;
    for (; j >= bit; bit >>= 1)
      j -= bit;
    j += bit;
    if (i < j)
      std::swap(data[i], data[j]);
  }

  // Cooley-Tukey FFT
  for (size_t len = 2; len <= N; len <<= 1)
  {
    double theta = -2 * iplug::PI / len; // Negative sign for forward FFT
    std::complex<double> wlen(std::cos(theta), std::sin(theta));

    for (size_t i = 0; i < N; i += len)
    {
      std::complex<double> w(1);
      for (size_t j = 0; j < len / 2; ++j)
      {
        std::complex<double> u = data[i + j];
        std::complex<double> v = data[i + j + len / 2] * w;
        data[i + j] = u + v;
        data[i + j + len / 2] = u - v;
        w *= wlen;
      }
    }
  }
}

std::vector<std::complex<double>> FFT_F_I::computeAnalyticSignal(const std::array<double, RingBuffer::getChunkSize()>& ringBuffer)
{
  // Copy input to fftIn (convert double to float)

  for (int i = 0; i < RingBuffer::getChunkSize(); ++i)
  {
    fft_in_data[i] = std::complex<double>(ringBuffer[i], 0.0);
  }

  // Perform forward FFT
  fwd_fft(fft_in_data);

  // Construct analytic signal
  std::vector<std::complex<double>> analyticSignal(RingBuffer::getChunkSize());

  // Positive frequencies (multiply by 2)
  for (int i = 0; i <= N / 2; ++i)
  {
    if (i == 0 || (i == N / 2 && N % 2 == 0))
    {
      // DC or Nyquist component (do not double)
      analyticSignal[i] = fft_in_data[i];
    }
    else
    {
      // Positive frequencies (double)
      analyticSignal[i] = 2.0 * fft_in_data[i];
    }
  }
  return analyticSignal;
}

void FFT_F_I::extractMagnitudeAndPhase(const std::vector<std::complex<double>>& analyticSignal,
                                              std::array<double, RingBuffer::getChunkSize()>& magnitude,
                                              std::array<double, RingBuffer::getChunkSize()>& phase) const
{
  for (int i = 0; i < N / 2; ++i)
  {
    magnitude[i] = std::abs(analyticSignal[i]);
    phase[i] = std::arg(analyticSignal[i]);
  }
  // for (int k = 1; k < N / 2; ++k)
  //{
  //   magnitude[N - k] = magnitude[k];
  //   phase[N - k] = -phase[k];
  // }
}

void FFT_F_I::reconstructSignal(std::array<double, RingBuffer::getChunkSize()>& magnitude,
                                       std::array<double, RingBuffer::getChunkSize()>& phase,
                                       std::array<double, RingBuffer::getChunkSize()>& ringBuffer,
                                       double sampleRate,
                                       FilterParameters& params) const
{
  const double deltaF = sampleRate / N; // Frequency resolution

  // Reconstruct the signal by summing custom sinusoidal components
  for (int k = 0; k <= N / 2; ++k)
  {
    double freq = k * deltaF;  // Frequency of the k-th component
    for (int t = 0; t < N; ++t)
    {
      double time = static_cast<double>(t) / sampleRate;
      ringBuffer[t] += magnitude[k] * std::cos(2.0 * iplug::PI * freq * time + phase[k]);
    }
  }
  // Normalize the output by dividing by N
  for (int t = 0; t < N; ++t)
  {
    ringBuffer[t] *= scale;
  }
}

