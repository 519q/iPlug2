#pragma once
#include "HighPassFilters.h"
#include "VintageFilters.h"

class DF1_1P_HP_Vintage : public DF1_1P_LP_Vintage
{
protected:
  int m_poles{};
  int m_resonance{};
  //DCStop dcstop{};

public:
  DF1_1P_HP_Vintage()
    : DF1_1P_LP_Vintage{}
  {
  }
  void processPoleHP(int& input, int& state, FilterParameters& params) const;
  void Process(double& input, FilterParameters& params) override;
};

class DF1_2P_HP_Vintage : public DF1_1P_HP_Vintage
{
  int m_poles[2]{};

public:
  DF1_2P_HP_Vintage()
    : DF1_1P_HP_Vintage{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_3P_HP_Vintage : public DF1_1P_HP_Vintage
{
  int m_poles[3]{};

public:
  DF1_3P_HP_Vintage()
    : DF1_1P_HP_Vintage{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_4P_HP_Vintage : public DF1_1P_HP_Vintage
{
  int m_poles[4]{};

public:
  DF1_4P_HP_Vintage()
    : DF1_1P_HP_Vintage{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_6P_HP_Vintage : public DF1_1P_HP_Vintage
{
  int m_poles[6]{};

public:
  DF1_6P_HP_Vintage()
    : DF1_1P_HP_Vintage{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF2_2P_HP_Vintage : public DF2_2P_LP_Vintage, public Highpass

{
private:
  int m_poles[2]{};

public:
  DF2_2P_HP_Vintage()
    : DF2_2P_LP_Vintage{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF2_4P_HP_Vintage : public DF2_2P_HP_Vintage

{
private:
  int m_poles[4]{};

public:
  DF2_4P_HP_Vintage()
    : DF2_2P_HP_Vintage{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class SVF1_2P_HP_Vintage : public SVF1_2P_LP_Vintage
{
private:
  int m_poles[2]{};

public:
  SVF1_2P_HP_Vintage()
    : SVF1_2P_LP_Vintage{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class SVF1_4P_HP_Vintage : public SVF1_2P_HP_Vintage
{
private:
  int m_poles[4]{};

public:
  SVF1_4P_HP_Vintage()
    : SVF1_2P_HP_Vintage{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class SVF1_6P_HP_Vintage : public SVF1_2P_HP_Vintage
{
private:
  int m_poles[6]{};

public:
  SVF1_6P_HP_Vintage()
    : SVF1_2P_HP_Vintage{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};
