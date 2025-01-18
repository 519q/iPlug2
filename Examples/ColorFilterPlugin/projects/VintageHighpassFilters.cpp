//#include "VintageHighpassFilters.h"
//
//void DF1_1P_HP_Vintage::processPoleHP(int& input, int& state, FilterParameters& params) const
//{
//  input += m_resonance;
//  state = (1 - m_alpha) * input + m_alpha * state;
//  int shaped = -state;
//  input += shaped;
//}
//
//void DF1_1P_HP_Vintage::Process(double& input, FilterParameters& params)
//{
//  getAlpha(params);
//  int fixedPointInput = (double)input * m_scale;
//  processPoleLP(fixedPointInput, m_state, params);
//  input -= (double)fixedPointInput / m_scale;
//}
//
//void DF1_2P_HP_Vintage::Process(double& input, FilterParameters& params)
//{
//  getAlpha(params);
//  int fixedPointInput = (double)input * m_scale;
//  double reso = resonate(fixedPointInput, params, m_poles[1], m_poles[0], 2);
//  for (int& pole : m_poles)
//  {
//    fixedPointInput += reso;
//    processPoleHP(fixedPointInput, pole, params);
//  }
//  input = (double)fixedPointInput / m_scale;
//  dcstop.process(input, params);
//}
//
//void DF1_3P_HP_Vintage::Process(double& input, FilterParameters& params)
//{
//  getAlpha(params);
//  int fixedPointInput = (double)input * m_scale;
//  double reso = resonate(fixedPointInput, params, m_poles[2], m_poles[0], 1.2);
//  for (int& pole : m_poles)
//  {
//    fixedPointInput += reso;
//    processPoleHP(fixedPointInput, pole, params);
//  }
//  input = (double)fixedPointInput / m_scale;
//}
//
//void DF1_4P_HP_Vintage::Process(double& input, FilterParameters& params)
//{
//  getAlpha(params);
//  int fixedPointInput = (double)input * m_scale;
//  double reso = resonate(fixedPointInput, params, m_poles[3], m_poles[1], 1.5);
//  for (int& pole : m_poles)
//  {
//    fixedPointInput += reso;
//    processPoleHP(fixedPointInput, pole, params);
//  }
//  input = (double)fixedPointInput / m_scale;
//}
//
//void DF1_6P_HP_Vintage::Process(double& input, FilterParameters& params)
//{
//  getAlpha(params);
//  int fixedPointInput = (double)input * m_scale;
//  double reso = resonate(fixedPointInput, params, m_poles[3], m_poles[1], 1.5);
//  for (int& pole : m_poles)
//  {
//    fixedPointInput += reso;
//    processPoleHP(fixedPointInput, pole, params);
//  }
//  input = (double)fixedPointInput / m_scale;
//}
//
//void DF2_2P_HP_Vintage::Process(double& input, FilterParameters& params)
//{
//  getCoefficients(params, 12.3, 0.25, -0.01, true);
//  addTolerances(params);
//  int inputScaled = input * m_scale;
//  process2pole(inputScaled, params, m_poles[0], m_poles[1]);
//  input = (double)inputScaled / m_scale;
//  addLeakage();
//}
//
//void DF2_4P_HP_Vintage::Process(double& input, FilterParameters& params)
//{
//  getCoefficients(params, 6.6, 0.25, -0.01, true);
//  addTolerances(params);
//  //dcstop.process(input, params);
//  int inputScaled = input * m_scale;
//  process2pole(inputScaled, params, m_poles[0], m_poles[1]);
//  input = (double)inputScaled / m_scale;
//  //dcstop.process(input, params);
//  inputScaled = input * m_scale;
//  process2pole(inputScaled, params, m_poles[2], m_poles[3]);
//  input = (double)inputScaled / m_scale;
//  addLeakage();
//}
//
//void SVF1_2P_HP_Vintage::Process(double& input, FilterParameters& params)
//{
//  getCoefficients(params);
//  int inputScaled = (double)input * m_scale;
//  int reso = resonate(inputScaled, params, m_poles[1], m_poles[0], 2.01);
//  for (int& pole : m_poles)
//  {
//    inputScaled += reso;
//    processPoleLP(inputScaled, pole, params, true);
//  }
//  input = (double)inputScaled / m_scale;
//};
//
//void SVF1_4P_HP_Vintage::Process(double& input, FilterParameters& params)
//{
//  getCoefficients(params);
//  int inputScaled = (double)input * m_scale;
//  int reso = resonate(inputScaled, params, m_poles[3], m_poles[1], 1.55);
//  for (int& pole : m_poles)
//  {
//    inputScaled += reso;
//    processPoleLP(inputScaled, pole, params, true);
//  }
//  input = (double)inputScaled / m_scale;
//};
//
//void SVF1_6P_HP_Vintage::Process(double& input, FilterParameters& params)
//{
//  getCoefficients(params);
//  int inputScaled = (double)input * m_scale;
//  int reso = resonate(inputScaled, params, m_poles[5], m_poles[1], 0.95);
//  for (int& pole : m_poles)
//  {
//    inputScaled += reso;
//    processPoleLP(inputScaled, pole, params, true);
//  }
//  input = (double)inputScaled / m_scale;
//};
