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

enum class FilterTypes
{
  DF1_1P,
  DF1_2P,
  DF1_3P,
  DF1_4P,
  DF1_6P,
  DF2_2P,
  DF2_4P,
  SVF1_2P,
  SVF1_4P,
  SVF1_6P,
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

class SVF1_2P_LP : public Filters
{
public:
  SVF1_2P_LP()
    : Filters{}
  {
  }

private:
  Sigmoidal sigmoidalShaper{};
  double m_state[2]{};

public:
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

class SVF1_6P_LP : public Filters
{
public:
  SVF1_6P_LP()
    : Filters{}
  {
  }

private:
  Sigmoidal sigmoidalShaper{};
  double m_state[6]{};

public:
  void Process(double& input, FilterParameters& params) override;
};
