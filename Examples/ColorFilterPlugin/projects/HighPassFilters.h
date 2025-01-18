#pragma once
#include "Filters.h"
#include "Shapers.h"

class Highpass
{
protected:
  //DCStop dcstop{};
};

class DF1_1P_HP : public DF1_1P_LP, public Highpass
{


public:
  DF1_1P_HP()
    : DF1_1P_LP{}
  {
  }
  void processPoleHP(double& input, double& state, FilterParameters& params) const;
  void Process(double& input, FilterParameters& params) override;
};

class DF1_2P_HP : public DF1_1P_HP
{
private:
  double m_poles[2]{};
public:
  DF1_2P_HP()
    : DF1_1P_HP{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_3P_HP : public DF1_1P_HP
{
private:
  double m_poles[3];
public:
  DF1_3P_HP()
    : DF1_1P_HP{}
    , m_poles{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_4P_HP : public DF1_1P_HP
{
private:
  double m_poles[4];

public:
  DF1_4P_HP()
    : DF1_1P_HP{}
    , m_poles{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_6P_HP : public DF1_1P_HP
{
private:
  double m_poles[6];

public:
  DF1_6P_HP()
    : DF1_1P_HP{}
    , m_poles{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF2_2P_HP : public DF2_2P_LP
{
private:
  double m_poles[2]{};

protected:
  //DCStop dcstop{};

public:
  DF2_2P_HP()
    : DF2_2P_LP{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF2_4P_HP : public DF2_2P_HP
{
private:
  double m_poles[4]{};

public:
  DF2_4P_HP()
    : DF2_2P_HP{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class SVF1_2P_HP : public SVF1_2P_LP, public Highpass
{
public:
  SVF1_2P_HP()
    : SVF1_2P_LP{}
  {
  }

private:
  double m_poles[2]{};

public:
  void processPoleHP(double& input, double& state, FilterParameters& params);
  void Process(double& input, FilterParameters& params) override;
};

class SVF1_4P_HP : public SVF1_2P_HP
{
public:
  SVF1_4P_HP()
    : SVF1_2P_HP{}
  {
  }

private:
  double m_poles[4]{};

public:
  void Process(double& input, FilterParameters& params) override;
};

class SVF1_6P_HP : public SVF1_2P_HP
{
public:
  SVF1_6P_HP()
    : SVF1_2P_HP{}
  {
  }

private:
  double m_poles[6]{};

public:
  void Process(double& input, FilterParameters& params) override;
};
