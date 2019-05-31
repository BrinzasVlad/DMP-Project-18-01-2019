# Wi-Fi Controlled Robot using Arduino

This was a project for my 3rd year, 2nd semester subject "Design with microprocessors" at the Technical University of Cluj Napoca.

The given task was to use the university-assembled robot (a platform with two fixed wheels connected to a DC motor each, with room for two AAA batteries and equipped with an ultrasonic sensor at the front), together with Arduino boards (I used an Arduino Mega and an Arduino Uno) to create a robot that is remote-controlled via Wi-Fi (for example, using the mobile phone) and that automatically stops before running into obstacles.

The project is distributed as follows:
<ul>
  <li><i>DMP Documentation.pdf</i> briefly describes the modules that make up the robot, as well as how to operate it</li>
  <li><i>WiFiMasterTest.ino</i> contains the Arduino code for the Arduino Mega board, which is connected to the Wi-Fi module and receives the user's commands</li>
  <li><i>SlaveMotorsUltrasonicTest.ino</i> contains the Arduino code for the Arduino Uno board, which receives the user commands from the Arduino Mega via I2C communication and activates the correct motors accordingly, while monitoring the ultrasonic sensor for imminent obstacles</li>
</ul>
