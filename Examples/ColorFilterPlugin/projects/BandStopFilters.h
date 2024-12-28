#pragma once
#include <projects/Filters.h>
 #include "projects/HighPassFilters.h"

class DF1_1P_BS : public Filters
{
private:
  DF1_1P_LP lp{};
  DF1_1P_HP hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_2P_BS : public Filters
{
private:
  DF1_2P_LP lp{};
  DF1_2P_HP hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_3P_BS : public Filters
{
private:
  DF1_3P_LP lp{};
  DF1_3P_HP hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_4P_BS : public Filters
{
private:
  DF1_4P_LP lp{};
  DF1_4P_HP hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_6P_BS : public Filters
{
private:
  DF1_6P_LP lp{};
  DF1_6P_HP hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF2_2P_BS : public Filters
{
private:
  DF2_2P_LP lp{};
  DF2_2P_HP hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF2_4P_BS : public Filters
{
private:
  DF2_4P_LP lp{};
  DF2_4P_HP hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class SVF1_2P_BS : public Filters
{
private:
  SVF1_2P_LP lp{};
  SVF1_2P_HP hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class SVF1_4P_BS : public Filters
{
private:
  SVF1_4P_LP lp{};
  SVF1_4P_HP hp{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};
