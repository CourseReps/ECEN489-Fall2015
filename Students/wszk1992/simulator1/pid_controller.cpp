#include "pid_controller.h"

PIDController::PIDController(double kp, double ki, double kd, double setpoint, double kb, double smooth)
  : d_period(1000), d_period_in_sec(1)
{
  setParameters(kp, ki, kd, kb, smooth);
  setSetpoint(setpoint);
  setConstraints(0.0, 255.0);
  setState(true);
  reset();
}

void PIDController::setSetpoint(double setpoint)
{
  d_setpoint = setpoint;
}

void PIDController::setParameters(double kp, double ki, double kd, double kb, double smooth)
{
  d_kp = kp;
  d_ki = ki * d_period_in_sec;
  d_kd = kd / d_period_in_sec;
  d_kb = kb;
  d_smooth = smooth;
}

void PIDController::setSamplePeriod(unsigned int period)
{
  d_period_in_sec = static_cast<double>(period) / 1000.0;
  double ratio = static_cast<double>(period) / static_cast<double>(d_period);

  d_ki *= ratio;
  d_kd /= ratio;
  d_period = period;
}

void PIDController::reset()
{
  d_ready = false;
  d_integral = 0;
  d_error = 0;
  d_derivative = 0;
  d_input = 0;
  d_output = 0;
}

void PIDController::setState(bool state)
{
  d_state = state;
  if(!state)
    d_ready = false;
}

double PIDController::control(double input)
{
  if(!d_state)
    return 0;
  d_error = d_setpoint - input;
  if(d_ready)
    d_derivative = (1 - d_smooth) * d_derivative + d_kd * d_smooth * (input - d_input); // Low pass filtered derivative
  else
    { d_derivative = 0; d_ready = true; }

  d_integral += d_ki * d_error;

  d_error *= d_kp;

  d_output = calculateOutput();

  d_integral += d_kb * (_constrain(d_output) - d_output);
  d_input = input;
//  std::cout<<"calculateOutput(): "<<calculateOutput()<<std::endl;
  return d_output = _constrain(calculateOutput());
}
