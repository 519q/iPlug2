#pragma once
#include "IPlug_include_in_plug_hdr.h"
#include "projects/DebugPrint.h"
#include "projects/Filters.h"
#include "projects/StateMixer.h"

class AllpassFilters : public Filters
{
  double m_freq{};
  int m_depth{};
  double m_sampleRate{};

protected:
  bool isDirty(FilterParameters& params) override;
  virtual double calculateCoefficients(FilterParameters& params);
};
class FirstOrderAllpassOnePole
{
private:
  double mDelay{};

public:
  void ProcessOnePole(double& input, FilterParameters& params, double A);
};
class FirstOrderAllpassFilter : public AllpassFilters
  {
private:
    StateMixer<FirstOrderAllpassOnePole> mixer{200};

public:
  FirstOrderAllpassFilter()
    : AllpassFilters{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
  };
class SecondOrderAllpassTwoPole
{
private:
  double mDelay1{}, mDelay2{};
public:
  void processTwoPole(double& input, double A);
};
class SecondOrderAllpassFilter : public AllpassFilters
  {
private:
    StateMixer<SecondOrderAllpassTwoPole> mixer{200};

public:
  SecondOrderAllpassFilter()
    : AllpassFilters{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
  };

class LatticeOnePole
{
private:
  double z1{}; // Delay element
  double z2{}; // Delay element
public:
  void processOnePole(double& input, double k);
};
class LatticeAllpass : public AllpassFilters
{
private:
  StateMixer<LatticeOnePole> mixer{50};
  double m_k{};

public:
  LatticeAllpass()
    : AllpassFilters{}
  {
  }
  void getCoeff(FilterParameters& params);
  void Process(double& input, FilterParameters& params) override;
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
