# ROS2_ESP32Bot

![](https://lh3.googleusercontent.com/_o8eYCek_wowWPqUyYcPlmgHZn2wdGjSYvckrcphoX2S8vozcttgKcpMYjJiTP1c8CLWG5Jg9PYTrsglpluo8gLecdKokHnP2urr8xbRPvxMWnNKt2HccLtOOUYphPJwwXdrCl_9xA)
* [YouTube](https://www.youtube.com/watch?v=GBBcU8ohSVk)


## Dependency
- ROS2 Dashing Diademata
- ros2arduino 0.1.3
- Micro-XRCE-DDS Agent 1.1.0


## Prepare Hardware
### Build a your own bot
![](https://lh3.googleusercontent.com/9dfobosXR7634ZKSsShJRUWD1uWRhtWJeT9GQ9RfxwCcWsK9J1JGwb6dlRg55y0pI-ZnMj4JBSxdQmKjgZJorxQf4oPs5a-GDD7l-d0fWv8GaHBYe6hCg3x1gJmyMPq3uXyvJGptEA)

I bought below parts.
- ¥1,480 [ESP32-DevKitC-32D](http://akizukidenshi.com/catalog/g/gM-13628/)
- ¥350 [AE-TB6612](http://akizukidenshi.com/catalog/g/gK-11219/)
- ¥90 [AZ0526](http://akizukidenshi.com/catalog/g/gP-04303/)


## Setup dependency
### Setup Micro-XRCE-DDS Agent
You need to [setup Micro-XRCE-DDS Agent](https://github.com/ROBOTIS-GIT/ros2arduino#version-specific-dependencies) for using ros2arduino.

### Arduino ESP32
If you have not setup arduino environment yet, you can check the instraction at [here](https://github.com/espressif/arduino-esp32#installation-instructions)


## Getting started
Make sure you need an access point.
```
$ git clone https://github.com/shirokunet/ros2_esp32bot
$ cd ros2_esp32bot/main/
$ cp network_config_templrate.h network_config.h
$ vi network_config.h
#define SSID       ""
#define SSID_PW    ""
#define AGENT_IP   ""
#define AGENT_PORT 2018
```
And write the .ino file into your ESP32.


## Run ROS2 nodes for sending commands
### Run MicroXRCEAgent
```
$ MicroXRCEAgent udp -p 2018 -v 6
```

### Run joy_node
```
$ ros2 run joy joy_node
```

### Run teleop_node
```
$ ros2 run teleop_twist_joy teleop_node
```

### Useful commands for debugging
```
$ ros2 node list
$ ros2 topic list
$ ros2 topic echo /esp32bot/status
$ ros2 topic echo /esp32bot/chatter
$ ros2 topic echo /joy
$ ros2 topic echo /cmd_vel
```


## teleop_twist_joy for Logicool Gamepad F310
If you do not have a PS4 controller, you can buy F310 and use my repository. 
- [shirokunet/teleop_twist_joy](https://github.com/shirokunet/teleop_twist_joy)
