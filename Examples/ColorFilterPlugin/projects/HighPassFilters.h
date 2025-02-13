#pragma once
#include "Filters.h"
#include "Shapers.h"

class DF1_1P_HP : public DF1_1P_LP
{


public:
  DF1_1P_HP()
    : DF1_1P_LP{}
  {
    m_poles.resize(1, 0.0);
  }
  virtual void Process(double& input, FilterParameters& params) override;
};

class DF1_2P_HP : public DF1_1P_HP
{
public:
  DF1_2P_HP()
    : DF1_1P_HP{}
  {
    m_poles.resize(2, 0.0);
    ResoScaling = DF1_2P_ResoScaling;
  }
};

class DF1_3P_HP : public DF1_1P_HP
{
public:
  DF1_3P_HP()
    : DF1_1P_HP{}
  {
    m_poles.resize(3, 0.0);
    ResoScaling = DF1_3P_ResoScaling;
  }
};

class DF1_4P_HP : public DF1_1P_HP
{
public:
  DF1_4P_HP()
    : DF1_1P_HP{}
  {
    m_poles.resize(4, 0.0);
    ResoScaling = DF1_4P_ResoScaling;
  }
};

class DF1_6P_HP : public DF1_1P_HP
{
public:
  DF1_6P_HP()
    : DF1_1P_HP{}
  {
    m_poles.resize(6, 0.0);
    ResoScaling = DF1_6P_ResoScaling;
  }
};

class DF2_2P_HP : public DF2_2P_LP
{
public:
  DF2_2P_HP()
    : DF2_2P_LP{}
  {
    m_poles.resize(2, 0.0);
    ResoScaling = DF2_2P_ResoScaling;
  }
  virtual void Process(double& input, FilterParameters& params) override;
};

class DF2_4P_HP : public DF2_2P_HP
{
public:
  DF2_4P_HP()
    : DF2_2P_HP{}
  {
    m_poles.resize(4, 0.0);
    ResoScaling = DF2_4P_ResoScaling;
  }
};

class DF2_6P_HP : public DF2_2P_HP
{
public:
  DF2_6P_HP()
    : DF2_2P_HP{}
  {
    m_poles.resize(6, 0.0);
    ResoScaling = DF2_6P_ResoScaling;
  }
};

class SVF1_1P_HP : public SVF1_1P_LP
{
public:
  SVF1_1P_HP()
    : SVF1_1P_LP{}
  {
    m_poles.resize(1, 0.0);
  }
  virtual void Process(double& input, FilterParameters& params) override;
};

class SVF1_2P_HP : public SVF1_1P_HP
{
public:
  SVF1_2P_HP()
    : SVF1_1P_HP{}
  {
    m_poles.resize(2, 0.0);
    ResoScaling = SVF1_2P_ResoScaling;
  }
};

class SVF1_3P_HP : public SVF1_1P_HP
{
public:
  SVF1_3P_HP()
    : SVF1_1P_HP{}
  {
    m_poles.resize(3, 0.0);
    ResoScaling = SVF1_3P_ResoScaling;
  }
};

class SVF1_4P_HP : public SVF1_1P_HP
{
public:
  SVF1_4P_HP()
    : SVF1_1P_HP{}
  {
    m_poles.resize(4, 0.0);
    ResoScaling = SVF1_4P_ResoScaling;
  }
};

class SVF1_6P_HP : public SVF1_2P_HP
{
public:
  SVF1_6P_HP()
    : SVF1_2P_HP{}
  {
    m_poles.resize(6, 0.0);
    ResoScaling = SVF1_6P_ResoScaling;
  }
};

class ZDF_1P_HP : public ZDF_1P_LP
{
public:
  ZDF_1P_HP()
    : ZDF_1P_LP{}
  {
    m_poles.resize(1, 0.0);
  }
  virtual void Process(double& input, FilterParameters& params) override;
};

class ZDF_2P_HP : public ZDF_1P_HP
{
public:
  ZDF_2P_HP()
    : ZDF_1P_HP{}
  {
    m_poles.resize(2, 0.0);
    ResoScaling = ZDF1_2P_ResoScaling;
  }
};

class ZDF_3P_HP : public ZDF_1P_HP
{
public:
  ZDF_3P_HP()
    : ZDF_1P_HP{}
  {
    m_poles.resize(3, 0.0);
    ResoScaling = ZDF1_3P_ResoScaling;
  }
};

class ZDF_4P_HP : public ZDF_1P_HP
{
public:
  ZDF_4P_HP()
    : ZDF_1P_HP{}
  {
    m_poles.resize(4, 0.0);
    ResoScaling = ZDF1_4P_ResoScaling;
  }
};

class ZDF_6P_HP : public ZDF_1P_HP
{
public:
  ZDF_6P_HP()
    : ZDF_1P_HP{}
  {
    m_poles.resize(6, 0.0);
    ResoScaling = ZDF1_6P_ResoScaling;
  }
};

