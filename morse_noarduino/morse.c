/*
 * morse.c
 *
 * Blink a given message onto a pin, presumably holding an LED in series with a resistor.
 *
 * Note that the .c file is an adaptation of the .ino version, designed to avoid
 * Arduino's headers and libraries to cut down on ROM usage.
 */

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

typedef uint16_t size_t;
typedef uint8_t byte;

/* On the Adafruit Trinket, there are few enough pins that there's only one
   pin register. Ref: https://teslaui.wordpress.com/2013/03/26/attiny85-port-registers/ */
#define pin PB0
#define SET(bit) do {PORTB |= (1<<bit);} while (0);
#define CLR(bit) do {PORTB &= ~(1<<bit);} while (0);
#define delay _delay_ms

static char message[] = "TUNA TUNA TUNA HAMACHI TUNA";

/*
 * Morse code
 */
#define dotLength_ms 50  /* this is the only configurable knob; 50 ms is standard */
#define dashLength_ms (3 * dotLength_ms)
#define blinkSpaceLength_ms dotLength_ms
#define charSpaceLength_ms (3 * dotLength_ms)
#define wordSpaceLength_ms (7 * dotLength_ms)

/* The 3 high bits are length; 5 low bits are dots (0) or dashes (1).
   The LSB is the first symbol, so you read them right to left. */
#define NUM_MORSE_SYMBOLS 36  /* capital letters plus numerals */
static byte morseEncoding[] = {
	0b10111111,  /* 0 is ----- */
	0b10111110,  /* 1 is .---- */
	0b10111100,  /* 2 is ..--- */
	0b10111000,  /* 3 is ...-- */
	0b10110000,  /* 4 is ....- */
	0b10100000,  /* 5 is ..... */
	0b10100001,  /* 6 is -.... */
	0b10100011,  /* 7 is --... */
	0b10100111,  /* 8 is ---.. */
	0b10101111,  /* 9 is ----. */
	0,           /* : is invalid */
	0,           /* ; is invalid */
	0,           /* < is invalid */
	0,           /* = is invalid */
	0,           /* > is invalid */
	0,           /* ? is invalid */
	0,           /* @ is invalid */
	0b01000010,  /* A is .- */
	0b10000001,  /* B is -... */
	0b10000101,  /* C is -.-. */
	0b01100001,  /* D is -.. */
	0b00100000,  /* E is . */
	0b10000100,  /* F is ..-. */
	0b01100011,  /* G is --. */
	0b10000000,  /* H is .... */
	0b01000000,  /* I is .. */
	0b10001110,  /* J is .--- */
	0b01100101,  /* K is -.- */
	0b10000010,  /* L is .-.. */
	0b01000011,  /* M is -- */
	0b01000001,  /* N is -. */
	0b01100111,  /* O is --- */
	0b10000110,  /* P is .--. */
	0b10001011,  /* Q is --.- */
	0b01100010,  /* R is .-. */
	0b01100000,  /* S is ... */
	0b00100001,  /* T is - */
	0b01100100,  /* U is ..- */
	0b10001000,  /* V is ...- */
	0b01100110,  /* W is .-- */
	0b10001001,  /* X is -..- */
	0b10001101,  /* Y is -.-- */
	0b10000011   /* Z is --.. */
};

static void dot() {
	SET(pin);
	delay(dotLength_ms);
	CLR(pin);
}

static void dash() {
	SET(pin);
	delay(dashLength_ms);
	CLR(pin);
}

static size_t messageIdx = 0;
static inline void advanceChar() {
	if (++messageIdx >= strlen(message)) {
		delay(wordSpaceLength_ms);  /* add implicit space before looping */
		messageIdx = 0;
	}
}

static inline void setup() {
	CLR(pin);
	DDRB |= 1 << pin; // set pin to output
}

/* proceed one character per cycle */
static inline void loop() {
	byte encodedChar;
	byte charNumSymbols;

	/* read next char */
	char messageChar = message[messageIdx];

	/* handle special cases and validate input */
	if (messageChar == ' ') {  /* space */
		delay(wordSpaceLength_ms - charSpaceLength_ms - blinkSpaceLength_ms);
		advanceChar();
		return;
	} else if (messageChar < '0' ||
		       (messageChar > '9' && messageChar < 'A') ||
		       messageChar > 'Z') {  /* Invalid char! Skip. */
		advanceChar();
		return;
	}

	/* unpack current character encoding */
	encodedChar = morseEncoding[messageChar - '0'];
	charNumSymbols = encodedChar >> 5;  /* top three bits */

	/* iterate over symbols in char frorm right to left */
	while (charNumSymbols--) {
		if (encodedChar & 1) dash();
		else dot();
		delay(blinkSpaceLength_ms);
		encodedChar >>= 1;
	}

	/* terminate char */
	delay(charSpaceLength_ms - blinkSpaceLength_ms);
	advanceChar();
}

int main(void) {
	setup();
	while (1) {
		loop();
	}
	return 0;
}