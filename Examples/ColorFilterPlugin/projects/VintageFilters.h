#pragma once
#include "FilterParameters.h"
#include "Filters.h"

class VintageProcessor
{
protected:
  std::vector<int> m_LUT{}; // Lookup table for alpha values
  std::vector<int> m_LUT_cutoff{};
  std::vector<int> m_LUT_resonance{};
  int m_scale{1 << VINTAGE_BIT_RATE}; // Scaling factor for fixed-point
  int m_cutoffIndex{};
  int m_resonanceIndex{};
  int m_state{0};
  double m_sampleRate{};

public:
  virtual ~VintageProcessor() {}
  void InitLUT_DF1();
  void InitLUT_SVF1();
  void MapLUTCutoff(FilterParameters& params);
  void SetSampleRate_DF1(FilterParameters& params);
  void SetSampleRate_SVF1(FilterParameters& params);
};

class DF1_1P_LP_Vintage : public Filters, public VintageProcessor
{
private:
public:
  DF1_1P_LP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
    InitLUT_DF1();
  }

  void Process(double& input, FilterParameters& params) override;
};

class DF1_2P_LP_Vintage : public Filters, public VintageProcessor
{
private:
  int m_state[2]{};
  Sigmoidal sig{};

public:
  DF1_2P_LP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
    InitLUT_DF1();
  }

  void Process(double& input, FilterParameters& params) override;
};

class DF1_3P_LP_Vintage : public Filters, public VintageProcessor
{
private:
  int m_state[3]{};
  Sigmoidal sig{};

public:
  DF1_3P_LP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
    InitLUT_DF1();
  }

  void Process(double& input, FilterParameters& params) override;
};

class DF1_4P_LP_Vintage : public Filters, public VintageProcessor
{
private:
  int m_state[4]{};
  Sigmoidal sig{};

public:
  DF1_4P_LP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
    InitLUT_DF1();
  }

  void Process(double& input, FilterParameters& params) override;
};

class DF1_6P_LP_Vintage : public Filters, public VintageProcessor
{
private:
  int m_state[6]{};
  Sigmoidal sig{};

public:
  DF1_6P_LP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
    InitLUT_DF1();
  }

  void Process(double& input, FilterParameters& params) override;
};

class DF2_2P_LP_Vintage : public Filters, public VintageProcessor

{
private:
  Sigmoidal sigmoidalShaper{};
  int m_state[2]{};

public:
  DF2_2P_LP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
  }

  void Process(double& input, FilterParameters& params) override;
};

class DF2_4P_LP_Vintage : public Filters, public VintageProcessor

{
private:
  Sigmoidal sigmoidalShaper{};
  int m_state[4]{};

public:
  DF2_4P_LP_Vintage()
    : Filters{}
    , VintageProcessor{}
  {
  }

  void Process(double& input, FilterParameters& params) override;
};

class SVF1_2P_LP_Vintage : public Filters, public VintageProcessor
{
public:
  SVF1_2P_LP_Vintage()
    : Filters{}
  {
    InitLUT_SVF1();
  }

private:
  Sigmoidal sigmoidalShaper{};
  int m_state[2]{};

public:
  void Process(double& input, FilterParameters& params) override;
};

class SVF1_4P_LP_Vintage : public Filters, public VintageProcessor
{
public:
  SVF1_4P_LP_Vintage()
    : Filters{}
  {
    InitLUT_SVF1();
  }

private:
  Sigmoidal sigmoidalShaper{};
  int m_state[4]{};

public:
  void Process(double& input, FilterParameters& params) override;
};

class SVF1_6P_LP_Vintage : public Filters, public VintageProcessor
{
public:
  SVF1_6P_LP_Vintage()
    : Filters{}
  {
    InitLUT_SVF1();
  }

private:
  Sigmoidal sigmoidalShaper{};
  int m_state[6]{};

public:
  void Process(double& input, FilterParameters& params) override;
};