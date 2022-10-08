# trialproj-auto-adjusting-gimbal
My submission for the SJSU robotics control systems team trial project.

This repository contains my code for the Arduino IDE. It retrieves data from the MPU6050 accelerometer registers to sense gimbal movement from three axes.
After running, the positional information is printed through the serial monitor. This information is then given to a servo with an arm that will always
point in a user-specified direction (through the serial monitor) within 180 degrees on a 2D plane.

The arduino uno board is able to communicate with the MPU6050 through the I2C protocol.
