//#pragma once
////#include "projects/RingBuffer.h"
//#include "FilterParameters.h"
//#include "projects/FFT_F_I_SIMD.h"
//
////#include "DebugPrint.h"
//#include <complex>
//#include <map>
//#include <vector>
//
//class FFT_F_I
//{
//private:
//  const int N = RingBuffer::getChunkSize();
//  const double scale = 1.0 / N;
//  std::vector<std::complex<double>> fft_in_data = std::vector<std::complex<double>>(N);
//
//public:
//
//  // Process the buffer and return the reconstructed signal
//  void ProcessBuffer(std::array<double, RingBuffer::getChunkSize()>& ChunkBuffer, bool chunkRead, FilterParameters& params);
//  // Compute the analytic signal using FFT
//  std::vector<std::complex<double>> computeAnalyticSignal(const std::array<double, RingBuffer::getChunkSize()>& ringBuffer);
//
//  void extractMagnitudeAndPhase(const std::vector<std::complex<double>>& analyticSignal,
//                                std::array<double, RingBuffer::getChunkSize()>& magnitude,
//                                std::array<double, RingBuffer::getChunkSize()>& phase) const;
//
//  void fwd_fft(std::vector<std::complex<double>>& data);
//
//  void reconstructSignal(std::array<double, RingBuffer::getChunkSize()>& magnitude,
//                         std::array<double, RingBuffer::getChunkSize()>& phase,
//                         std::array<double, RingBuffer::getChunkSize()>& ringBuffer,
//                         FilterParameters& params) const;
//};
