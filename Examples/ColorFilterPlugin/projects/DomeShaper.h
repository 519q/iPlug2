#pragma once
#include "projects/RingBuffer.h"
#include <complex>
#include <map>
#include <vector>

extern "C" {
#include "kiss_fft.h"
#include "kiss_fftr.h"
}

class DomeShaper
{
private:
  size_t bufferIndex;
  std::vector<double> CosineLUT{};
  const int lutResolution = 3600; // 0.1-degree increments
  kiss_fftr_cfg fftCfg;
  kiss_fft_scalar* fftIn;
  kiss_fft_cpx* fftOut;

public:
  DomeShaper()
  {
    CosineLUT = generateCosineLUT();
    fftCfg = kiss_fftr_alloc(RingBuffer::getChunkSize(), 0, NULL, NULL);
    fftIn = new kiss_fft_scalar[RingBuffer::getChunkSize()];
    fftOut = new kiss_fft_cpx[RingBuffer::getChunkSize() / 2 + 1];
  }

  ~DomeShaper()
  {
    // Free FFT resources
    kiss_fftr_free(fftCfg);
    delete[] fftIn;
    delete[] fftOut;
  }

  // Process the buffer and return the reconstructed signal
  void ProcessBuffer(std::array<double, RingBuffer::getChunkSize()>& ChunkBuffer);
  // Compute the analytic signal using FFT
  std::vector<std::complex<double>> computeAnalyticSignal(const std::array<double, RingBuffer::getChunkSize()>& ringBuffer);

  void extractMagnitudeAndPhase(const std::vector<std::complex<double>>& analyticSignal,
                                std::array<double, RingBuffer::getChunkSize()>& magnitude,
                                std::array<double, RingBuffer::getChunkSize()>& phase);

  // Generate a sine LUT
  std::vector<double> generateCosineLUT();

  // Get sine value from LUT
  double getCosineFromLUT(double phase) const;

  void reconstructSignal(std::array<double, RingBuffer::getChunkSize()>& magnitude,
                         std::array<double, RingBuffer::getChunkSize()>& phase,
                         std::array<double, RingBuffer::getChunkSize()>& ringBuffer) const;
};
