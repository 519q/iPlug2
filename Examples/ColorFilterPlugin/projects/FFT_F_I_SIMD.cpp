#include "FFT_F_I_SIMD.h"
#include "IPlugConstants.h"
#include <iostream>
#include <sstream>
#include <windows.h>

// namespace xs = xsimd;
// using arch_type = xs::best_arch;
// using b_type = xs::batch<double>;
// size_t inc = b_type::size;
////size_t size = res.size();
// template <class T, class Arch>
// xs::batch<T, Arch> mean(xs::batch<T, Arch> lhs, xs::batch<T, Arch> rhs)
//{
//   return (lhs + rhs) / 2;
// }

void FFT_F_I_SIMD::ProcessBuffer(std::array<double, RingBuffer::getChunkSize()>& ChunkBuffer, bool chunkRead, FilterParameters& params)
{
  if (chunkRead)
  {
    // Process the chunk
    auto analyticSignal = computeAnalyticSignal(ChunkBuffer);
    std::array<double, RingBuffer::getChunkSize()> magnitude{}, phase{};
    extractMagnitudeAndPhase(analyticSignal, magnitude, phase);
    // for a while all processing is here
    reconstructSignal(magnitude, phase, ChunkBuffer, params);
  }
}

void FFT_F_I_SIMD::fwd_fft(std::vector<std::complex<double>>& data)
{
  constexpr size_t N = RingBuffer::getChunkSize();
  using batch_type = xsimd::batch<std::complex<double>>;
  constexpr size_t simd_size = batch_type::size;

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
    double theta = -2 * iplug::PI / len;
    std::complex<double> wlen(std::cos(theta), std::sin(theta));

    for (size_t i = 0; i < N; i += len)
    {
      std::complex<double> w(1.0, 0.0);
      size_t j = 0;

      // Process SIMD-size chunks
      for (; j + simd_size <= len / 2; j += simd_size)
      {
        // Prepare twiddle factors
        std::complex<double> w_array[simd_size];
        for (size_t k = 0; k < simd_size; ++k)
        {
          w_array[k] = w;
          w *= wlen;
        }

        // Load data
        batch_type u_v = batch_type::load_unaligned(&data[i + j]);
        batch_type v_v = batch_type::load_unaligned(&data[i + j + len / 2]);
        batch_type w_v = batch_type::load_unaligned(w_array);

        // Butterfly computation
        batch_type temp = v_v * w_v;
        batch_type sum = u_v + temp;
        batch_type diff = u_v - temp;

        // Store results
        sum.store_unaligned(&data[i + j]);
        diff.store_unaligned(&data[i + j + len / 2]);
      }

      // Handle remaining elements scalar
      for (; j < len / 2; ++j)
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

std::vector<std::complex<double>> FFT_F_I_SIMD::computeAnalyticSignal(const std::array<double, RingBuffer::getChunkSize()>& ringBuffer)
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

void FFT_F_I_SIMD::extractMagnitudeAndPhase(const std::vector<std::complex<double>>& analyticSignal,
                                            std::array<double, RingBuffer::getChunkSize()>& magnitude,
                                            std::array<double, RingBuffer::getChunkSize()>& phase) const
{
  for (int i = 0; i < N / 2; ++i)
  {
    magnitude[i] = std::abs(analyticSignal[i]);
    phase[i] = std::arg(analyticSignal[i]);
  }
}

void FFT_F_I_SIMD::reconstructSignal(std::array<double, RingBuffer::getChunkSize()>& magnitude,
                                     std::array<double, RingBuffer::getChunkSize()>& phase,
                                     std::array<double, RingBuffer::getChunkSize()>& ringBuffer,
                                     FilterParameters& params) const
{
  const double deltaF = params.m_sampleRate / N; // Frequency resolution

  // Reconstruct the signal by summing custom sinusoidal components
  for (int k = 0; k <= N / 2; ++k)
  {
    double freq = k * deltaF; // Frequency of the k-th component
    for (int t = 0; t < N; ++t)
    {
      double time = static_cast<double>(t) / params.m_sampleRate;
      ringBuffer[t] += magnitude[k] * std::cos(2.0 * iplug::PI * freq * time + phase[k]);
    }
  }
  // Normalize the output by dividing by N
  for (int t = 0; t < N; ++t)
  {
    ringBuffer[t] *= scale;
  }
}
