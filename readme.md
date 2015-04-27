Arduino projects
================

This repository has my amateur attempts at playing with Arduinos and their kin.
All were developed with Makefile-based build-and-flash in mind:

* `blink`: This is the most basic blink example.
* `morse`: Blink an arbitrary message in morse code.
* `morse_noarduino`: Same as morse, but use bit-banging instead of Arduino
  headers or libraries. This results in a substantially lower ROM footprint.
  This project has its own Makefile to invoke `avr-gcc`, `avrdude`, and friends.
  Only tested on an Adafruit Trinket (ATtiny85).
* `range`: Bit-banging approach to using a HC-SR04 ultrasonic range-finder.
