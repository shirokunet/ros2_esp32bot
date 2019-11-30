#ifndef ROS2_ESP32Bot_
#define ROS2_ESP32Bot_

#include <ros2arduino.h>

#define PUBLISH_FREQUENCY_HZ 50
#define QUEUE_SIZE 10


class ROS2ESP32Bot : public ros2::Node{
  public:
    ROS2ESP32Bot();
    static ROS2ESP32Bot* thisPtr;
    QueueHandle_t command_q, status_q;

  private:
    static void subscribeCommand(geometry_msgs::Twist*, void*);
    static void publishChatter(std_msgs::String*, void*);
    static void publishStatus(geometry_msgs::Vector3*, void*);

    geometry_msgs::Vector3 status_msg;
};

#endif