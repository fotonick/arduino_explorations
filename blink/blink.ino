const unsigned int pin = 8;
unsigned long interval = 1000;
unsigned long previousTime_ms = 0;
int ledStateHigh = 0;

void toggleLED() {
	if (ledStateHigh) {
		digitalWrite(pin, LOW);
		ledStateHigh = 0;
	}
	else {
		digitalWrite(pin, HIGH);
		ledStateHigh = 1;
	}
}

void setup() {
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

void loop() {
	unsigned long now_ms = millis();

	if (now_ms - previousTime_ms >= interval) {
		toggleLED();
		previousTime_ms = now_ms;
	}
}