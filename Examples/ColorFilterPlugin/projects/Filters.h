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
constexpr int LUT_SIZE{1 << 7};
constexpr int VINTAGE_BIT_RATE{15};

enum class FilterTypes
{
  DF1_1P_LP,
  DF1_1P_LP_Vintage,
  //DF1_1P_HP,
  DF1_2P_LP,
  //DF1_2P_HP,
  DF1_3P_LP,
  DF1_4P_LP,
  DF1_6P_LP,
  DF2_2P_LP,
  DF2_4P_LP,
  SVF1_4P_LP,
  MAX_FILTER_TYPES
};

class Filters
{
public:
  virtual ~Filters() {}
  virtual void Process(double& input, FilterParameters& params) = 0; // Pure virtual function
};

class DF1_1P_LP : public Filters
{
private:
  double m_state{};
  double m_alpha{};

public:
  DF1_1P_LP()
    : Filters{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_1P_LP_Vintage : public Filters
{
private:
  int m_LUT[LUT_SIZE]{};              // Lookup table for alpha values
  int m_scale{1 << VINTAGE_BIT_RATE}; // Scaling factor for fixed-point
  int m_cutoffIndex{};  
  int m_state{0};
  double m_sampleRate{};

public:
  DF1_1P_LP_Vintage()
    : Filters{}
  {
    InitLUT();
  }

  void MapLUTCutoff(FilterParameters& params);

  void InitLUT();

  void SetSampleRate(FilterParameters& params);


  void Process(double& input, FilterParameters& params) override;
};

class DF1_2P_LP : public Filters
{
private:
  Sigmoidal sigmoidalShaper{};
  double m_alpha{};
  double m_state[2];

public:
  DF1_2P_LP()
    : Filters{}
    , m_state{}
    , sigmoidalShaper{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_3P_LP : public Filters
{
private:
  Sigmoidal sigmoidalShaper{};
  double m_alpha{};
  double m_state[3];

public:
  DF1_3P_LP()
    : Filters{}
    , m_state{}
    , sigmoidalShaper{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_4P_LP : public Filters
{
private:
  Sigmoidal sigmoidalShaper{};
  double m_alpha{};
  double m_state[4];

public:
  DF1_4P_LP()
    : Filters{}
    , m_state{}
    , sigmoidalShaper{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_6P_LP : public Filters
{
private:
  Sigmoidal sigmoidalShaper{};
  double m_alpha{};
  double m_state[6];

public:
  DF1_6P_LP()
    : Filters{}
    , m_state{}
    , sigmoidalShaper{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF2_2P_LP : public Filters
{
private:
  Sigmoidal sigmoidalShaper{};
  double m_state[2]{};

public:
  DF2_2P_LP()
    : Filters{}
  {
  }

  void Process(double& input, FilterParameters& params) override;
};

class DF2_4P_LP : public Filters
{
private:
  Sigmoidal sigmoidalShaper{};
  double m_state[4]{};

public:
  DF2_4P_LP()
    : Filters{}
  {
  }

  void Process(double& input, FilterParameters& params) override;
};

class SVF1_4P_LP : public Filters
{
public:
  SVF1_4P_LP()
    : Filters{}
  {
  }

private:
  Sigmoidal sigmoidalShaper{};
  double m_state[4]{};
public:
  void Process(double& input, FilterParameters& params) override;
};
