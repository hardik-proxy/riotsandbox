# SAUL to CoAP
This document describes the way, RIOT automatically exports SAUL devices via CoAP.

## Resource types


## Resources

saul/
+-- [0-9]+/
|   +-- val
|   +-- conf


### saul/

Search query options:
- rt: we define our own namespace here for now:

saul
+-- .any
+-- .sense
|   +-- .any
|   +-- .btn
|   +-- .temp
|   +-- .hum
|   +-- .light
|   +-- .accel
|   +-- .mag
|   +-- .gyro
|   +-- .color
|   +-- .press
|   +-- .analog
+-- .act
|   +-- .any
|   +-- led_rgb
|   +-- servo
|   +-- motor
|   +-- switch
|   +-- dimmer
