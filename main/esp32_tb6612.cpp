#include "esp32_tb6612.hpp"
#include <Arduino.h>

Motor::Motor(int pin_in1, int pin_in2, int pin_pwm, int pin_standby, int ledc_channel, bool dir)
{
  pin_in1_ = pin_in1;
  pin_in2_ = pin_in2;
  pin_pwm_ = pin_pwm;
  pin_standby_ = pin_standby;
  ledc_channel_ = ledc_channel;
  dir_ = dir;
  
  pinMode(pin_in1_, OUTPUT);
  pinMode(pin_in2_, OUTPUT);
  pinMode(pin_pwm_, OUTPUT);
  pinMode(pin_standby_, OUTPUT);

  ledcSetup(ledc_channel_, 12800, 8);
  ledcAttachPin(pin_pwm_, ledc_channel_);
}

void Motor::drive(int speed)
{
  digitalWrite(pin_standby_, HIGH);

  if (dir_ == false) speed = -speed;

  if (speed >= 0) fwd(speed);
  else rev(-speed);
}

void Motor::drive(int speed, int duration)
{
  drive(speed);
  delay(duration);
}

void Motor::fwd(int speed)
{
  digitalWrite(pin_in1_, HIGH);
  digitalWrite(pin_in2_, LOW);
  ledcWrite(ledc_channel_, speed);
}

void Motor::rev(int speed)
{
  digitalWrite(pin_in1_, LOW);
  digitalWrite(pin_in2_, HIGH);
  ledcWrite(ledc_channel_, speed);
}

void Motor::brake()
{
  digitalWrite(pin_in1_, HIGH);
  digitalWrite(pin_in2_, HIGH);
  ledcWrite(ledc_channel_,0);
}

void Motor::standby()
{
  digitalWrite(pin_standby_, LOW);
}
