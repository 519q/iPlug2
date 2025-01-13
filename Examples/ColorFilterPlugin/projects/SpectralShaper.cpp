#include "SpectralShaper.h"
#include "IPlugConstants.h"
#include <iostream>
#include <sstream>
#include <windows.h>

void SpectralShaper::ProcessBuffer(std::array<double, RingBuffer::getChunkSize()>& ChunkBuffer, bool chunkRead, FilterParameters& params, double sampleRate)
{
  if (chunkRead)
  {
    // Process the chunk
    auto analyticSignal = computeAnalyticSignal(ChunkBuffer);
    std::array<double, RingBuffer::getChunkSize()> magnitude{}, phase{};
    extractMagnitudeAndPhase(analyticSignal, magnitude, phase);
    reconstructSignal(magnitude, phase, ChunkBuffer, sampleRate, params);
  }
}

void SpectralShaper::fwd_fft(std::vector<std::complex<double>>& data)
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

std::vector<std::complex<double>> SpectralShaper::computeAnalyticSignal(const std::array<double, RingBuffer::getChunkSize()>& ringBuffer)
{
  // Copy input to fftIn (convert double to float)

  for (int i = 0; i < RingBuffer::getChunkSize(); ++i)
  {
#ifdef MY_FFT
    fft_in_data[i] = std::complex<double>(ringBuffer[i], 0.0);
#endif // MY_FFT
#ifdef PFFFT
    fftIn[i] = static_cast<float>(ringBuffer[i]);
#endif // PFFFT
#ifdef KISS_FFT
    fftIn[i] = static_cast<kiss_fft_scalar>(ringBuffer[i]);
#endif // KISS_FFT
  }

  // Perform forward FFT
#ifdef MY_FFT
  fwd_fft(fft_in_data);
#endif // MY_FFT
#ifdef PFFFT
  fft.forward(fftIn, fftOut);
#endif // PFFFT
#ifdef KISS_FFT
  kiss_fftr(fftConfig, fftIn, fftOut);
#endif // KISS_FFT

  // Construct analytic signal
  std::vector<std::complex<double>> analyticSignal(RingBuffer::getChunkSize());

  // Positive frequencies (multiply by 2)
  for (int i = 0; i <= N / 2; ++i)
  {
    if (i == 0 || (i == N / 2 && N % 2 == 0))
    {
      // DC or Nyquist component (do not double)
#ifdef MY_FFT
      analyticSignal[i] = fft_in_data[i];
#endif // MY_FFT
#ifdef PFFFT
      analyticSignal[i] = std::complex<double>(fftOut[i].real(), fftOut[i].imag());
#endif // PFFFT
#ifdef KISS_FFT
      analyticSignal[i] = std::complex<double>(fftOut[i].r, fftOut[i].i);
#endif // KISS_FFT
    }
    else
    {
      // Positive frequencies (double)
#ifdef MY_FFT
      analyticSignal[i] = 2.0 * fft_in_data[i];
#endif // MY_FFT
#ifdef PFFFT
      analyticSignal[i] = 2.0 * std::complex<double>(fftOut[i].real(), fftOut[i].imag());
#endif // PFFFT
#ifdef KISS_FFT
      analyticSignal[i] = 2.0 * std::complex<double>(fftOut[i].r, fftOut[i].i);
#endif // KISS_FFT
    }
  }

  // Negative frequencies (set to zero)
  for (int i = N / 2 + 1; i < N; ++i)
  {
    analyticSignal[i] = std::complex<double>(0.0, 0.0);
  }

  return analyticSignal;
}

void SpectralShaper::extractMagnitudeAndPhase(const std::vector<std::complex<double>>& analyticSignal,
                                              std::array<double, RingBuffer::getChunkSize()>& magnitude,
                                              std::array<double, RingBuffer::getChunkSize()>& phase)
{
  for (int i = 0; i < RingBuffer::getChunkSize(); ++i)
  {
    magnitude[i] = std::abs(analyticSignal[i]);
    phase[i] = std::arg(analyticSignal[i]);
  }
}

void SpectralShaper::reconstructSignal(std::array<double, RingBuffer::getChunkSize()>& magnitude,
                                       std::array<double, RingBuffer::getChunkSize()>& phase,
                                       std::array<double, RingBuffer::getChunkSize()>& ringBuffer,
                                       double sampleRate,
                                       FilterParameters& params)
{
  const double deltaF = sampleRate / N; // Frequency resolution

#ifdef INVERSE_FFT
  #ifdef PFFFT
  // TODO: Mirror negative freqs in kissfft to remove saw modulation
  // 
  // Reconstruct the analytic signal from magnitude and phase
  AlignedVectorComplex analyticSignal(RingBuffer::getChunkSize() / 2 + 1);
  for (int i = 0; i <= RingBuffer::getChunkSize() / 2; ++i)
  {
    analyticSignal[i] = std::polar(magnitude[i], phase[i]);
    analyticSignal[i] *= 0.5f;
  }
  // Perform inverse FFT
  AlignedVectorFloat timeDomain(RingBuffer::getChunkSize());
  fft.inverse(analyticSignal, timeDomain);

  // Normalize the output by dividing by N
  constexpr double scale = 1.0 / RingBuffer::getChunkSize();
  // Copy the result back to the ring buffer
  for (int i = 0; i < RingBuffer::getChunkSize(); ++i)
  {
    ringBuffer[i] = static_cast<double>(timeDomain[i]) * scale;
  }
  #endif // PFFFT
  #ifdef KISS_FFT
  // Reconstruct the analytic signal from magnitude and phase
  std::vector<kiss_fft_cpx> analyticSignal(RingBuffer::getChunkSize() / 2 + 1);
  for (int i = 0; i <= RingBuffer::getChunkSize() / 2; ++i)
  {
    analyticSignal[i].r = magnitude[i] * std::cos(phase[i]);
    analyticSignal[i].i = magnitude[i] * std::sin(phase[i]);
    analyticSignal[i].r *= 0.5f; // Compensate for doubling
    analyticSignal[i].i *= 0.5f; // Compensate for doubling
  }

  // Perform inverse FFT
  std::vector<kiss_fft_scalar> timeDomain(RingBuffer::getChunkSize());
  kiss_fftri(ifftConfig, analyticSignal.data(), timeDomain.data());

  // Copy the result back to the ring buffer
  // Normalize the output by dividing by the FFT size
  constexpr double scale = 1.0 / RingBuffer::getChunkSize();
  for (int i = 0; i < RingBuffer::getChunkSize(); ++i)
  {
    ringBuffer[i] = static_cast<double>(timeDomain[i]) * scale;
  }
  #endif // KISS_FFT

#endif // INVERSE_FFT
#ifndef INVERSE_FFT
  // Reconstruct the signal by summing custom sinusoidal components
  for (int k = 0; k <= N / 2; ++k)
  {
    double freq = k * deltaF;  // Frequency of the k-th component
    double A_k = magnitude[k]; // Magnitude of the k-th component
    double phi_k = phase[k];   // Phase of the k-th component

    for (int t = 0; t < N; ++t)
    {
      double time = static_cast<double>(t) / sampleRate;
      double customSinusoid = generateCleanCos(freq, time, phi_k);
      // double customSinusoid = generateCustomSinusoid(freq, time, phi_k, params);
      ringBuffer[t] += A_k * customSinusoid;
    }
  }
  #if defined(PFFFT) || defined(KISSFFT)
  // Mirror negatives
  for (int k = 1; k < N / 2; ++k)
  {
    double freq = (N - k) * deltaF; // Frequency of the k-th component
    double A_k = magnitude[k];      // Magnitude of the k-th component
    double phi_k = -phase[k];       // Phase of the k-th component
    for (int t = 0; t < N; ++t)
    {
      double time = static_cast<double>(t) / sampleRate;
      double customSinusoid = generateCleanCos(freq, time, phi_k);
      // double customSinusoid = generateCustomSinusoid(freq, time, phi_k, params);
      ringBuffer[t] += A_k * customSinusoid;
    }
  }
  #endif
  // Normalize the output by dividing by N
  for (int t = 0; t < N; ++t)
  {
    ringBuffer[t] *= scale;
  }
#endif // !INVERSE_FFT
}

double SpectralShaper::generateCleanCos(double freq, double time, double phase)
{
  // A pure cosine wave with phase shift
  return std::cos(2.0 * iplug::PI * freq * time + phase);
  // Or alternatively:
  // return 0.5 * (std::cos(2.0 * iplug::PI * freq * time + phase) +
  //               std::cos(2.0 * iplug::PI * (-freq) * time - phase));
}

double SpectralShaper::generateCustomSinusoid(double freq, double time, double phase, FilterParameters& params)
{
  double sineValue = std::cos(2.0 * iplug::PI * freq * time + phase);
  return shapeSine(sineValue, params);
}

double SpectralShaper::shapeSine(double sineValue, FilterParameters& params)
{
  // Apply tanh shaping
  double shapedValue = std::tanh(params.m_SH_shape * sineValue);

  // Normalize to maintain the original amplitude
  static const double maxTanh = std::tanh(params.m_SH_shape);
  shapedValue /= maxTanh;

  return shapedValue;
}