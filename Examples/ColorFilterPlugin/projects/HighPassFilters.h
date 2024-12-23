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