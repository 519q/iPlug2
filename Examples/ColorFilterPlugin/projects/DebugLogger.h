/*

Recieve msgs in:
 https://github.com/CobaltFusion/DebugViewPP/releases

// Usage Example: .cpp

#include "DebugLogger.h"
DebugPrint(value);
*/
#pragma once

#include <sstream>
#include <windows.h> // For OutputDebugString
// Helper function to print any value to the debug console
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
