# Camera Operated Gate Opening IoT System
## 1. Product Definition
### 1.1 Summary

 The goal of the project is to allow a person to cross through a gate without needing to touch
anything to pass through. The gate opening and closing will be decided by authorized
personnel viewing a camera stream on a site where the personnel will input a lock or unlock
command and initiate the actual mechanism. This system creates a safe environment for
animals and personnel by decreasing the surface contact of an area with high traffic and high
risk of cross contamination. 

### 1.2 Users
The user would be any business or home that requires a high level of security or as stated before a laboratory that needs to keep an area clean to avoid contamination.

### 1.3 Interfaces
The user who is trying to get through the gate simply needs to pass by the PIR sensor so that
it detects movement. From there the person viewing the camera can decide to let the user in
by simply typing “lock” or “unlock” into the submission field of the site the streaming is
being hosted on.

## 2. Project Definition
### 2.1 Summary
The embedded circuit is made up of a dc motor, a servo, two PIR sensors, and an LED
matrix which is all connected to a microcontroller. The LED matrix simply plays an
animation to let the person know the system is idle and hasn’t detected movement yet. The
PIR sensors detect the movement. One of them is to notify the camera operator that a person
is waiting for entry and the other is to let the operator know that the person has passed
through the gate. The servo serves as a lock and unlock mechanism while the motor opens
and closes the gate. The microcontroller is connected to a Raspberry Pi board via a couple
GPIO pins. The Raspberry Pi has a camera connected to it which does the actual streaming.
The microcontroller sends a signal to the Raspberry Pi to start or stop streaming and the
Raspberry Pi sends a signal back to start the lock or unlock sequence.

### 2.2 Constraints and Limitations
For an actual gate, a large stepper motor would most likely be used compared to a simple
DC motor. I don’t have a stepper motor and have never used one, so the DC motor was used
instead. I would have like to be able to use two cameras so that the gate could operate from
both sides and so that the camera operator can see everything.

### 2.3 Specifications
Metric Number | Need Number | Metric | Units | Marginal Value | Ideal Value
--- | --- | --- | --- | --- | ---
1 | 5 | Low Stream Output Latency | ms | <50ms | <3ms
2 | 5 | Low User Input Latency | ms | <10ms | <5ms 
3 | 4 | Compact Design | cm | <30cm | <15cm

### 2.4 Prototype Costs
Quantity | Description| Unit Cost
--- | --- | --- 
1 | Tiva C Series TM4C123G LaunchPad Evaluation Kit | $22.60
2 | PIR Motion Sensor | $.99
1 | Raspberry Pi 4 Model B | $55
1 | 5V Servo Motor | $11.99
1 | 5V DC Motor | $14.36 
1 | 8x8 LED Matrix | $1.99
| | | Total Cost: $106.93

## 3. Project Design
### 3.1 Software Components
Embedded C script for controlling the locking and unlocking mechanism and a Python
script for creating a server and streaming the footage to a web page.
### 3.2 Hardware Components
A servo, DC motor, LED matrix, a camera, and 2 PIR sensors
