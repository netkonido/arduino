# Wall follower robot

## Requirements
- The robot must be able to move forward and backwards
- The robot must be able to turn right and turn left
- The robot must follow a wall next to it at a constant, predetermined distance.
- Wall distance must be set by a PID controller

## Physical specifications
- The chassis is made of laser-cut acrylic
- Has 2 driven wheels on the side, and 1 caster wheel in the center towards the rear side. The wheels are driven with a gearbox and a small general-purpose dc motor
- The robot is equipped with 2 ultrasonic distance sensors on the right side, about 15 cm apart, perpendicular to the midline of the robot
- The control circuitry consists of an Arduino Uno with a motor driver shield. The shield can control the 2 motors with pwm forwards and backwards and has the capability for motor braking
- The power is supplied by a 2 cell Lithium-Polymer battery connected to the motor driver shield 

## Wiring
- **Motor driver** is controlled by *(SPI/I2C)* ```pin _``` and ```pin _```
- **Sensor 1** is connected to trig: ```pin _``` and echo: ```pin _```
- **Sensor 2** is connected to trig: ```pin _``` and echo: ```pin _```
- **Right side motor** is connected to **M1** on the driver
- **Left side motor** is connected to **M2** on the driver

## Kinematics
From the distance read by the 2 ultrasonic sensors, we can determine the angle beween the midline of the robot and the wall.

${X = sensor1 - sensor2}$

${\theta = \arctan{\frac{X}{d}}}$  , where d is the distance between the 2 sensors

this angle must be 0, unless the 

from this, the angular velocity can be controlled, and with the forward speed constant, the robot will steer towards or away from the wall, depending on the distance.

${\omega_{R} = \frac{v +\frac{L}{2}\omega}{r}}$

${\omega_{L} = \frac{v -\frac{L}{2}\omega}{r}}$


Since the actual speed of the motor is not known, the relative speed between the motors should be taken. Thus, the equation is modified to use percentages instead of angular velocities.

${p_{steer} = X \times K_p}$

${p_{R} = p_{forward} +p_{steer}}$

${p_{L} = p_{forward} -p_{steer}}$

## Implementation
- The sensors values are noisy, so a way of smoothing must be employed. Ive decided on a **rolling average** smoothing method
- 

## Potential improvements
- The wheels are equipped with rotary pulse generating wheels, they can be used for more precise speed and direction control
- When started with no wall next to it, the robot must move forward until it finds a wall, then follow that wall.
- the robot must be able to recognize and correct motor wire permutation effects.
- add a 3rd ultrasonic sensor facing forward, for collision avoidance.