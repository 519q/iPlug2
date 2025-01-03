#pragma once
#include "Filters.h"
#include "Shapers.h"

class DF1_1P_HP : public Filters
{
private:
  double m_alpha{};
  double m_state{};

public:
  void Process(double& input, FilterParameters& params) override;
};

class DF1_2P_HP : public Filters
{
  Sigmoidal sigmoidalShaper{};
  double m_alpha{};
  double m_state[2]{};
  double bandpass{};
  DCStop dcstop{};

public:
  void Process(double& input, FilterParameters& params) override;
};

class DF1_3P_HP : public Filters
{
private:
  Sigmoidal sigmoidalShaper{};
  double m_alpha{};
  double m_state[3];
  DCStop dcstop{};

public:
  DF1_3P_HP()
    : Filters{}
    , m_state{}
    , sigmoidalShaper{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_4P_HP : public Filters
{
private:
  Sigmoidal sigmoidalShaper{};
  double m_alpha{};
  double m_state[4];
  DCStop dcstop{};

public:
  DF1_4P_HP()
    : Filters{}
    , m_state{}
    , sigmoidalShaper{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF1_6P_HP : public Filters
{
private:
  Sigmoidal sigmoidalShaper{};
  double m_alpha{};
  double m_state[6];
  DCStop dcstop{};

public:
  DF1_6P_HP()
    : Filters{}
    , m_state{}
    , sigmoidalShaper{}
  {
  }
  void Process(double& input, FilterParameters& params) override;
};

class DF2_2P_HP : public Filters
{
private:
  Sigmoidal sigmoidalShaper{};
  double m_state[2]{};
  DCStop dcstop{};

public:
  DF2_2P_HP()
    : Filters{}
  {
  }

  void Process(double& input, FilterParameters& params) override;
};

class DF2_4P_HP : public Filters
{
private:
  Sigmoidal sigmoidalShaper{};
  double m_state[4]{};
  DCStop dcstop{};

public:
  DF2_4P_HP()
    : Filters{}
  {
  }

  void Process(double& input, FilterParameters& params) override;
};

class SVF1_2P_HP : public Filters
{
public:
  SVF1_2P_HP()
    : Filters{}
  {
  }

private:
  Sigmoidal sigmoidalShaper{};
  double m_state[2]{};
  DCStop dcstop{};

public:
  void Process(double& input, FilterParameters& params) override;
};

class SVF1_4P_HP : public Filters
{
public:
  SVF1_4P_HP()
    : Filters{}
  {
  }

private:
  Sigmoidal sigmoidalShaper{};
  double m_state[4]{};
  DCStop dcstop{};

public:
  void Process(double& input, FilterParameters& params) override;
};

class SVF1_6P_HP : public Filters
{
public:
  SVF1_6P_HP()
    : Filters{}
  {
  }

private:
  Sigmoidal sigmoidalShaper{};
  double m_state[6]{};
  DCStop dcstop{};

public:
  void Process(double& input, FilterParameters& params) override;
};
