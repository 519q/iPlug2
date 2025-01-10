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
  double m_sampleRate{};

public:
  virtual ~VintageProcessor() {}
  void InitLUT_DF1();
  void InitLUT_SVF1();
  void MapLUTCutoff(FilterParameters& params);
  void SetSampleRate_DF1(FilterParameters& params);
  void SetSampleRate_SVF1(FilterParameters& params);
};

class DF1_1P_LP_Vintage : public VintageProcessor, public Filters
{
private:
  int m_state{0};

protected:
  double m_alpha{};

public:
  DF1_1P_LP_Vintage()
    : VintageProcessor{}
  {
    InitLUT_DF1();
  }
  void getAlpha(FilterParameters& params);
  void processPoleLP(int& input, int& state, FilterParameters& params) const;
  void Process(double& input, FilterParameters& params) override;
};

class DF1_2P_LP_Vintage : public DF1_1P_LP_Vintage
{
private:
  int m_poles[2]{};

public:
  DF1_2P_LP_Vintage()
    : DF1_1P_LP_Vintage{}
  {
    InitLUT_DF1();
  }

  void Process(double& input, FilterParameters& params) override;
};

class DF1_3P_LP_Vintage : public DF1_1P_LP_Vintage
{
private:
  int m_poles[3]{};

public:
  DF1_3P_LP_Vintage()
    : DF1_1P_LP_Vintage{}
  {
    InitLUT_DF1();
  }

  void Process(double& input, FilterParameters& params) override;
};

class DF1_4P_LP_Vintage : public DF1_1P_LP_Vintage
{
private:
  int m_poles[4]{};

public:
  DF1_4P_LP_Vintage()
    : DF1_1P_LP_Vintage{}
  {
    InitLUT_DF1();
  }

  void Process(double& input, FilterParameters& params) override;
};

class DF1_6P_LP_Vintage : public DF1_1P_LP_Vintage
{
private:
  int m_poles[6]{};

public:
  DF1_6P_LP_Vintage()
    : DF1_1P_LP_Vintage{}
  {
    InitLUT_DF1();
  }

  void Process(double& input, FilterParameters& params) override;
};

class DF2_2P_LP_Vintage : public DF2_2P_LP, public VintageProcessor

{
private:
  int m_poles[2]{};

public:
  DF2_2P_LP_Vintage()
    : DF2_2P_LP{}
    , VintageProcessor{}
  {
  }
  void addLeakage();
  void addTolerances(FilterParameters& params);
  void Process(double& input, FilterParameters& params) override;
};

class DF2_4P_LP_Vintage : public DF2_2P_LP_Vintage

{
private:
  int m_poles[4]{};

public:
  DF2_4P_LP_Vintage()
    : DF2_2P_LP_Vintage{}
  {
  }

  void Process(double& input, FilterParameters& params) override;
};

class SVF1_2P_LP_Vintage : public SVF1_2P_LP, public VintageProcessor
{
public:
  SVF1_2P_LP_Vintage()
    : SVF1_2P_LP{}
  {
    InitLUT_SVF1();
  }

private:
  int m_poles[2]{};

public:
  void getCoefficients(FilterParameters& params, double cutoffOffset = 0.25, double offset = 0.);
  void Process(double& input, FilterParameters& params) override;
};

class SVF1_4P_LP_Vintage : public SVF1_2P_LP_Vintage
{
public:
  SVF1_4P_LP_Vintage()
    : SVF1_2P_LP_Vintage{}
  {
    InitLUT_SVF1();
  }

private:
  int m_poles[4]{};

public:
  void Process(double& input, FilterParameters& params) override;
};

class SVF1_6P_LP_Vintage : public SVF1_2P_LP_Vintage
{
public:
  SVF1_6P_LP_Vintage()
    : SVF1_2P_LP_Vintage{}
  {
    InitLUT_SVF1();
  }

private:
  int m_poles[6]{};

public:
  void Process(double& input, FilterParameters& params) override;
};