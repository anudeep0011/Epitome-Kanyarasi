# Epitome-Kanyarasi
Griet Hackthon (Epitome)

Platoon System 

Project Overview:

This project implements a platoon-based vehicle system using ESP32, NRF24L01, and ultrasonic sensors. It consists of a Header Vehicle (Leader) that controls the Slave Vehicles (Followers). The Header Vehicle is controlled via a web interface(for prototype), and the Slave Vehicles autonomously follow based on distance measurements using an ultrasonic sensor.

Features:
Header Vehicle (Leader):
Creates a Wi-Fi access point for control via a web interface.
Sends movement commands (Forward, Backward, Left, Right, Stop) to Slave 1 using NRF24L01.
Web-based control with touch-friendly UI.

Slave Vehicles (Followers):
Receive movement commands from the Header Vehicle.
Adjust speed dynamically based on distance from the previous vehicle using an ultrasonic sensor.
Stop automatically if the leader vehicle stops or if there is no signal.


Hardware Components:

ESP32 (for each vehicle)
NRF24L01 Transceiver Module
Ultrasonic Sensor (HC-SR04) (for slave vehicles)
Motor Driver (L298N or L293D)
Motors & Wheels
Power Supply (Battery)

Code Explanation:

Header Vehicle Code (header.ino)
This is the leader vehicle responsible for controlling the slave vehicles through a web interface.

Key Functionalities:

Wi-Fi Access Point: Creates a local network for remote control.
Web Server Interface: Allows movement control via a browser.
NRF24L01 Communication: Sends movement commands to the first slave vehicle.
Motor Control: Controls movement using PWM signals.


Important Functions:

handleRoot() → Serves the web page with control buttons.
handleControl() → Processes movement commands from the web UI.
moveForward() / moveBackward() / turnLeft() / turnRight() / stopCar() → Control motor movement.
sendRadioMessage() → Sends movement commands to Slave 1.


Slave Vehicle Code (slave.ino):

This is the follower vehicle that listens to commands from Slave 1 and follows it while maintaining a safe distance.

Key Functionalities:

NRF24L01 Communication: 
Receives movement commands from Slave 1.
Ultrasonic Sensor Integration: Adjusts speed dynamically to maintain a safe distance.
Motor Control: Moves forward, backward, turns, and stops based on received commands.
Failsafe Mechanism: Stops if no command is received for 5 seconds.

Important Functions:

receiveCommand() → Listens for commands from Slave 1.
handleMovement() → Adjusts speed based on distance.
getDistance() → Measures distance using the ultrasonic sensor.
adaptiveForward() / adaptiveBackward() → Adjusts speed dynamically.
stopCar() → Stops the vehicle if no command is received.

Communication Flow:

The Header Vehicle is controlled via a web interface.
The Header Vehicle sends movement commands via NRF24L01 to Slave 1.
Slave 1 executes the movement and relays the same command to Slave 2.
Slave 2 follows Slave 1 while adjusting its speed using an ultrasonic sensor.
