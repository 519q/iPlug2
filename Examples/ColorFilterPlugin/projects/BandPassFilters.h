#pragma once
#include "Filters.h"
#include "HighPassFilters.h"

class BandpassFilters : public Filters
{
private:
  std::unique_ptr<Filters> lpl;
  std::unique_ptr<Filters> lph;
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{bandwidthHighestLimit};

public:
  BandpassFilters(std::unique_ptr<Filters> lplFilter, std::unique_ptr<Filters> lphFilter)
    : lpl(std::move(lplFilter))
    , lph(std::move(lphFilter))
  {
  }
  virtual void Process(double& input, FilterParameters& params) override;
};

class DF1_1P_BP : public BandpassFilters
{
public:
  DF1_1P_BP()
    : BandpassFilters(std::make_unique<DF1_1P_LP>(), std::make_unique<DF1_1P_LP>())
  {
  }
};

class DF1_2P_BP : public BandpassFilters
{
public:
  DF1_2P_BP()
    : BandpassFilters(std::make_unique<DF1_2P_LP>(), std::make_unique<DF1_2P_LP>())
  {
  }
};

class DF1_3P_BP : public BandpassFilters
{
public:
  DF1_3P_BP()
    : BandpassFilters(std::make_unique<DF1_3P_LP>(), std::make_unique<DF1_3P_LP>())
  {
  }
};

class DF1_4P_BP : public BandpassFilters
{
public:
  DF1_4P_BP()
    : BandpassFilters(std::make_unique<DF1_4P_LP>(), std::make_unique<DF1_4P_LP>())
  {
  }
};

class DF1_6P_BP : public BandpassFilters
{
public:
  DF1_6P_BP()
    : BandpassFilters(std::make_unique<DF1_6P_LP>(), std::make_unique<DF1_6P_LP>())
  {
  }
};

class DF2_2P_BP : public BandpassFilters
{
public:
  DF2_2P_BP()
    : BandpassFilters(std::make_unique<DF2_2P_LP>(), std::make_unique<DF2_2P_LP>())
  {
  }
};

class DF2_4P_BP : public BandpassFilters
{
public:
  DF2_4P_BP()
    : BandpassFilters(std::make_unique<DF2_4P_LP>(), std::make_unique<DF2_4P_LP>())
  {
  }
};

class DF2_6P_BP : public BandpassFilters
{
public:
  DF2_6P_BP()
    : BandpassFilters(std::make_unique<DF2_6P_LP>(), std::make_unique<DF2_6P_LP>())
  {
  }
};

class SVF1_1P_BP : public BandpassFilters
{
public:
  SVF1_1P_BP()
    : BandpassFilters(std::make_unique<SVF1_1P_LP>(), std::make_unique<SVF1_1P_LP>())
  {
  }
};

class SVF1_2P_BP : public BandpassFilters
{
public:
  SVF1_2P_BP()
    : BandpassFilters(std::make_unique<SVF1_2P_LP>(), std::make_unique<SVF1_2P_LP>())
  {
  }
};

class SVF1_3P_BP : public BandpassFilters
{
public:
  SVF1_3P_BP()
    : BandpassFilters(std::make_unique<SVF1_3P_LP>(), std::make_unique<SVF1_3P_LP>())
  {
  }
};

class SVF1_4P_BP : public BandpassFilters
{
public:
  SVF1_4P_BP()
    : BandpassFilters(std::make_unique<SVF1_4P_LP>(), std::make_unique<SVF1_4P_LP>())
  {
  }
};

class SVF1_6P_BP : public BandpassFilters
{
public:
  SVF1_6P_BP()
    : BandpassFilters(std::make_unique<SVF1_6P_LP>(), std::make_unique<SVF1_6P_LP>())
  {
  }
};

class ZDF_1P_BP : public BandpassFilters
{
public:
  ZDF_1P_BP()
    : BandpassFilters(std::make_unique<ZDF_1P_LP>(), std::make_unique<ZDF_1P_LP>())
  {
  }
};

class ZDF_2P_BP : public BandpassFilters
{
public:
  ZDF_2P_BP()
    : BandpassFilters(std::make_unique<ZDF_2P_LP>(), std::make_unique<ZDF_2P_LP>())
  {
  }
};

class ZDF_3P_BP : public BandpassFilters
{
public:
  ZDF_3P_BP()
    : BandpassFilters(std::make_unique<ZDF_3P_LP>(), std::make_unique<ZDF_3P_LP>())
  {
  }
};

class ZDF_4P_BP : public BandpassFilters
{
public:
  ZDF_4P_BP()
    : BandpassFilters(std::make_unique<ZDF_4P_LP>(), std::make_unique<ZDF_4P_LP>())
  {
  }
};

class ZDF_6P_BP : public BandpassFilters
{
public:
  ZDF_6P_BP()
    : BandpassFilters(std::make_unique<ZDF_6P_LP>(), std::make_unique<ZDF_6P_LP>())
  {
  }
};
