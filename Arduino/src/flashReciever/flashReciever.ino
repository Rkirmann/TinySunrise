#define in A1
#define out 1
int msDeley = 200;
int sensorValue = 0;
int ambientLight = 0;

const boolean debug = true;
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
String lcdLine = "";
// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(in, INPUT);
  pinMode(out, OUTPUT);
  //ambientLight = analogRead(in);

  for (int i = 0; i < 100; i++) {
    //ambientLight = (ambientLight * 0.9) + (analogRead(in) * 0.1);
    ambientLight +=  analogRead(in);
    delay(10);
  }
  ambientLight = ambientLight/100*1.75;
  //ambientLight = max(500, ambientLight);
}

char getBinChar() {
  if (signalStateLast == 1) {
    // ignore light pulse
    return '-';
  }

  if (signalLen >= signalLenLow * 0.3 &&  signalLen <= signalLenLow * 1.5) {
    //digitalWrite(out, 0);
    return '0';
  } else if (signalLen >= signalLenHigh * 0.8 &&  signalLen <= signalLenHigh * 2) {
    //digitalWrite(out, 1);
    return '1';
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


// the loop routine runs over and over again forever:
void loop() {
  sensorValue = analogRead(in);

  
  if (sensorValue >= ambientLight ) {
    signalStateCurrent = 1;
    //digitalWrite(out, HIGH);
  } else {
    signalStateCurrent = 0;
    //digitalWrite(out, HIGH);
  }

  if (signalStateCurrent == signalStateLast)
    signalLen = millis() - signalLastTime; 
  else {
    signalLen = millis() - signalLastTime;
    // signal change
    char cBit = getBinChar();

    if (cBit != '-') {
      //digitalWrite(out, 1);
      inByte += cBit;
    } else {
      ;
      //digitalWrite(out, 0);
    }
    signalLastTime = millis();
    signalStateLast = signalStateCurrent;
  }

  if (inByte.length() == 8) {
    const char * c = inByte.c_str();
    recieved += (char) strtol(c, NULL, 2);
    if (recieved == "t")
      digitalWrite(out, HIGH);
    inByte = "";
  }

  //delay(1000);
}
