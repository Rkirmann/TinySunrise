#include <tinysnore.h>

#define in A1
#define out 0
#define out2 1
int msDeley = 200;
int sensorValue = 0;
int ambientLight = 0;

// signal length in ms
const int signalLenHigh = 300;
const int signalLenLow = 150;

// 0 - dark , 1 - light
int signalStateLast = 0;
int signalStateCurrent = 0;

long int signalLastTime = 0;
long int signalLen = 0;
String inByte = "";
String recieved = "";

boolean alarm = false;
long timer;
long duration;

unsigned long timerMillis = 0;
unsigned long snoreMillis = 0;

float R;
// the setup routine runs once when you press reset:
void setup() {
    // Serial.begin(9600);
    // initialize the digital pin as an output.
    pinMode(in, INPUT);
    pinMode(out, OUTPUT);
    pinMode(out2, OUTPUT);
    //wakeup fade in 
    R = (255 * log10(2))/(log10(255));

    for (int i = 0; i < 100; i++) {
        // ambientLight = (ambientLight * 0.9) + (analogRead(in) * 0.1);
        int light = analogRead(in);
        if (light > ambientLight) ambientLight = light;
        delay(10);
    }
    ambientLight *= 1.1;

    // Serial.println(ambientLight);

    // ambientLight = max(500, ambientLight);
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
    if (millis() + snoreMillis - timerMillis >= timer && alarm) 
        wakeUP();

    sensorValue = analogRead(in);
    // Serial.println(sensorValue);

    if (sensorValue >= ambientLight) {
        signalStateCurrent = 1;
        digitalWrite(out2, HIGH);
    } else {
        signalStateCurrent = 0;
        digitalWrite(out2, LOW);
    }

    if (signalStateCurrent == 0 && signalStateLast == 0 &&
        signalLen > signalLenHigh * 3) {
        //timerMillis += millis();
        snore(500);
        snoreMillis += 500;
    }
  

    if (signalStateCurrent == signalStateLast) {
        signalLen = millis() - signalLastTime;
    } else {
        signalLen = millis() - signalLastTime;
        // signal change
        char cBit = getBinChar();
        if (cBit != '-') {
            inByte += cBit;
        } else {
            ;
        }
        signalLastTime = millis();
        signalStateLast = signalStateCurrent;
    }
    // message recieved
    if (inByte.length() == 8) {
        const char *c = inByte.c_str();
        recieved += (char)strtol(c, NULL, 2);
        inByte = "";
        // Serial.println("recieved "+recieved);
        if (recieved[recieved.length() - 1] == 'f') decode();
    }

}  // end loop

void decode() {
    recieved.remove(recieved.length() - 1, 1);
    String x = getValue(recieved, 't', 0);
    String y = getValue(recieved, 't', 1);
    recieved = "";
    // Serial.println("x "+x);
    // Serial.println("y "+y);

    timer = x.toInt();
    duration = y.toInt();
    // Serial.println(timer);
    // Serial.println(duration);

    if (timer > 0) startTimer();
}
String getValue(String data, char separator, int index) {
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void startTimer() {
    alarm = true;
    timer = timer * 60000;
    duration = duration * 60000;
    timerMillis = millis();
    for (int fadeValue = 255; fadeValue > 0; fadeValue--) {
        // sets the value (range from 0 to 255):
        analogWrite(out, fadeValue);
        analogWrite(out2, fadeValue);
        // wait for x milliseconds to see the dimming effect
        delay(10);
        //timerMillis += 10;
    }
}

void wakeUP() {
  timer = 86400000;
  timerMillis = millis();
  int brightness = 0;
  unsigned long delayIncrement = duration / 255;
    // fade in from min to max in increments of 5 points:
    for (int fadeValue = 0; fadeValue <= 255; fadeValue++) {
        // sets the value (range from 0 to 255):
        brightness = pow (2, (fadeValue / R)) - 1;
        analogWrite(out, brightness);
        analogWrite(out2, brightness);
        // wait for x milliseconds to see the dimming effect
        delay(delayIncrement);
        //timerMillis += delayIncrement;

    }
    digitalWrite(out, LOW);
    digitalWrite(out2, LOW);
    
    
}
