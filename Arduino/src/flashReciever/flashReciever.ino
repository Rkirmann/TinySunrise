#include <tinysnore.h>
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define in A1
#define out 0
#define out2 1
int msDeley = 200;
int sensorValue = 0;
int ambientLight = 0;

// signal length in ms
const int signalLenHigh = 320;
const int signalLenLow = 160;

// 0 - dark , 1 - light
int signalStateLast = 0;
int signalStateCurrent = 0;

long int signalLastTime = 0;
long int signalLen = 0;
String inByte = "";
String recieved = "";

unsigned long timer;
float minutes;
boolean startTimer = false;
boolean firstMessage = true;

// the setup routine runs once when you press reset:
void setup() {
    // initialize the digital pin as an output.
    pinMode(in, INPUT);
    pinMode(out, OUTPUT);
    pinMode(out2, OUTPUT);
    // ambientLight = analogRead(in);

    for (int i = 0; i < 100; i++) {
        // ambientLight = (ambientLight * 0.9) + (analogRead(in) * 0.1);
        ambientLight += analogRead(in);
        delay(10);
    }
    ambientLight = ambientLight / 100 * 1.75;
    // ambientLight = max(500, ambientLight);

    attachInterrupt(0, loop, RISING);
}

void wakeUp() {
    sbi(ADCSRA, ADEN);  // switch Analog to Digitalconverter ON
    sleep_disable();  // System continues execution here when watchdog timed out
    loop();
}

char getBinChar() {
    if (signalStateLast == 1) {
        // ignore light pulse
        return '-';
    }
    if (signalLen >= signalLenLow * 0.3 && signalLen <= signalLenLow * 1.5) {
        // digitalWrite(out, 0);
        return '0';
    } else if (signalLen >= signalLenHigh * 0.8 &&
               signalLen <= signalLenHigh * 2) {
        // digitalWrite(out, 1);
        return '1';
        // end of message
    } else if (signalLen > signalLenHigh * 3) {
        // long  sig - reset
        recieved = "";
        inByte = "";
        return '-';
    } else {
        // ignore short signals
        return '-';
    }
}

void loop() {
    sensorValue = analogRead(in);

    if (sensorValue >= ambientLight) {
        signalStateCurrent = 1;
        // digitalWrite(out, HIGH);
    } else {
        signalStateCurrent = 0;
        // digitalWrite(out, HIGH);
    }

    if (signalStateCurrent == signalStateLast)
        signalLen = millis() - signalLastTime;
    else {
        signalLen = millis() - signalLastTime;
        // signal change
        char cBit = getBinChar();

        if (cBit != '-') {
            // digitalWrite(out, 1);
            inByte += cBit;
        } else {
            ;
            // digitalWrite(out, 0);
        }
        signalLastTime = millis();
        signalStateLast = signalStateCurrent;
    }

    // message recieved
    if (inByte.length() == 8 && firstMessage) {
        const char *c = inByte.c_str();
        recieved += (char)strtol(c, NULL, 2);
        // for testing
        // if (recieved == "1") digitalWrite(out, HIGH);
        inByte = "";
        int numbers = recieved.toInt();
        timer = numbers;
        firstMessage = false;
    } else if (inByte.length() == 8 && !firstMessage) {
        const char *c = inByte.c_str();
        recieved += (char)strtol(c, NULL, 2);
        inByte = "";
        int numbers = recieved.toInt();
        minutes = float(numbers);
        firstMessage = true;
        startTimer = true;
    }

    // snore for given time
    if (startTimer) {
        snore(timer * 60000);
        // fade in from min to max in increments of 5 points:
        for (int fadeValue = 0; fadeValue <= 255; fadeValue++) {
            // sets the value (range from 0 to 255):
            analogWrite(out, fadeValue);
            analogWrite(out2, fadeValue);
            // wait for x milliseconds to see the dimming effect
            delay(minutes / 255 * 60000);
        }
        startTimer = false;
    }
}
