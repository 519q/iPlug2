#pragma once
#include "Filters.h"
#include "HighPassFilters.h"


class DF1_1P_BP : public Filters
{
private:
  DF1_1P_LP lph{};
  DF1_1P_LP lpl{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_2P_BP : public Filters
{
private:
  DF1_2P_LP lpl{};
  DF1_2P_LP lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_3P_BP : public Filters
{
private:
  DF1_3P_LP lpl{};
  DF1_3P_LP lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_4P_BP : public Filters
{
private:
  DF1_4P_LP lpl{};
  DF1_4P_LP lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF1_6P_BP : public Filters
{
private:
  DF1_6P_LP lpl{};
  DF1_6P_LP lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF2_2P_BP : public Filters
{
private:
  DF2_2P_LP lpl{};
  DF2_2P_LP lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class DF2_4P_BP : public Filters
 {
private:
  DF2_4P_LP lpl{};
  DF2_4P_LP lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class SVF1_2P_BP : public Filters
{
private:
  SVF1_2P_LP lpl{};
  SVF1_2P_LP lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class SVF1_4P_BP : public Filters
{
private:
  SVF1_4P_LP lpl{};
  SVF1_4P_LP lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};

class SVF1_6P_BP : public Filters
{
private:
  SVF1_6P_LP lpl{};
  SVF1_6P_HP lph{};
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{0.11};

public:
  void Process(double& input, FilterParameters& params);
};
