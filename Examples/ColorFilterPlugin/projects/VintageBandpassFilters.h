#pragma once
#pragma once
#include "VintageFilters.h"
#include "VintageHighPassFilters.h"


class DF1_1P_BP_Vintage : public Filters, public VintageProcessor
{
private:
  DF1_1P_LP_Vintage lph{};
  DF1_1P_LP_Vintage lpl{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_2P_BP_Vintage : public Filters
{
private:
  DF1_2P_LP_Vintage lpl{};
  DF1_2P_LP_Vintage lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_3P_BP_Vintage : public Filters, public VintageProcessor
{
private:
  DF1_3P_LP_Vintage lpl{};
  DF1_3P_LP_Vintage lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_4P_BP_Vintage : public Filters, public VintageProcessor
{
private:
  DF1_4P_LP_Vintage lpl{};
  DF1_4P_LP_Vintage lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_6P_BP_Vintage : public Filters, public VintageProcessor
{
private:
  DF1_6P_LP_Vintage lpl{};
  DF1_6P_LP_Vintage lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF2_2P_BP_Vintage : public Filters, public VintageProcessor
{
private:
  DF2_2P_LP_Vintage lpl{};
  DF2_2P_LP_Vintage lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF2_4P_BP_Vintage : public Filters, public VintageProcessor
{
private:
  DF2_4P_LP_Vintage lpl{};
  DF2_4P_LP_Vintage lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class SVF1_2P_BP_Vintage : public Filters, public VintageProcessor
{
private:
  SVF1_2P_LP_Vintage lpl{};
  SVF1_2P_LP_Vintage lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class SVF1_4P_BP_Vintage : public Filters, public VintageProcessor
{
private:
  SVF1_4P_LP_Vintage lpl{};
  SVF1_4P_LP_Vintage lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};
