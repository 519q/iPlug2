#pragma once
#include "Filters.h"

class DampFilter : public ZDF_1P_LP // 1pole zdf
{
public:
  DampFilter()
    : ZDF_1P_LP{}
  {
  }
  void setCutoff(FilterParameters& params) override
  {
    if (isDirty(params))
    {
      double cutoffFreq = getCutoffFreq(params, params.m_dampFilterCutoff);
      m_alpha = std::tan(iplug::PI * (cutoffFreq / params.m_sampleRate));
    }
  }
  bool isDirty(FilterParameters& params) override
  {
    if ((m_cutoff != params.m_dampFilterCutoff) || (m_sampleRate != params.m_sampleRate))
    {
      m_cutoff = params.m_dampFilterCutoff;
      m_sampleRate = params.m_sampleRate;
      return true;
    }
    return false;
  }
};

class DCBlock : public DampFilter // 1pole zdf
{
private:
  double mCutoff{};

public:
  DCBlock(double cutoffHz = 5)
    : DampFilter{}
    , mCutoff(cutoffHz)
  {
  }
  void setCutoff(FilterParameters& params) override
  {
    if (isDirty(params))
    {
      double cutoffFreq = mCutoff;
      m_alpha = std::tan(iplug::PI * (cutoffFreq / params.m_sampleRate));
    }
  }
  bool isDirty(FilterParameters& params) override
  {
    if (m_sampleRate != params.m_sampleRate)
    {
      m_sampleRate = params.m_sampleRate;
      return true;
    }
    return false;
  }
  void Process(double& input, FilterParameters& params) override
  {
    double proc = input;
    ZDF_1P_LP::Process(proc, params);
    input -= proc;
  }
};
