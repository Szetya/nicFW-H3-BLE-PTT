Implementation of BLE PTT button for nicFW H3 radio with ESP32.

A simple implementation. The push button currently has 3 functions. The code uses the nicFW remote protocol to manage the buttons.
- Press and hold PTT to double click
- Release PTT with a single click
- activate and deactivate flashlight with a long press
Currently I have not created a configuration header file to define ESP pins. The default BOOT pushbutton for ESP32 is GPIO0.

For safety reasons, the PTT start is double pressed on the ESP32. This avoids accidental transmission. 
It also has a safety TOT function which currently lasts 2 minutes. If the ESP32 is used to start the transmission, it will automatically release the PTT button after this time.
The time for this function is :
const unsigned long totMaxTime = 2 * 60 * 1000; // 2 minutes (ms)
line in the main.cpp.

In several places the comments are written in Hungarian. Sorry but it was easier for me. :)
