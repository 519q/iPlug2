#pragma once
#include "IPlug_include_in_plug_hdr.h"
#include "projects/DebugPrint.h"
#include "projects/SmoothTools.h"
#include "projects/StateMixer.h"
#include <array>
#include <vector>
struct IntPlusInterpParam
{
  int stepped;
  double interpFactor;
};
inline IntPlusInterpParam mapParamToOrder(double controlValue, int size, int base, int step, int offset = 0)
{
  double overFractional = controlValue * size - base;
  int overInt = std::floor(overFractional);
  int steppedOver = overInt - (overInt % step) + offset;
  double interpFactor = overFractional - overInt;
  return {std::clamp((base + steppedOver), base, size), interpFactor};
}
struct Real_Imag_Output
{
  double real;
  double imag;
};

struct Magn_Phas_Output
{
  double magnitude;
  double phase;
};

static constexpr int IIR_MAX_ORDER = 40;
class IIR_HilbertTransform
{
private:
  std::array<double, IIR_MAX_ORDER> x1, x2, y1, y2;
  std::array<std::vector<double>, IIR_MAX_ORDER> IIRPrecomputedCoeffs;
  std::array<double, IIR_MAX_ORDER> iirDelayLine;
  std::array<double, IIR_MAX_ORDER> totalDelayArr;
  size_t delayPos;

public:
  IIR_HilbertTransform();
  void PrecomputeCoefficients();
  Magn_Phas_Output getMagnitude_Phase(double input, int order);
  Real_Imag_Output getReal_Imag(double input, int order);
  double getImaginary(double input, int order);
  void initializeDelayLine();
  std::vector<double>& GetCoefficients(int order);
};

static constexpr int FIR_MAX_ORDER = 200;
class FIR_HilbertTransform
{
  std::array<double, FIR_MAX_ORDER> firDelayLine;
  size_t position = 0;
  std::array<std::vector<double>, FIR_MAX_ORDER> FIRPrecomputedCoeffs;

public:
  FIR_HilbertTransform();
  double getImaginary(double input, int order);
  std::vector<double>& GetCoefficients(int order);
  void PrecomputeCoefficients();
  double getMagnitude(double input, int order);
  double getPhase(double input, int order);
  Magn_Phas_Output getMagnitude_Phase(double input, int order);
  double getReal(double input, int order);
};

static constexpr int LATTICE_MAX_ORDER = 200;
class LatticeHilbertTransform
{
private:
  std::array<double, LATTICE_MAX_ORDER> zReal{}, zImag{}; // Delay buffers for two parallel allpass filters
  std::array<std::vector<double>, LATTICE_MAX_ORDER> latticePrecomputedCoeffs;
  int m_size{};

public:
  LatticeHilbertTransform();
  std::vector<double>& GetCoefficients(int order);
  Real_Imag_Output getRealImag(double input, int order, std::vector<double>& coeffs);
  Magn_Phas_Output getMagnitude_Phase(double input, int order);
  void PrecomputeCoefficients();
};

template <typename T>
class HilbertMixer
{
private:
  T ceil{};
  T floor{};
  int m_size{};

public:
  Magn_Phas_Output process(double input, double sizeControl, int size, int base, int step, std::function<Magn_Phas_Output(T&, double, int)> func, int offset = 0)
  {
    auto paramStruct = mapParamToOrder(sizeControl, size, base, step, offset);
    double interpFactor = paramStruct.interpFactor;
    int floorSize = paramStruct.stepped;


    m_size = floorSize;
    int ceilSize = std::clamp((paramStruct.stepped + step), paramStruct.stepped, size);
    Magn_Phas_Output floorProcessed = func(floor, input, floorSize);
    Magn_Phas_Output ceilProcessed = func(ceil, input, ceilSize);
    Magn_Phas_Output outputStruct{};
    outputStruct.magnitude = interpolateLin(ceilProcessed.magnitude, floorProcessed.magnitude, interpFactor);
    outputStruct.phase = interpolateLin(ceilProcessed.phase, floorProcessed.phase, interpFactor);
    return outputStruct;
  }
};

class Hilbert_Transformer
{
private:
  HilbertMixer<IIR_HilbertTransform> IIRmixer{};
  HilbertMixer<FIR_HilbertTransform> FIRmixer{};
  HilbertMixer<LatticeHilbertTransform> LATTICEmixer{};

public:
  Magn_Phas_Output ProcessIIR(double input, double controlParam);
  Magn_Phas_Output ProcessFIR(double input, double controlParam, int offset);
  Magn_Phas_Output ProcessLATTICE(double input, double controlParam);
};
