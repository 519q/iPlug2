#pragma once
#include "FilterParameters.h"
#include "IPlugConstants.h"
#include "Shapers.h"
#include <algorithm>
#include <vector>
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
  double m_alpha{};
  std::vector<double> m_poles{};
  double ResoScaling{};

public:
  Filters() {}
  virtual ~Filters() = default;
  virtual bool isDirty(FilterParameters& params);
  double getCutoffFreq(FilterParameters& params, double cutoff);
  double getOmega(FilterParameters& params);

  double resonate(FilterParameters& params, double bpPole1, double minusbpPole2, double resoScl = 1);

  virtual void Process(double& input, FilterParameters& params) = 0; // Pure virtual function
};

class DF1_1P_LP : public Filters
{
public:
  DF1_1P_LP()
    : Filters{}
  {
    m_poles.resize(1, 0.0);
  }
  void processPole(double& input, double& state, FilterParameters& params, bool HP = 0) const;
  void getCoefficients(FilterParameters& params);
  virtual void Process(double& input, FilterParameters& params) override;
};

class DF1_2P_LP : public DF1_1P_LP
{
public:
  DF1_2P_LP()
    : DF1_1P_LP{}
  {
    m_poles.resize(2, 0.0);
    ResoScaling = DF1_2P_ResoScaling;
  }
};

class DF1_3P_LP : public DF1_1P_LP
{
public:
  DF1_3P_LP()
    : DF1_1P_LP{}
  {
    m_poles.resize(3, 0.0);
    ResoScaling = DF1_3P_ResoScaling;
  }
};

class DF1_4P_LP : public DF1_1P_LP
{
public:
  DF1_4P_LP()
    : DF1_1P_LP{}
  {
    m_poles.resize(4, 0.0);
    ResoScaling = DF1_4P_ResoScaling;
  }
};

class DF1_6P_LP : public DF1_1P_LP
{
public:
  DF1_6P_LP()
    : DF1_1P_LP{}
  {
    m_poles.resize(6, 0.0);
    ResoScaling = DF1_6P_ResoScaling;
  }
};

class DF2_2P_LP : public Filters
{
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
    m_poles.resize(2, 0.0);
    ResoScaling = DF2_2P_ResoScaling;
  }
  void getCoefficients(FilterParameters& params, double resoScaling, bool isHP = false);
  void process2pole(double& input, FilterParameters& params, double& state1, double& state2) const;
  virtual void Process(double& input, FilterParameters& params) override;
};

class DF2_4P_LP : public DF2_2P_LP
{
public:
  DF2_4P_LP()
    : DF2_2P_LP{}
  {
    m_poles.resize(4, 0.0);
    ResoScaling = DF2_4P_ResoScaling;
  }
};

class DF2_6P_LP : public DF2_2P_LP
{
public:
  DF2_6P_LP()
    : DF2_2P_LP{}
  {
    m_poles.resize(6, 0.0);
    ResoScaling = DF2_6P_ResoScaling;
  }
};

class SVF1_1P_LP : public Filters
{
public:
  SVF1_1P_LP()
    : Filters{}
  {
    m_poles.resize(1, 0.0);
  }

  void processPole(double& input, double& state, FilterParameters& params, bool isHP = false) const;
  void getCoefficients(FilterParameters& params);
  virtual void Process(double& input, FilterParameters& params) override;
};

class SVF1_2P_LP : public SVF1_1P_LP
{
public:
  SVF1_2P_LP()
    : SVF1_1P_LP{}
  {
    m_poles.resize(2, 0.0);
    ResoScaling = SVF1_2P_ResoScaling;
  }
};

class SVF1_3P_LP : public SVF1_1P_LP
{
public:
  SVF1_3P_LP()
    : SVF1_1P_LP{}
  {
    m_poles.resize(3, 0.0);
    ResoScaling = SVF1_3P_ResoScaling;
  }
};

class SVF1_4P_LP : public SVF1_1P_LP
{
public:
  SVF1_4P_LP()
    : SVF1_1P_LP{}
  {
    m_poles.resize(4, 0.0);
    ResoScaling = SVF1_4P_ResoScaling;
  }
};

class SVF1_6P_LP : public SVF1_1P_LP
{
public:
  SVF1_6P_LP()
    : SVF1_1P_LP{}
  {
    m_poles.resize(6, 0.0);
    ResoScaling = SVF1_6P_ResoScaling;
  }
};

class ZDF_1P_LP : public Filters
{

public:
  ZDF_1P_LP()
    : Filters{}
  {
    m_poles.resize(1, 0.0);
  }
  virtual void setCutoff(FilterParameters& params);
  void processOnePole(double& input, double& state, bool HP = false) const;
  virtual void Process(double& input, FilterParameters& params) override;
};

class ZDF_2P_LP : public ZDF_1P_LP
{
public:
  ZDF_2P_LP()
    : ZDF_1P_LP{}
  {
    m_poles.resize(2, 0.0);
    ResoScaling = ZDF1_2P_ResoScaling;
  }
};

class ZDF_3P_LP : public ZDF_1P_LP
{
public:
  ZDF_3P_LP()
    : ZDF_1P_LP{}
  {
    m_poles.resize(3, 0.0);
    ResoScaling = ZDF1_3P_ResoScaling;
  }
};

class ZDF_4P_LP : public ZDF_1P_LP
{
public:
  ZDF_4P_LP()
    : ZDF_1P_LP{}
  {
    m_poles.resize(4, 0.0);
    ResoScaling = ZDF1_4P_ResoScaling;
  }
};

class ZDF_6P_LP : public ZDF_1P_LP
{
public:
  ZDF_6P_LP()
    : ZDF_1P_LP{}
  {
    m_poles.resize(6, 0.0);
    ResoScaling = ZDF1_6P_ResoScaling;
  }
};

