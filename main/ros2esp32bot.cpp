#include "ros2esp32bot.hpp"


ROS2ESP32Bot::ROS2ESP32Bot():Node("esp32bot"){
  thisPtr = this;
  command_q = xQueueCreate(QUEUE_SIZE, sizeof(geometry_msgs::Twist));
  status_q = xQueueCreate(QUEUE_SIZE, sizeof(geometry_msgs::Vector3));

  /* Publisher */
  ros2::Publisher<std_msgs::String>* publisher_chatter = this->createPublisher<std_msgs::String>("esp32bot/chatter");
  this->createWallFreq(PUBLISH_FREQUENCY_HZ, (ros2::CallbackFunc)this->publishChatter, nullptr, publisher_chatter);

  ros2::Publisher<geometry_msgs::Vector3>* publisher_status = this->createPublisher<geometry_msgs::Vector3>("esp32bot/status");
  this->createWallFreq(PUBLISH_FREQUENCY_HZ, (ros2::CallbackFunc)this->publishStatus, nullptr, publisher_status);

  /* Subscriber */
  this->createSubscriber<geometry_msgs::Twist>("cmd_vel",(ros2::CallbackFunc)this->subscribeCommand, nullptr);
}

void ROS2ESP32Bot::subscribeCommand(geometry_msgs::Twist* msg, void* arg) {
  (void)(arg);
  static ROS2ESP32Bot *_this = ROS2ESP32Bot::thisPtr;
  xQueueSend(_this->command_q, msg, 0);
}

void ROS2ESP32Bot::publishChatter(std_msgs::String* msg, void* arg){
  (void)(arg);
  static int cnt = 0;
  sprintf(msg->data, "Hello from esp32bot %d", cnt++);
}

void ROS2ESP32Bot::publishStatus(geometry_msgs::Vector3* msg, void* arg){
  (void)(arg);
  static ROS2ESP32Bot *_this = ROS2ESP32Bot::thisPtr;

  if(xQueueReceive(_this->status_q, &_this->status_msg, 0) == pdTRUE){
    msg->x = _this->status_msg.x;
    msg->y = _this->status_msg.y;
    msg->z = _this->status_msg.z;
  }
}