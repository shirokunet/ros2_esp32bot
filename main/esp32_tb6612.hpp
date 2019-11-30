#ifndef ESP32_TB6612_h
#define ESP32_TB6612_h

#include <Arduino.h>

class Motor
{
  public:
    Motor(int pin_in1, int pin_in2, int pin_pwm, int pin_standby, int ledc_channel, bool dir);

    void drive(float desired_speed);    // -1.0 ~ 1.0
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
    int minmax(int value, int min, int max);
};

#endif