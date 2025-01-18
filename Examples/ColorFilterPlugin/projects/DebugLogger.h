/*

Recieve msgs in:
 https://learn.microsoft.com/sysinternals/downloads/debugview

// Usage Example: .cpp

#include "DebugLogger.h"

IppStatus initStatus = ippsFFTInit_R_64f(&pFFTSpec, order, IPP_FFT_NODIV_BY_ANY, ippAlgHintNone, pFFTInitBuf, pFFTBuffer);
if (initStatus != ippStsNoErr)
{
  DebugPrint(ippGetStatusString(initStatus));
}

*/
#pragma once

#include <sstream>
#include <windows.h> // For OutputDebugString
// Helper function to print any value to the debug console
template <typename T>
void DebugPrint(const T& value, const std::string& label = "")
{
  std::ostringstream oss;
  if (!label.empty())
  {
    oss << label << ": ";
  }
  oss << value;
  OutputDebugString((oss.str() + "\n").c_str());
}
