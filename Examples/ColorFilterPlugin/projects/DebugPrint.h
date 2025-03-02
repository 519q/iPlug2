/*

Recieve msgs in:
 https://github.com/CobaltFusion/DebugViewPP/releases

// Usage Example: .cpp

#include "DebugPrint.h"
DebugPrint(value);
*/
#pragma once

#include <sstream>
#include <windows.h> // For OutputDebugString
// Helper function to print any value to the debug console

inline bool counter()
{
  static int debugCounter = 0;
  if (debugCounter == 48000)
  {
    debugCounter = 0;
    return true;
  }
  ++debugCounter;
  return false;
}

template <typename... Args>
void DebugPrint(const std::string& label, Args... values)
{
  std::ostringstream oss;
  if (!label.empty())
  {
    oss << label << ": ";
  }
  ((oss << values << "\n"), ...);
  OutputDebugString((oss.str() + "\n").c_str());
}
template <typename T>
void DebugPrint(const T value, const std::string& label = "")
{
  std::ostringstream oss;
  if (!label.empty())
  {
    oss << label << ": ";
  }
  oss << value << "\n";
  OutputDebugString((oss.str() + "\n").c_str());
}
template <typename... Args>
inline void DebugPrintDSP(const std::string& label, Args... values)
{
  if (counter())
  {
    std::ostringstream oss;
    if (!label.empty())
    {
      oss << label << ": ";
    }
    ((oss << values << "\n"), ...);
    OutputDebugString((oss.str() + "\n").c_str());
  }
}
template <typename T>
inline void DebugPrintDSP(const T value, const std::string& label = "")
{
  if (counter())
  {
    std::ostringstream oss;
    if (!label.empty())
    {
      oss << label << ": ";
    }
    oss << value << "\n";
    OutputDebugString((oss.str() + "\n").c_str());
  }
}
class DebugPrintObj
{
private:
  int debugCounter{};
  bool counter()
  {
    
    if (debugCounter == 48000)
    {
      debugCounter = 0;
      return true;
    }
    ++debugCounter;
    return false;
  }

public:
  template <typename... Args>
  void DebugPrint(const std::string& label, Args... values)
  {
    std::ostringstream oss;
    if (!label.empty())
    {
      oss << label << ": ";
    }
    ((oss << values << "\n"), ...);
    OutputDebugString((oss.str() + "\n").c_str());
  }
  template <typename T>
  void DebugPrint(const T value, const std::string& label = "")
  {
    std::ostringstream oss;
    if (!label.empty())
    {
      oss << label << ": ";
    }
    oss << value << "\n";
    OutputDebugString((oss.str() + "\n").c_str());
  }
  template <typename... Args>
  void DebugPrintDSP(const std::string& label, Args... values)
  {
    if (counter())
    {
      std::ostringstream oss;
      if (!label.empty())
      {
        oss << label << ": ";
      }
      ((oss << values << "\n"), ...);
      OutputDebugString((oss.str() + "\n").c_str());
    }
  }
  template <typename T>
  void DebugPrintDSP(const T value, const std::string& label = "")
  {
    if (counter())
    {
      std::ostringstream oss;
      if (!label.empty())
      {
        oss << label << ": ";
      }
      oss << value << "\n";
      OutputDebugString((oss.str() + "\n").c_str());
    }
  }
};
