#pragma once
#include "projects/FilterParameters.h"
#include <cmath>
#include <functional>
#include <vector>

template <typename T>
class StateMixer
{
private:
  double m_previousFractionalSize{};
  double m_fractionalSize{};
  int m_ceil_size{};
  int m_floor_size{};
  int m_stepSize{};
  int m_sizeMultiplier{};

  std::vector<T> m_floor_T{};
  std::vector<T> m_ceil_T{};

public:
  StateMixer(int sizeMultiplier)
    : m_sizeMultiplier(std::max(1, sizeMultiplier))
  {
    m_floor_T.resize(m_sizeMultiplier);
    m_ceil_T.resize(m_sizeMultiplier);
  }
  double Process(double input, FilterParameters& params, double sizeParam, int baseStepSize, std::function<void(T&, double&)> func)
  {
    m_stepSize = baseStepSize;
    // Calculate velocity-dependent step size
    double velocity = std::abs(m_fractionalSize - m_previousFractionalSize);
    m_previousFractionalSize = m_fractionalSize;
    // Calculate the fractional pole count
    m_fractionalSize = sizeParam * m_sizeMultiplier;                 // Scale control to [0, 200]
    int floorSize = static_cast<int>(std::floor(m_fractionalSize));  // Integer part (floor)
    int ceilSize = std::min(m_sizeMultiplier, floorSize + m_stepSize); // Next integer part (ceiling)
    double interpFactor = m_fractionalSize - floorSize;              // Fractional part for interpolation
    if (floorSize != m_floor_size || ceilSize != m_ceil_size)
    {
      if (floorSize > m_floor_size)
      {
        std::swap(m_floor_T, m_ceil_T); // Safe swap instead of move
      }
      else if (floorSize < m_floor_size)
      {
        int oldSize = m_floor_size;
        T lastT = m_floor_size > 0 ? m_floor_T[m_floor_size - 1] : T(); // Safe default value
        std::swap(m_ceil_T, m_floor_T);
        for (int i = oldSize; i < floorSize; ++i)
        {
          m_ceil_T[i] = lastT; // Ensure valid initialization
        }
      } // Safe swap instead of move
    }
    m_floor_size = floorSize;
    m_ceil_size = ceilSize;

    // Process the input through the floor processors
    double processedFloor = input;
    for (int i = 0; i < m_floor_size && i < m_floor_T.size(); ++i)
    {
      func(m_floor_T[i], processedFloor);
    }

    // Process ceiling filters (only first 'ceilSize' elements)
    double processedCeil = input;
    for (int i = 0; i < m_ceil_size && i < m_ceil_T.size(); ++i)
    {
      func(m_ceil_T[i], processedCeil);
    }

    // Interpolate between the floor and ceiling outputs
    double processed = interpolateLin(processedFloor, processedCeil, interpFactor);

    return processed;
  }
  
};
