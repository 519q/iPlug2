#pragma once
#define MY_FFT
 //#define KISS_FFT
 //#define PFFFT
 //#define INVERSE_FFT
#include "projects/RingBuffer.h"
#ifdef PFFFT
  #include "pffft.hpp"
#endif // PFFFT
#ifdef KISS_FFT
extern "C" {
  #include "kiss_fft.h"
  #include "kiss_fftr.h"
}
#endif // KISS_FFT
#include "FilterParameters.h"

#include "DebugLogger.h"
#include <complex>
#include <map>
#include <vector>
#ifdef PFFFT
using AlignedVectorFloat = pffft::AlignedVector<float>;
using AlignedVectorComplex = pffft::AlignedVector<std::complex<float>>;
#endif // PFFFT

class SpectralShaper
{
private:
  const int N = RingBuffer::getChunkSize();
  const double scale = 1.0 / N;
#ifdef MY_FFT
  std::vector<std::complex<double>> fft_in_data = std::vector<std::complex<double>>(N);
#endif // MY_FFT

#ifdef PFFFT
  pffft::Fft<float> fft;       // pffft FFT setup
  AlignedVectorFloat fftIn;    // Input buffer for FFT
  AlignedVectorComplex fftOut; // Output buffer for FFT
#endif                         // PFFFT

#ifdef KISS_FFT
                               // KissFFT configuration
  kiss_fftr_cfg fftConfig; // Configuration for real-to-complex FFT
  #ifdef INVERSE_FFT
  kiss_fftr_cfg ifftConfig;
  #endif                  // INVERSE_FFT
  kiss_fft_cpx* fftOut;   // Output buffer for FFT (complex)
  kiss_fft_scalar* fftIn; // Input buffer for FFT (real)
#endif                    // KISS_FFT

public:
  SpectralShaper()
#ifdef PFFFT
    : fft(RingBuffer::getChunkSize())
    , fftIn(RingBuffer::getChunkSize())
    , fftOut(RingBuffer::getChunkSize() / 2 + 1)
#endif // PFFFT
#ifdef KISS_FFT
    : fftConfig(kiss_fftr_alloc(RingBuffer::getChunkSize(), 0, nullptr, nullptr))
  #ifdef INVERSE_FFT
    , ifftConfig(kiss_fftr_alloc(RingBuffer::getChunkSize(), 1, nullptr, nullptr))
  #endif // INVERSE_FFT

    , fftOut(new kiss_fft_cpx[RingBuffer::getChunkSize() / 2 + 1])
    , fftIn(new kiss_fft_scalar[RingBuffer::getChunkSize()])
#endif // KISS_FFT
  {
  }

  ~SpectralShaper()
  {
#ifdef KISS_FFT
    // Free KissFFT resources
    kiss_fftr_free(fftConfig);
  #ifdef INVERSE_FFT
    kiss_fftr_free(ifftConfig);
  #endif // INVERSE_FFT
    delete[] fftOut;
    delete[] fftIn;
#endif // KISS_FFT
  }

  // Process the buffer and return the reconstructed signal
  void ProcessBuffer(std::array<double, RingBuffer::getChunkSize()>& ChunkBuffer, bool chunkRead, FilterParameters& params, double sampleRate = 44100);
  // Compute the analytic signal using FFT
  std::vector<std::complex<double>> computeAnalyticSignal(const std::array<double, RingBuffer::getChunkSize()>& ringBuffer);

  void extractMagnitudeAndPhase(const std::vector<std::complex<double>>& analyticSignal,
                                std::array<double, RingBuffer::getChunkSize()>& magnitude,
                                std::array<double, RingBuffer::getChunkSize()>& phase);

  void fwd_fft(std::vector<std::complex<double>>& data);

  void reconstructSignal(std::array<double, RingBuffer::getChunkSize()>& magnitude,
                         std::array<double, RingBuffer::getChunkSize()>& phase,
                         std::array<double, RingBuffer::getChunkSize()>& ringBuffer,
                         double sampleRate,
                         FilterParameters& params);

  double generateCleanCos(double freq, double time, double phase);

  double generateCustomSinusoid(double freq, double time, double phase, FilterParameters& params);

  double shapeSine(double sineValue, FilterParameters& params);
};
