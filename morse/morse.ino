/*
 * morse.ino
 *
 * Blink a given message onto a pin, presumable holding an LED in series with a resistor.
 */
const unsigned int pin = 8;
const char message[] = "NICK HEARTS MARIE STOP";

/*
 * Morse code
 * NB: delay() takes unsigned long
 */
const unsigned long dotLength_ms = 100;  /* this is the only configurable knob; 50 ms is standard */
const unsigned long dashLength_ms = 3 * dotLength_ms;
const unsigned long blinkSpaceLength_ms = dotLength_ms;
const unsigned long charSpaceLength_ms = 3 * dotLength_ms;
const unsigned long wordSpaceLength_ms = 7 * dotLength_ms;

/* The 3 high bits are length; 5 low bits are dots (0) or dashes (1).
   The LSB is the first symbol, so you read them right to left. */
#define NUM_MORSE_SYMBOLS 36  /* capital letters plus numerals */
const byte morseEncoding[] = {
	B10111111,  /* 0 is ----- */
	B10111110,  /* 1 is .---- */
	B10111100,  /* 2 is ..--- */
	B10111000,  /* 3 is ...-- */
	B10110000,  /* 4 is ....- */
	B10100000,  /* 5 is ..... */
	B10100001,  /* 6 is -.... */
	B10100011,  /* 7 is --... */
	B10100111,  /* 8 is ---.. */
	B10101111,  /* 9 is ----. */
	0,          /* : is invalid */
	0,          /* ; is invalid */
	0,          /* < is invalid */
	0,          /* = is invalid */
	0,          /* > is invalid */
	0,          /* ? is invalid */
	0,          /* @ is invalid */
	B01000010,  /* A is .- */
	B10000001,  /* B is -... */
	B10000101,  /* C is -.-. */
	B01100001,  /* D is -.. */
	B00100000,  /* E is . */
	B10000100,  /* F is ..-. */
	B01100011,  /* G is --. */
	B10000000,  /* H is .... */
	B01000000,  /* I is .. */
	B10001110,  /* J is .--- */
	B01100101,  /* K is -.- */
	B10000010,  /* L is .-.. */
	B01000011,  /* M is -- */
	B01000001,  /* N is -. */
	B01100111,  /* O is --- */
	B10000110,  /* P is .--. */
	B10001011,  /* Q is --.- */
	B01100010,  /* R is .-. */
	B01100000,  /* S is ... */
	B00100001,  /* T is - */
	B01100100,  /* U is ..- */
	B10001000,  /* V is ...- */
	B01100110,  /* W is .-- */
	B10001001,  /* X is -..- */
	B10001101,  /* Y is -.-- */
	B10000011  /* Z is --.. */
};

void dot() {
	digitalWrite(pin, HIGH);
	delay(dotLength_ms);
	digitalWrite(pin, LOW);
}

void dash() {
	digitalWrite(pin, HIGH);
	delay(dashLength_ms);
	digitalWrite(pin, LOW);
}

void setup() {
	digitalWrite(pin, LOW);
	pinMode(pin, OUTPUT);
}

size_t messageIdx = 0;
void advanceChar() {
	messageIdx++;
	if (messageIdx >= strlen(message)) {
		delay(wordSpaceLength_ms);  /* add implicit space before looping */
		messageIdx = 0;
	}
}

/* proceed one character per cycle */
void loop() {
	byte messageChar = message[messageIdx];

	/* special cases */
	if (messageChar == ' ') {  /* space */
		delay(wordSpaceLength_ms - charSpaceLength_ms - blinkSpaceLength_ms);
		advanceChar();
		return;
	} else if (messageChar > '9' && messageChar < 'A') {  /* Invalid char! Skip. */
		advanceChar();
		return;
	}

	/* unpack current character encoding */
	byte encodedChar = morseEncoding[messageChar - '0'];
	byte charNumSymbols = encodedChar >> 5;  /* top three bits */
	byte charSymbols = encodedChar & 0x1F;  /* bottom five bits */ 

	/* iterate over symbols in char frorm right to left */
	while (charNumSymbols--) {
		if (charSymbols & 1) dash();
		else dot();
		delay(blinkSpaceLength_ms);
		charSymbols >>= 1;
	}

	/* terminate char */
	delay(charSpaceLength_ms - blinkSpaceLength_ms);
	advanceChar();
}