#pragma once
#include "FilterParameters.h"
#include "IPlugConstants.h"
#include "Shapers.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <memory>
// #include "Oversampler.h"
const double antiDenormal = 1e-20;
constexpr double clampValue{5};


class Filters
{
protected:
  double m_cutoff{};
  double m_reso{};
  double m_sampleRate{};

public:
  Filters()
  {
  }
  virtual ~Filters() = default;
  bool isDirty(FilterParameters& params);
  double getCutoffFreq(FilterParameters& params);
  double getOmega(FilterParameters& params);

  double resonate(double input, FilterParameters& params, double bpPole1, double minusbpPole2, double resoScl);

  virtual void Process(double& input, FilterParameters& params) = 0; // Pure virtual function
};

class DF1_1P_LP : public Filters
{
protected:
  double m_state{};
  double m_alpha{};

public:
  DF1_1P_LP()
    : Filters{}
  {
  }
  void processPoleLP(double& input, double& state, FilterParameters& params) const;
  void getCoefficients(FilterParameters& params, double cutoffOffset = 0.25);
  void Process(double& input, FilterParameters& params) override;
};

class DF1_2P_LP : public DF1_1P_LP
{
private:
  double m_poles[2];

public:
  DF1_2P_LP()
    : DF1_1P_LP{}
    , m_poles{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_3P_LP : public DF1_1P_LP
{
private:
  double m_poles[3];

public:
  DF1_3P_LP()
    : DF1_1P_LP{}
    , m_poles{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_4P_LP : public DF1_1P_LP
{
private:
  double m_poles[4];

public:
  DF1_4P_LP()
    : DF1_1P_LP{}
    , m_poles{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_6P_LP : public DF1_1P_LP
{
private:
  double m_poles[6];

public:
  DF1_6P_LP()
    : DF1_1P_LP{}
    , m_poles{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF2_2P_LP : public Filters
{
private:
  double m_state[2]{};

protected:
  double b0{};
  double b1{};
  double b2{};
  double a1{};
  double a2{};
  double feedback_amount{};

public:
  DF2_2P_LP()
    : Filters{}
  {
  }
  void getCoefficients(FilterParameters& params, double resoScaling, double cutoffOffset = 0.25, double offset = 0, bool isHP = false);

  template <typename T>
  void process2pole(T& input, FilterParameters& params, T& state1, T& state2) const
  {
    double w = input - a1 * state1 - a2 * state2 + antiDenormal;
    input = (b0 * w + b1 * state1 + b2 * state2) * mapRange((1. - params.m_resonance), 0.5, 1);
    state2 = state1;
    state1 = w;
  }

  void Process(double& input, FilterParameters& params) override;
};

class DF2_4P_LP : public DF2_2P_LP
{
private:
  double m_state[4]{};

public:
  DF2_4P_LP()
    : DF2_2P_LP{}
  {
  }

  void Process(double& input, FilterParameters& params) override;
};

class SVF1_2P_LP : public Filters
{
public:
  SVF1_2P_LP()
    : Filters{}
  {
  }

private:
  double m_poles[2]{};

protected:
  double f{};

public:
  template <typename T>
  void processPoleLP(T& input, T& state, FilterParameters& params, bool isHP = false) const
  {
    state = state + f * (input - state);
    if (!isHP)
    {
      input = state;
    }
    else
    {
      input -= state;
    }
  }
  void getCoefficients(FilterParameters& params, double cutoffOffset = 0.25, double offset = 0);
  void Process(double& input, FilterParameters& params) override;
};

class SVF1_4P_LP : public SVF1_2P_LP
{
public:
  SVF1_4P_LP()
    : SVF1_2P_LP{}
  {
  }

private:
  double m_poles[4]{};

public:
  void Process(double& input, FilterParameters& params) override;
};

class SVF1_6P_LP : public SVF1_2P_LP
{
public:
  SVF1_6P_LP()
    : SVF1_2P_LP{}
  {
  }

private:
  double m_poles[6]{};

public:
  void Process(double& input, FilterParameters& params) override;
};

class ZDF1_2P_LP : public Filters
{
  // y[n] = b 0⋅x[n] + b 1⋅x[n−1] + b 2⋅x[n−2]−a 1⋅y[n−1]−a 2⋅y[n−2]
private:
  const double cutoffOffset{0.25};
  // Initialize past input and output values
  double x_prev1{}; // x[n - 1]
  double x_prev2{}; // x[n - 2]
  double y_prev1{}; // y[n - 1]
  double y_prev2{}; // y[n - 2]
public:
  ZDF1_2P_LP()
    : Filters{}
  {
  }
  void Process(double& input, FilterParameters& params) override
  {
    double resoScaling = 15;
    double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset)) * params.m_cutoff);
    const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
    // Intermediate variables
    double omega = 2 * iplug::PI * cutoffFreq / params.m_sampleRate;                                       // Normalized frequency
    double alpha = std::sin(omega) / (2 * std::max(params.m_resonance * resoScaling, 0.5) + antiDenormal); // Resonance factor
    double a0 = 1 + alpha;
    double b0 = ((1 - std::cos(omega)) / 2) / a0;
    double b1 = (1 - std::cos(omega)) / a0;
    double b2 = ((1 - std::cos(omega)) / 2) / a0;
    double a1 = (-2 * std::cos(omega)) / a0;
    double a2 = (1 - alpha) / a0;


    // Compute the current output
    double output_sample = (b0 * input + b1 * x_prev1 + b2 * x_prev2 - a1 * y_prev1 - a2 * y_prev2);

    // Update past values for the next iteration
    x_prev2 = x_prev1;
    x_prev1 = input;
    y_prev2 = y_prev1;
    y_prev1 = output_sample;

    input = output_sample;
  }
};

class ZDF2_1P_LP : public Filters
{
  //  Your filter is a 1-pole low-pass filter with the following characteristics:
  //
  // Transfer Function:
  //
  // H(z) = g / (1 + g - g * z^(-1))
  // where g = tan(π * cutoffFreq / sampleRate).
  //
  // Pole: One pole at z = 1 / (1 + g).
  //
  // Zero: No zeros (the numerator is a constant).
  //
  // This filter is computationally efficient and introduces no phase delay, making it suitable for real-time audio processing.
private:
  double state{};

public:
  ZDF2_1P_LP()
    : Filters{}
  {
  }
  void Process(double& input, FilterParameters& params) override
  {
    const double cutoffFreq = 20 * std::pow(1000.0, params.m_cutoff);
    double g = std::tan(iplug::PI * cutoffFreq / params.m_sampleRate); // Compute filter coefficient
    double v = (input - state) / (1 + g);                              // Intermediate variable
    double output = g * v + state;                                     // Compute output
    state = g * v + output;                                            // Update state
    input = output;
  }
};

class ZDF2_2P_LP : public Filters
{
private:
  double state[2]{};
  double state2{};
  double prev_v1 = 0.0; // Previous v1 value
  double prev_v2 = 0.0; // Previous v2 value
public:
  ZDF2_2P_LP()
    : Filters{}
  {
  }
  void Process(double& input, FilterParameters& params) override
  {
    ////double resoScaling = 2;
    ////double cutoffOffset = 0.25;
    ////double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset)) * params.m_cutoff);
    ////const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
    // const double cutoffFreq = 20.0 * std::pow(1000.0, params.m_cutoff);
    ////double wd = 2.0 * iplug::PI * cutoffFreq;
    ////double wa = 2.0 * params.m_sampleRate * std::tan(wd / (2.0 * params.m_sampleRate));
    ////double g = wa / (2.0 * params.m_sampleRate);
    // double resoScaling = 4.0;
    ////double k = 1.0 / (params.m_resonance * resoScaling + 0.5);
    // double g = std::tan(iplug::PI * cutoffFreq / params.m_sampleRate);
    // double k = 1.0 / (std::max(params.m_resonance * resoScaling, 0.5) + antiDenormal);
    ////  Improved resonance scaling
    //// Intermediate variables
    // double a0 = 1.0 + g * (g + k);
    // double v1 = (input - state1 - k * state2) / a0 + antiDenormal;
    ////double v1 = (input - (state1 + k * state2)) / (1 + g * (g + k)) + antiDenormal;
    // double v2 = state2 + g * v1;
    //// Compute output
    // double output = v2;
    //// Update state
    // state1 = v1;
    // state2 = v2;
    // input = output;
    double resoScaling = 2;
    const double cutoffFreq = 20 * std::pow(1000.0, params.m_cutoff);
    double g = std::tan(iplug::PI * cutoffFreq / params.m_sampleRate); // Compute filter coefficient
    input += (state[0] - state[1]) * (params.m_resonance * resoScaling);
    for (double& pole : state)
    {
      double v = (input - pole) / (1 + g); // Intermediate variable
      double output = g * v + pole;        // Compute output
      pole = g * v + output;               // Update state
      input = pole;
    }
  }
};