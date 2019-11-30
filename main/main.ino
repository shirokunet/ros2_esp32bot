#include <ros2arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "esp32_tb6612.hpp"
#include "network_config.h"
#include "ros2esp32bot.hpp"

#define USE_SERIAL_DEBUG

Motor left_wheel = Motor(26, 27, A16, 12, 0, false);
Motor right_wheel = Motor(25, 33, A4, 12, 1, false);
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
  left_wheel.drive(int(msg->linear.x * 255));
  right_wheel.drive(int(msg->linear.y * 255));
}

void updateSensor(geometry_msgs::Vector3* msg){
  static float cnt = 0.0;
  cnt += 0.001;
  msg->x = cnt;
  msg->y = cnt + 1;
  msg->z = cnt + 2;
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