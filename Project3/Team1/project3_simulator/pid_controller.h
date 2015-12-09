#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H
class PIDController
{
  double d_kp;
  double d_ki;
  double d_kd;
  double d_kb;
  double d_smooth;
  double d_input;
  double d_output;
  double d_error;
  double d_setpoint;
  double d_integral;
  double d_derivative;
  double d_min;
  double d_max;
  double d_period_in_sec;
  unsigned int d_period;
  bool d_ready;
  bool d_state;

  public:
    PIDController(double kp, double ki, double kd, double setpoint, double kb = 1.0, double smooth = 0.25);
    void setParameters(double kp, double ki, double kd, double kb = 1.0, double smooth = 0.25);
    void setConstraints(double min, double max);
    void setSetpoint(double setpoint);
    void setSamplePeriod(unsigned int period);
    void setState(bool state);
    void reset();
    double getKp() const;
    double getKi() const;
    double getKd() const;
    double getKb() const;
    double getSmooth() const;
    void setKp(double kp);
    void setKi(double ki);
    void setKd(double kd);
    void setKb(double kb);
    void setSmooth(double smooth);
    double getProportional() const;
    double getIntegral() const;
    double getDerivative() const;
    double control(double input);

  private:
    double _constrain(double val);
    double calculateOutput();
};

inline double PIDController::_constrain(double val)
{
  if (val > d_max)
    return d_max;
  else if (val < d_min)
    return d_min;

  return val;
}

inline double PIDController::calculateOutput()
{
  return d_error + d_integral - d_derivative;
}

inline double PIDController::getKp() const { return d_kp; }
inline double PIDController::getKi() const { return d_ki / d_period_in_sec; }
inline double PIDController::getKd() const { return d_kd * d_period_in_sec; }
inline double PIDController::getKb() const { return d_kb; }
inline double PIDController::getSmooth() const { return d_smooth; }
inline void PIDController::setKp(double kp) { d_kp = kp; }
inline void PIDController::setKi(double ki) { d_ki = ki * d_period_in_sec; }
inline void PIDController::setKd(double kd) { d_kd = kd / d_period_in_sec; }
inline void PIDController::setKb(double kb) { d_kb = kb; }
inline void PIDController::setSmooth(double smooth) { d_smooth = smooth; }
inline void PIDController::setConstraints(double min, double max) { d_min = min; d_max = max; }
inline double PIDController::getProportional() const { return d_error; }
inline double PIDController::getIntegral() const { return d_integral; }
inline double PIDController::getDerivative() const { return d_derivative; }

#endif
