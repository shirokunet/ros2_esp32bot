#include <ros2arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "esp32_tb6612.hpp"
#include "network_config.h"
#include "ros2esp32bot.hpp"

#define USE_SERIAL_DEBUG

Motor left_wheel = Motor(27, 26, A16, 12, 0, true);
Motor right_wheel = Motor(33, 25, A4, 12, 1, true);
Motor vehicle_arm = Motor(5, 18, A11, 19, 2, true);

ROS2ESP32Bot* ros2esp32bot_node;
ROS2ESP32Bot* ROS2ESP32Bot::thisPtr;
TaskHandle_t th[2];
WiFiUDP udp;


void setup() {
#if defined(USE_SERIAL_DEBUG)
  Serial.begin(115200);
#endif

  WiFi.begin(SSID, SSID_PW);
  while(WiFi.status() != WL_CONNECTED);

  ros2::init(&udp, AGENT_IP, AGENT_PORT);
  ros2esp32bot_node = new ROS2ESP32Bot();

  xTaskCreatePinnedToCore(updateBot, "update_bot", 4096, NULL, 1, &th[1], 1);
}

void loop() {
  ros2::spin(ros2esp32bot_node);
}

void updateBot(void *pvParameters){
  while(1)
  {
    static ROS2ESP32Bot*_this = ROS2ESP32Bot::thisPtr;
    static geometry_msgs::Twist msg_rx;
    static geometry_msgs::Vector3 msg_tx;

    updateActuator(&msg_rx);
    updateSensor(&msg_tx);

    if(xQueueReceive(_this->command_q, &msg_rx, 0) == pdTRUE){
      // do something
    }
    xQueueSend(_this->status_q, &msg_tx, 0);

#if defined(USE_SERIAL_DEBUG)
    outputSerial(&msg_rx, &msg_tx);
#endif

    delay(10);
  }
}

void updateActuator(geometry_msgs::Twist* msg){
  const float tread_m = 0.125;
  const float tire_radius_m = 0.025;
  const float linear_gain = 0.0235;
  const float angular_gain = 0.392;
  const float arm_gain = 2.0;

  float desired_vel_l = (msg->linear.x * linear_gain / tire_radius_m)
                          - (tread_m / (2.0 * tire_radius_m )) * msg->angular.z * angular_gain;
  float desired_vel_r = (msg->linear.x * linear_gain/ tire_radius_m)
                          + (tread_m / (2.0 * tire_radius_m )) * msg->angular.z * angular_gain;
  float desired_vel_arm = msg->linear.z * arm_gain;

#if defined(USE_SERIAL_DEBUG)
  Serial.print("motor: ");
  Serial.print(desired_vel_l);
  Serial.print(", ");
  Serial.print(desired_vel_r);
  Serial.print(", ");
  Serial.print(desired_vel_arm);
  Serial.print(", ");
#endif

  left_wheel.drive(desired_vel_l);
  right_wheel.drive(desired_vel_r);
  vehicle_arm.drive(desired_vel_arm);
}

void updateSensor(geometry_msgs::Vector3* msg){
  static float cnt = 0.0;
  cnt += 0.001;
  msg->x = cnt;
  msg->y = cnt + 1;
  msg->z = cnt + 2;
  if (cnt >= 1000.0) cnt = 0.0;
}

#if defined(USE_SERIAL_DEBUG)
void outputSerial(geometry_msgs::Twist* msg_rx, geometry_msgs::Vector3* msg_tx){
  Serial.print("rx: ");
  Serial.print(msg_rx->linear.x);
  Serial.print(", ");
  Serial.print(msg_rx->linear.y);
  Serial.print(", ");
  Serial.print(msg_rx->linear.z);
  Serial.print(", ");
  Serial.print("tx: ");
  Serial.print(msg_tx->x);
  Serial.print(", ");
  Serial.print(msg_tx->y);
  Serial.print(", ");
  Serial.print(msg_tx->z);
  Serial.print("\n");
}
#endif