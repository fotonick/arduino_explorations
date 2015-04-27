/*
 * Taken from http://www.xappsoftware.com/wordpress/2012/03/15/how-to-interface-the-hc-sr04-ultrasonic-ranging-module-to-arduino/
 * as a start.
 */

#include <Arduino.h>
#include <HardwareSerial.h>

#define ECHOPIN 8        // Pin to receive echo pulse
#define TRIGPIN 4        // Pin to send trigger pulse
#define timeout_us 38000UL


#define SPEED_OF_SOUND_MM_US 0.340  // speed of sound in mm per microsecond

static uint8_t echoBit, trigBit;
static volatile uint8_t *echoRegister;
static volatile uint8_t *trigRegister;

static unsigned long pingTime_us() {
  unsigned long start, maxTime_us;
  unsigned long beforePulseStart, beforePulseEnd;

  /* paranoid check that pin is off */
  *trigRegister &= ~trigBit;
  delayMicroseconds(2);

  /* fire missiles! */
  *trigRegister |= trigBit;
  delayMicroseconds(10);
  *trigRegister &= ~trigBit;
  start = micros();
  maxTime_us = start + timeout_us;

  /* find start of pulse */
  do {
    beforePulseStart = micros();
    if (beforePulseStart > maxTime_us) {
      return 0;
    }
  } while (!(*echoRegister & echoBit));

  /* found start; now find end */
  do {
    beforePulseEnd = micros();
    if (beforePulseEnd > maxTime_us) {
      return 0;
    }
  } while (*echoRegister & echoBit);

  return beforePulseEnd - beforePulseStart;
  // return pulseIn(ECHOPIN, HIGH);
}

void setup() {
  /* Let's set register bits directly for minimal overhead and more accurate timings. */
  echoBit = digitalPinToBitMask(ECHOPIN);
  trigBit = digitalPinToBitMask(TRIGPIN);
  echoRegister = portInputRegister(digitalPinToPort(ECHOPIN));
  trigRegister = portOutputRegister(digitalPinToPort(TRIGPIN));

  /* set i/o mode */
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);

  /* turn trig pin off */
  digitalWrite(TRIGPIN, LOW);

  /* turn on pull-up resistor for echo pin; so that it's on by default */
  digitalWrite(ECHOPIN, HIGH);

  Serial.begin(9600);
}

void loop() {
  // Compute distance
  unsigned long timeDelay_us = pingTime_us();
  Serial.println(0.5 * SPEED_OF_SOUND_MM_US * timeDelay_us);
  delay(200);
}