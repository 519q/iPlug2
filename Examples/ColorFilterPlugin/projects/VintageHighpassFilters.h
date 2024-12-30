#pragma once
#include "HighPassFilters.h"
#include "VintageFilters.h"

class DF1_1P_HP_Vintage : public Filters, public VintageProcessor
{
private:
  int m_alpha{};
  double m_state{};

public:
  DF1_1P_HP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_2P_HP_Vintage : public Filters, public VintageProcessor
{
  Sigmoidal sigmoidalShaper{};
  int m_state[2]{};
  int bandpass{};

public:
  DF1_2P_HP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_3P_HP_Vintage : public Filters, public VintageProcessor
{
  Sigmoidal sigmoidalShaper{};
  int m_state[3]{};
  int bandpass{};

public:
  DF1_3P_HP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_4P_HP_Vintage : public Filters, public VintageProcessor
{
  Sigmoidal sigmoidalShaper{};
  int m_state[4]{};
  int bandpass{};

public:
  DF1_4P_HP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_6P_HP_Vintage : public Filters, public VintageProcessor
{
  Sigmoidal sigmoidalShaper{};
  int m_state[6]{};
  int bandpass{};

public:
  DF1_6P_HP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF2_2P_HP_Vintage : public Filters, public VintageProcessor

{
private:
  Sigmoidal sigmoidalShaper{};
  int m_state[2]{};

public:
  DF2_2P_HP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
  }

  void Process(double& input, FilterParameters& params) override;
};

class DF2_4P_HP_Vintage : public Filters, public VintageProcessor

{
private:
  Sigmoidal sigmoidalShaper{};
  int m_state[4]{};

public:
  DF2_4P_HP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
  }

  void Process(double& input, FilterParameters& params) override;
};

class SVF1_2P_HP_Vintage : public Filters, public VintageProcessor
{
public:
  SVF1_2P_HP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
  }

private:
  Sigmoidal sigmoidalShaper{};
  int m_state[2]{};

public:
  void Process(double& input, FilterParameters& params) override;
};

class SVF1_4P_HP_Vintage : public Filters, public VintageProcessor
{
public:
  SVF1_4P_HP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
  }

private:
  Sigmoidal sigmoidalShaper{};
  int m_state[4]{};

public:
  void Process(double& input, FilterParameters& params) override;
};
