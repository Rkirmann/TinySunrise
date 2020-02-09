# flashCom2
 Fork of https://github.com/cyhex/flashCom
 
 
 This is a project for a wearable device using only LEDs for receiving data to set a timer for a fade effect sunrise alarm.
 The repository includes an android app that sends data and an Arduino sketch for ATtiny85 to decode the light flashes and set a linear fade in effect at the given time.
 Without an accurate external clock, transmission speed can be low: currently around 6bps, also to keep accurate time for longer periods. 
 
 Some sources:
 [Tests of LED and Various Light Sensors](https://www.instructables.com/id/Tests-of-LEDs-and-Various-Light-Sensors/)  
 [PWM Exponential LED Fading](https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms)  
 [Timed deep sleep for Attiny](https://github.com/connornishijima/TinySnore)  
 [Internal battery level reading](https://github.com/cano64/ArduinoSystemStatus)  
 [Power saving techniques for microprocessors](https://www.gammon.com.au/forum/?id=11497)  



![PCB](https://i.ibb.co/zmmqdgV/PCB-PCB-2020-01-27-19-44-28-20200131153600.png)  
PCB design using a coin cell battery, ATtiny85, 8MHz crystal and 3 leds to capture the light signal.


