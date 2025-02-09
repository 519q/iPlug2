#pragma once
#include <cmath>

#pragma region Constants

constexpr int LUT_BITS{12};
constexpr int LUT_SIZE{1 << LUT_BITS};
constexpr int VINTAGE_BIT_RATE{16};

static constexpr double bandwidthLowestLimit = 0.025;
static constexpr double bandwidthHighestLimit = 0.07;

static constexpr double DF1_2P_ResoScaling = 2;
static constexpr double DF1_3P_ResoScaling = 1.2;
static constexpr double DF1_4P_ResoScaling = 1.44;
static constexpr double DF1_6P_ResoScaling = 0.9;

static constexpr double DF2_2P_ResoScaling = 6;
static constexpr double DF2_4P_ResoScaling = 4;

static constexpr double SVF1_2P_ResoScaling = 1.9;
static constexpr double SVF1_4P_ResoScaling = 1.4;
static constexpr double SVF1_6P_ResoScaling = 0.95;

#pragma endregion


enum class FilterTypes
{
  DF1_1P,
  DF1_2P,
  DF1_3P,
  DF1_4P,
  DF1_6P,
  DF2_2P,
  DF2_4P,
  SVF1_2P,
  SVF1_4P,
  SVF1_6P,
  MAX_FILTER_TYPES
};

enum class FilterAlgo
{
  DF1,
  DF2,
  SVF1,
  MAX_ALGO
};

enum class Spectral_IR
{
  FIR,
  IIR,
  MAX_IR
};

enum class FilterType
{
  LP,
  BP,
  BS,
  HP
};


class FilterParameters
{
public:
  int m_filterType{};
  int m_filterSelector{};

  double m_cutoff{};
  double m_resonance{};
  double m_bandwidth{};
  double m_drive{};
  double m_shape{};
  double m_bias{};
  double m_spectralFilterDrive{};
  bool m_spectralFilter_IR{};
  bool m_spectralFilter_Harder{};

  int m_spectralFilterFIR_Order{};
  int m_spectralShaperFIR_Order{};

  int m_spectralFilterIIR_Order{};
  int m_spectralShaperIIR_Order{};

  double m_spectralShaperDrive{};
  bool m_spectralShaper_IR{};
  int m_spectralShaperSelector{};
  double m_sampleRate{};
  int m_oversample{};

  void setFilterParameters(int filterType,
                           int filterSelector,
                           bool spectralFilter_IR,
                           double cutoff,
                           double resonance,
                           double bandwidth,
                           double drive,
                           double shape,
                           double bias,
                           double spectralShaperDrive,
                           bool spectralShaper_IR,
                           double spectralFilterDrive,
                           bool spectralFilter_harder,
                           int spectralShaperSelector,
                           int spectralFilterFIR_Order,
                           int spectralShaperFIR_Order,
                           int spectralFilterIIR_Order,
                           int spectralShaperIIR_Order,
                           int overSampling,
                           double sampleRate)
  {
    m_filterType = filterType;
    m_filterSelector = filterSelector;
    m_spectralFilterDrive = spectralFilterDrive;
    m_spectralFilter_IR = spectralFilter_IR;
    m_spectralFilter_Harder = spectralFilter_harder;
    m_cutoff = cutoff;
    m_resonance = resonance;
    m_bandwidth = bandwidth;
    m_drive = drive;
    m_shape = shape;
    m_bias = bias;

    m_spectralShaperDrive = spectralShaperDrive;
    m_spectralShaper_IR = spectralShaper_IR;
    m_spectralShaperSelector = spectralShaperSelector;

    m_spectralFilterFIR_Order = spectralFilterFIR_Order;
    m_spectralShaperFIR_Order = spectralShaperFIR_Order;

    m_spectralFilterIIR_Order = spectralFilterIIR_Order;
    m_spectralShaperIIR_Order = spectralShaperIIR_Order;

    m_oversample = overSampling;
    m_sampleRate = sampleRate * std::pow(2, m_oversample);
  }
};
