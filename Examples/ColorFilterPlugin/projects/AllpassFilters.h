#pragma once
#include "IPlug_include_in_plug_hdr.h"
#include "projects/DebugPrint.h"
#include "projects/Filters.h"
#include <cmath>
#include <vector>

template <typename T>
class StateMixer
{
private:
  double m_previousFractionalSize{};
  double m_fractionalSize{};
  int m_ceil_size{};
  int m_floor_size{};
  int m_stepSize{};

  std::vector<T> m_floor_T;
  std::vector<T> m_ceil_T;

  int calculateStepSize(double velocity) const
  {
    // Define a mapping from velocity to step size
    if (velocity < 0.005)
      return m_stepSize;
    if (velocity < 0.01)
      return m_stepSize * 2;
    if (velocity < 0.05)
      return m_stepSize * 3;
    if (velocity < 0.1)
      return m_stepSize * 6;
    if (velocity < 0.2)
      return m_stepSize * 15;
    return m_stepSize * 30; // Default step size for high velocity
  }

public:
  StateMixer() {}
  void initializeMixer()
  {
    m_floor_size = 1;
    m_ceil_size = 2;

    m_floor_T.resize(m_floor_size, T());
    m_ceil_T.resize(m_ceil_size, T());
  }
  double Process(double input, FilterParameters& params, double sizeParam, int sizeMultiplier, int baseStepSize, std::function<void(T&, double&)> func)
  {
    m_stepSize = baseStepSize;
    // Calculate velocity-dependent step size
    double velocity = std::abs(m_fractionalSize - m_previousFractionalSize);
    int stepSize = calculateStepSize(velocity);
    m_previousFractionalSize = m_fractionalSize;
    // Calculate the fractional pole count
    m_fractionalSize = sizeParam * sizeMultiplier;                  // Scale control to [0, 200]
    int floorSize = static_cast<int>(std::floor(m_fractionalSize)); // Integer part (floor)
    int ceilSize = std::min(sizeMultiplier, floorSize + stepSize);  // Next integer part (ceiling)
    double interpFactor = m_fractionalSize - floorSize;             // Fractional part for interpolation
    // Ensure the allpass arrays are resized as needed
    if (floorSize != m_floor_size || ceilSize != m_ceil_size)
    {
      if (floorSize > m_floor_size)
      {
        if (!m_ceil_T.empty())
        {
          std::swap(m_floor_T, m_ceil_T); // Safe swap instead of move
        }
        m_floor_size = floorSize;
        // Resize m_ceil_T to the new ceiling size
        m_ceil_T.resize(ceilSize, T());
        m_ceil_size = ceilSize;
      }
      else if (floorSize < m_floor_size)
      {
        // Store the old size for comparison
        int oldSize = m_floor_size;
        T lastT = m_floor_T.empty() ? T() : m_floor_T.back(); // Safe default value
        std::swap(m_ceil_T, m_floor_T); // Safe swap
        for (int i = oldSize; i < floorSize; ++i)
        {
          m_ceil_T[i] = lastT; // Ensure valid initialization
        }
        m_ceil_size = ceilSize;
        // Resize floor_allpass to the new floor size
        m_floor_T.resize(floorSize, T());

        m_floor_size = floorSize;
      }
    }
    // Process the input through the floor processors
    double processedFloor = input;
    for (auto& filter : m_floor_T)
    {
      func(filter, processedFloor);
    }

    // Process the input through the ceiling processors
    double processedCeil = input;
    for (auto& filter : m_ceil_T)
    {
      func(filter, processedCeil);
    }

    // Interpolate between the floor and ceiling outputs
    double processed = interpolateLin(processedFloor, processedCeil, interpFactor);

    return processed;
  }
};
class AllpassFilters : public Filters
{
  double m_freq{};
  int m_depth{};
  double m_sampleRate{};

protected:
  bool isDirty(FilterParameters& params) override
  {
    if ((m_freq != params.m_phaserFreq) || (m_depth != params.m_phaserDepth) || (m_sampleRate != params.m_sampleRate))
    {
      m_freq = params.m_phaserFreq;
      m_depth = params.m_phaserDepth;
      m_sampleRate = params.m_sampleRate;
      return true;
    }
    return false;
  }
  virtual double calculateCoefficients(FilterParameters& params) { return -.95 + (std::clamp(params.m_phaserFreq, 0., .99) * 1.95); }
};

class FirstOrderAllpassFilter : public AllpassFilters
{
private:
  double mDelay{};
  StateMixer<FirstOrderAllpassFilter> mixer{};

public:
  FirstOrderAllpassFilter()
    : AllpassFilters{}
  {
  }
  void ProcessOnePole(double& input, FilterParameters& params, double A)
  {
    double output = A * input + mDelay;
    mDelay = input - A * output;
    input = output;
  }

  void Process(double& input, FilterParameters& params) override
  {
    double output = mixer.Process(
      input, params, params.m_phaserDepth, 200, 1, [this, &params](FirstOrderAllpassFilter& filter, double& sample) { filter.ProcessOnePole(sample, params, calculateCoefficients(params)); });
    input = output;
  }
};

class SecondOrderAllpassFilterV1 : public AllpassFilters
{
private:
  double mDelay1{}, mDelay2{};
  StateMixer<SecondOrderAllpassFilterV1> mixer{};

public:
  SecondOrderAllpassFilterV1()
    : AllpassFilters{}
  {
  }
  void processTwoPole(double& input, double A)
  {
    double output = A * input + mDelay2;
    mDelay2 = mDelay1;
    mDelay1 = input - A * output;
    input = output;
  }

  void Process(double& input, FilterParameters& params) override
  {
    double output =
      mixer.Process(input, params, params.m_phaserDepth, 200, 1, [this, &params](SecondOrderAllpassFilterV1& filter, double& sample) { filter.processTwoPole(sample, calculateCoefficients(params)); });
    input = output;
  }
};

class SecondOrderAllpassFilterV2 : public AllpassFilters
{
private:
  double mDelay1{}, mDelay2{};
  StateMixer<SecondOrderAllpassFilterV2> mixer{};

public:
  SecondOrderAllpassFilterV2()
    : AllpassFilters{}
  {
  }

  void processTwoPole(double& input, double A)
  {
    double output = -A * input + mDelay2;
    mDelay2 = mDelay1;
    mDelay1 = input + A * output; // Corrected feedback calculation
    input = output;
  }

  void Process(double& input, FilterParameters& params) override
  {
    double output =
      mixer.Process(input, params, params.m_phaserDepth, 200, 1, [this, &params](SecondOrderAllpassFilterV2& filter, double& sample) { filter.processTwoPole(sample, calculateCoefficients(params)); });
    input = output;
  }
};

class LatticeAllpass : public AllpassFilters
{
private:
  double z1{}; // Delay element
  double z2{}; // Delay element
  StateMixer<LatticeAllpass> mixer{};

public:
  double getCoeff(FilterParameters& params)
  {
    if (isDirty(params))
    {
      // Convert cutoff to normalized frequency (0 to 0.5)
      double omega = 2.0 * iplug::PI * (getCutoffFreq(params, params.m_phaserFreq) / params.m_sampleRate);

      // Compute reflection coefficient (approximated for smoothness)
      return (sin(omega) - 1.0) / (sin(omega) + 1.0);
    }
  }
  void processOnePole(double& input, double k)
  {
    double v = input - k * z1;
    double outSample = k * v + z1;

    z1 = v; // Delay update
    input = outSample;
  }
  void Process(double& input, FilterParameters& params) override
  {

    double output = mixer.Process(input, params, params.m_phaserDepth, 50, 1, [this, &params](LatticeAllpass& filter, double& sample) { filter.processOnePole(sample, getCoeff(params)); });
    input = output;
  }

};

// class PhaseVocoderAllpassFilter
//{
// public:
//   PhaseVocoderAllpassFilter(double phaseShift)
//     : mPhaseShift(phaseShift)
//     , mDelay(0.0)
//   {
//   }
//
//   double process(double input)
//   {
//     double output = input * std::cos(mPhaseShift) + mDelay * std::sin(mPhaseShift);
//     mDelay = input * std::sin(mPhaseShift) - mDelay * std::cos(mPhaseShift);
//     return output;
//   }
//
// private:
//   double mPhaseShift;
//   double mDelay;
// };
