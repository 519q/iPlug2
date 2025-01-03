#pragma once
#pragma once
#pragma once
#include "VintageFilters.h"
#include "VintageHighPassFilters.h"

class DF1_1P_BS_Vintage : public Filters
{
private:
  DF1_1P_LP_Vintage lp{};
  DF1_1P_HP_Vintage hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_2P_BS_Vintage : public Filters
{
private:
  DF1_2P_LP_Vintage lp{};
  DF1_2P_HP_Vintage hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_3P_BS_Vintage : public Filters
{
private:
  DF1_3P_LP_Vintage lp{};
  DF1_3P_HP_Vintage hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_4P_BS_Vintage : public Filters
{
private:
  DF1_4P_LP_Vintage lp{};
  DF1_4P_HP_Vintage hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_6P_BS_Vintage : public Filters
{
private:
  DF1_6P_LP_Vintage lp{};
  DF1_6P_HP_Vintage hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF2_2P_BS_Vintage : public Filters
{
private:
  DF2_2P_LP_Vintage lp{};
  DF2_2P_HP_Vintage hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF2_4P_BS_Vintage : public Filters
{
private:
  DF2_4P_LP_Vintage lp{};
  DF2_4P_HP_Vintage hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class SVF1_2P_BS_Vintage : public Filters
{
private:
  SVF1_2P_LP_Vintage lp{};
  SVF1_2P_HP_Vintage hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class SVF1_4P_BS_Vintage : public Filters
{
private:
  SVF1_4P_LP_Vintage lp{};
  SVF1_4P_HP_Vintage hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class SVF1_6P_BS_Vintage : public Filters
{
private:
  SVF1_6P_LP_Vintage lp{};
  SVF1_6P_HP_Vintage hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};
