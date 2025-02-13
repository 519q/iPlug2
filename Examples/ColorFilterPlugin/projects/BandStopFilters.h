#pragma once
#include "projects/HighPassFilters.h"
#include <projects/Filters.h>

class BandstopFilters : public Filters
{
  std::unique_ptr<Filters> lp;
  std::unique_ptr<Filters> hp;
  double m_stateL{};
  double m_stateH{};
  double m_bandwidth{bandwidthHighestLimit};

public:
  BandstopFilters(std::unique_ptr<Filters> lpFilter, std::unique_ptr<Filters> hpFilter)
    : lp(std::move(lpFilter))
    , hp(std::move(hpFilter))
  {
  }
  virtual void Process(double& input, FilterParameters& params) override;
  // virtual ~BandstopFilters() = default;
};

class DF1_1P_BS : public BandstopFilters
{
public:
  DF1_1P_BS()
    : BandstopFilters(std::make_unique<DF1_1P_LP>(), std::make_unique<DF1_1P_HP>())
  {
  }
};

class DF1_2P_BS : public BandstopFilters
{
public:
  DF1_2P_BS()
    : BandstopFilters(std::make_unique<DF1_2P_LP>(), std::make_unique<DF1_2P_HP>())
  {
  }
};

class DF1_3P_BS : public BandstopFilters
{
public:
  DF1_3P_BS()
    : BandstopFilters(std::make_unique<DF1_3P_LP>(), std::make_unique<DF1_3P_HP>())
  {
  }
};

class DF1_4P_BS : public BandstopFilters
{
public:
  DF1_4P_BS()
    : BandstopFilters(std::make_unique<DF1_4P_LP>(), std::make_unique<DF1_4P_HP>())
  {
  }
};

class DF1_6P_BS : public BandstopFilters
{
public:
  DF1_6P_BS()
    : BandstopFilters(std::make_unique<DF1_6P_LP>(), std::make_unique<DF1_6P_HP>())
  {
  }
};

class DF2_2P_BS : public BandstopFilters
{
public:
  DF2_2P_BS()
    : BandstopFilters(std::make_unique<DF2_2P_LP>(), std::make_unique<DF2_2P_HP>())
  {
  }
};

class DF2_4P_BS : public BandstopFilters
{
public:
  DF2_4P_BS()
    : BandstopFilters(std::make_unique<DF2_4P_LP>(), std::make_unique<DF2_4P_HP>())
  {
  }
};

class DF2_6P_BS : public BandstopFilters
{
public:
  DF2_6P_BS()
    : BandstopFilters(std::make_unique<DF2_6P_LP>(), std::make_unique<DF2_6P_HP>())
  {
  }
};

class SVF1_1P_BS : public BandstopFilters
{
public:
  SVF1_1P_BS()
    : BandstopFilters(std::make_unique<SVF1_1P_LP>(), std::make_unique<SVF1_1P_HP>())
  {
  }
};

class SVF1_2P_BS : public BandstopFilters
{
public:
  SVF1_2P_BS()
    : BandstopFilters(std::make_unique<SVF1_2P_LP>(), std::make_unique<SVF1_2P_HP>())
  {
  }
};

class SVF1_3P_BS : public BandstopFilters
{
public:
  SVF1_3P_BS()
    : BandstopFilters(std::make_unique<SVF1_3P_LP>(), std::make_unique<SVF1_3P_HP>())
  {
  }
};

class SVF1_4P_BS : public BandstopFilters
{
public:
  SVF1_4P_BS()
    : BandstopFilters(std::make_unique<SVF1_4P_LP>(), std::make_unique<SVF1_4P_HP>())
  {
  }
};

class SVF1_6P_BS : public BandstopFilters
{
public:
  SVF1_6P_BS()
    : BandstopFilters(std::make_unique<SVF1_6P_LP>(), std::make_unique<SVF1_6P_HP>())
  {
  }
};

class ZDF_1P_BS : public BandstopFilters
{
public:
  ZDF_1P_BS()
    : BandstopFilters(std::make_unique<ZDF_1P_LP>(), std::make_unique<ZDF_1P_HP>())
  {
  }
};

class ZDF_2P_BS : public BandstopFilters
{
public:
  ZDF_2P_BS()
    : BandstopFilters(std::make_unique<ZDF_2P_LP>(), std::make_unique<ZDF_2P_HP>())
  {
  }
};

class ZDF_3P_BS : public BandstopFilters
{
public:
  ZDF_3P_BS()
    : BandstopFilters(std::make_unique<ZDF_3P_LP>(), std::make_unique<ZDF_3P_HP>())
  {
  }
};

class ZDF_4P_BS : public BandstopFilters
{
public:
  ZDF_4P_BS()
    : BandstopFilters(std::make_unique<ZDF_4P_LP>(), std::make_unique<ZDF_4P_HP>())
  {
  }
};

class ZDF_6P_BS : public BandstopFilters
{
public:
  ZDF_6P_BS()
    : BandstopFilters(std::make_unique<ZDF_6P_LP>(), std::make_unique<ZDF_6P_HP>())
  {
  }
};
