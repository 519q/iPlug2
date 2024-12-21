#pragma once
#include "FilterParameters.h"
#include "IPlugConstants.h"
#include "Shapers.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <memory>
// #include "Oversampler.h"

constexpr double clampValue{5};

constexpr int LUT_SIZE{1 << 7};
constexpr int VINTAGE_BIT_RATE{15};

enum class FilterTypes
{
  DF1_1P,
  DF1_1P_Vintage,
  DF1_2P,
  DF1_3P,
  DF1_4P,
  DF1_6P,
  DF2_2P,
  DF2_4P,
  SVF1_4P,
  MAX_FILTER_TYPES
};


class VintageFilter
{
protected:
  int m_LUT[LUT_SIZE]{};              // Lookup table for alpha values
  int m_scale{1 << VINTAGE_BIT_RATE}; // Scaling factor for fixed-point
  int m_cutoffIndex{};                // Index for the LUT for current cutoff frequency
};

class Filters
{
protected:
  double m_sampleRate{44100.};
  double m_cutoff{1.};


public:
  virtual ~Filters() {}
  virtual void Process(double& input, FilterParameters& params) = 0; // Pure virtual function
};

class DF1_1P : public Filters
{
private:
  double m_state{};
  double m_alpha{};

public:
  DF1_1P()
    : Filters{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_1P_Vintage : public DF1_1P, public VintageFilter
{
private:
  int m_state{0};

public:
  DF1_1P_Vintage()
    : DF1_1P{}
    , VintageFilter{}
  {
    InitLUT();
  }

  void MapLUTCutoff(double knobValue);

  void InitLUT();

  void SetSampleRate(double sampleRate);


  void Process(double& input, FilterParameters& params) override;
};

class DF1_2P : public DF1_1P
{
private:
  double resoScaling{2.};
  Sigmoidal sigmoidalShaper{};
  double m_resonance{};
  double m_alpha{};
  double m_state[2];

public:
  DF1_2P()
    : DF1_1P{}
    , m_state{}
    , sigmoidalShaper{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_3P : public DF1_1P
{
private:
  double resoScaling{1.2};
  Sigmoidal sigmoidalShaper{};
  double m_resonance{};
  double m_alpha{};
  double m_state[3];

public:
  DF1_3P()
    : DF1_1P{}
    , m_state{}
    , sigmoidalShaper{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_4P : public DF1_1P
{
private:
  double resoScaling{1.44};
  Sigmoidal sigmoidalShaper{};
  double m_resonance{};
  double m_alpha{};
  double m_state[4];

public:
  DF1_4P()
    : DF1_1P{}
    , m_state{}
    , sigmoidalShaper{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_6P : public DF1_1P
{
private:
  double resoScaling{0.85};
  Sigmoidal sigmoidalShaper{};
  double m_resonance{};
  double m_alpha{};
  double m_state[6];

public:
  DF1_6P()
    : DF1_1P{}
    , m_state{}
    , sigmoidalShaper{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF2_2P : public Filters
{
private:
  Sigmoidal sigmoidalShaper{};
  double resoScaling{12};
  double m_state[2]{};
  double a1{};
  double a2{};
  double b0{};
  double b1{};
  double b2{};

public:
  DF2_2P()
    : Filters{}
  {
  }

  void Process(double& input, FilterParameters& params) override;
};

class DF2_4P : public Filters
{
private:
  Sigmoidal sigmoidalShaper{};
  double resoScaling{6};
  double m_state[4]{};
  double a1{};
  double a2{};
  double b0{};
  double b1{};
  double b2{};

public:
  DF2_4P()
    : Filters{}
  {
  }

  void Process(double& input, FilterParameters& params) override;
};

class SVF1_4P : public Filters
{
public:
  SVF1_4P()
    : Filters{}
  {
  }

private:
  double resoScaling{1.55};
  Sigmoidal sigmoidalShaper{};
  double m_state[4]{};
  double m_resonance{0.0};
  double m_feedback{0.0};

public:
  void Process(double& input, FilterParameters& params) override;
};
