#ifndef ESP32_TB6612_h
#define ESP32_TB6612_h

#include <Arduino.h>

class Motor
{
  public:
    Motor(int pin_in1, int pin_in2, int pin_pwm, int pin_standby, int ledc_channel, bool dir);

    void drive(int speed);
    void drive(int speed, int duration);
    void brake();
    void standby();

  private:
    int pin_in1_;
    int pin_in2_;
    int pin_pwm_;
    int pin_standby_;
    int ledc_channel_;
    bool dir_;

    void fwd(int speed);
    void rev(int speed);
};

#endif