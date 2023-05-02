/*
PhoneRobot by Brian "24 Hour Engineer" McEvoy
https://www.24hourengineer.com/

// Neopixel code from Adafruit (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        11

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 12

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Declare inputs
const int rightPotentiometer = A1;
const int leftPotentiometer = A0;
const int rightServo = 2;
const int leftServo = 3;
const int upPushbutton = 8;
const int downPushbutton = 7;
const int leftPushbutton = 4;
const int rightPushbutton = 5;
const int circlePushbutton = 6;
const int squarePushbutton = 9;
const int yellowPushbutton = 10;
const int touchEmulator = 12;

int battlePosition[2][5] = {
  {1500,1400,1450,1500,1600}, 
  {1700,1550,1500,1550,1550}};
const int curveballLocations[2][4] = {{1145, 1145, 1280, 1280}, {1860, 1700, 1750, 1900}};
const int potentiometerMinimum = 800;
const int potentiometerMaximum = 2200;
bool verboseFeedback = true;
const int servoSpeed = 10;
const int totalModes = 6;
int currentMode = 0;
bool modeChange = false;
const int maxBrightness = 2;
bool touchEmulatorState = false;
const int repeatInterval = 40;
long fastRepeatTimer = 0;
bool battleBoolean = false;
int battlePhase = 0;
int battleCounter =0;
const int leftServoMin = 1400;
const int leftServoMax = 1600;
const int rightServoMin = 1450;
const int rightServoMax = 1700;
int funLeftServoRandom = (leftServoMin + leftServoMax)/2;
int funRightServoRandom = (rightServoMin + rightServoMax)/2;

void setup() {
  pinMode(leftServo, OUTPUT);
  pinMode(rightServo, OUTPUT);
  pinMode(leftPotentiometer, INPUT);
  pinMode(rightPotentiometer, INPUT);
  pinMode(upPushbutton, INPUT_PULLUP);
  pinMode(downPushbutton, INPUT_PULLUP);
  pinMode(leftPushbutton, INPUT_PULLUP);
  pinMode(rightPushbutton, INPUT_PULLUP);
  pinMode(circlePushbutton, INPUT_PULLUP);
  pinMode(squarePushbutton, INPUT_PULLUP);
  pinMode(yellowPushbutton, INPUT_PULLUP);
  pinMode(touchEmulator, OUTPUT);
  
  Serial.begin(115200);
  
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'
  pixelStart();
  singleWhitePixel(currentMode);
}

void loop() {
  scanInputs();
  readPushbuttons(4, 10);
  if(currentMode == 0){
    freeMovementMode();
  }
  if(currentMode == 1){
    basicMode();
  }
  if(currentMode == 2){
    battleMode();
  }
  if(currentMode == 3){
    raidMode();
  }
  if(currentMode == 4){
    prandomMovementMode();
  }
  
//  if (verboseFeedback){
//    Serial.print("\n");
//  }
  delay(15);
}

void battleMode(){
  int battleSpeed = analogRead(leftPotentiometer);
  battleSpeed = 34 - (battleSpeed / 32);
  if(!digitalRead(yellowPushbutton)){
    battleBoolean = !battleBoolean;
    delay(100);
    battlePhase = 0;
    if(verboseFeedback && battleBoolean){
      Serial.print("Battle start\n");
    }
    if(battleBoolean){
      singleGreenPixel(currentMode);
    }else{
      singleWhitePixel(currentMode);
    }
  }
  if(millis() > fastRepeatTimer){
    touchEmulatorState = !touchEmulatorState;
    digitalWrite(touchEmulator, touchEmulatorState);
    fastRepeatTimer = millis() + repeatInterval;
    battleCounter++;
  }
  if(battleBoolean && (battleCounter >= battleSpeed)){
    battleCounter = 0;
    pixels.setPixelColor(11-battlePhase, pixels.Color(0, 0, 0));
    battlePhase++;
    if(battlePhase >= 5){
      battlePhase = 0;
    }
    pixels.setPixelColor(11-battlePhase, pixels.Color(0, 0, maxBrightness));
    pixels.show();
  }
  if(battleBoolean){
    if(verboseFeedback){
      Serial.print("Battle phase: ");
      Serial.print(battlePhase);
      Serial.print("\tServo0: ");
      Serial.print(battlePosition[0][battlePhase]);
      Serial.print("\tServo1: ");
      Serial.print(battlePosition[1][battlePhase]);
      Serial.print("\n");
    }
    digitalWrite(leftServo, HIGH);
    delayMicroseconds(battlePosition[0][battlePhase]);
    digitalWrite(leftServo, LOW);
    digitalWrite(rightServo, HIGH);
    delayMicroseconds(battlePosition[1][battlePhase]);
    digitalWrite(rightServo, LOW);
  }
}

void raidMode(){
  int raidSpeed = analogRead(leftPotentiometer);
  raidSpeed = 34 - (raidSpeed / 32);
  if(!digitalRead(yellowPushbutton)){
    battleBoolean = !battleBoolean;
    delay(100);
    battlePhase = 0;
    if(verboseFeedback && battleBoolean){
      Serial.print("Battle start\n");
    }
    if(battleBoolean){
      singleGreenPixel(currentMode);
    }else{
      singleWhitePixel(currentMode);
    }
  }
  if(millis() > fastRepeatTimer){
    touchEmulatorState = !touchEmulatorState;
    digitalWrite(touchEmulator, touchEmulatorState);
    fastRepeatTimer = millis() + repeatInterval;
    battleCounter++;
  }
  if(battleBoolean && (battleCounter >= raidSpeed)){
    battleCounter = 0;
    pixels.setPixelColor(11-battlePhase, pixels.Color(0, 0, 0));
    battlePhase++;
    if(battlePhase >= 3){
      battlePhase = 0;
    }
    pixels.setPixelColor(11-battlePhase, pixels.Color(0, 0, maxBrightness));
    pixels.show();
  }
  if(battleBoolean){
    if(verboseFeedback){
      Serial.print("Battle phase: ");
      Serial.print(battlePhase);
      Serial.print("\tServo0: ");
      Serial.print(battlePosition[0][battlePhase+2]);
      Serial.print("\tServo1: ");
      Serial.print(battlePosition[1][battlePhase+2]);
      Serial.print("\n");
    }
    digitalWrite(leftServo, HIGH);
    delayMicroseconds(battlePosition[0][battlePhase+2]);
    digitalWrite(leftServo, LOW);
    digitalWrite(rightServo, HIGH);
    delayMicroseconds(battlePosition[1][battlePhase+2]);
    digitalWrite(rightServo, LOW);
  }
}

void prandomMovementMode(){
  int prandomPotSpeed = analogRead(leftPotentiometer);
  prandomPotSpeed = map(prandomPotSpeed, 0, 1023, 50, 1000);
  if(millis() % prandomPotSpeed < 15){
    funLeftServoRandom = random(leftServoMin, leftServoMax);
    funRightServoRandom = random(rightServoMin, rightServoMax);
  }
  if(verboseFeedback){
    Serial.print("Left prandom: ");
    Serial.print(funLeftServoRandom);
    Serial.print("\tRight prandom: ");
    Serial.print(funRightServoRandom);
    Serial.print("\n");
  }
  digitalWrite(leftServo, HIGH);
  delayMicroseconds(funLeftServoRandom);
  digitalWrite(leftServo, LOW);
  digitalWrite(rightServo, HIGH);
  delayMicroseconds(funRightServoRandom);
  digitalWrite(rightServo, LOW);
}

void freeMovementMode(){
  touchEmulationButtons();
}

void basicMode(){
  int funLeftPosition = basicControl(leftPotentiometer, leftServo);
  int funRightPosition = basicControl(rightPotentiometer, rightServo);
  if(verboseFeedback){
    Serial.print("\tPosition total: ");
    Serial.print(funLeftPosition + funRightPosition);
    Serial.print("\n");
  }
  touchEmulationButtons();
}

void touchEmulationButtons(){
  if(!digitalRead(upPushbutton) || !digitalRead(circlePushbutton) || !digitalRead(downPushbutton) || !digitalRead(squarePushbutton)){
    if(!digitalRead(upPushbutton)){
      if(millis() > fastRepeatTimer){
        touchEmulatorState = !touchEmulatorState;
        digitalWrite(touchEmulator, touchEmulatorState);
        fastRepeatTimer = millis() + repeatInterval;
      }
    }
    if(!digitalRead(circlePushbutton)){
      if(millis() > fastRepeatTimer){
        touchEmulatorState = !touchEmulatorState;
        digitalWrite(touchEmulator, touchEmulatorState);
        fastRepeatTimer = millis() + repeatInterval * 2;
      }
    }
    if(!digitalRead(downPushbutton)){
      if(millis() > fastRepeatTimer){
        touchEmulatorState = !touchEmulatorState;
        digitalWrite(touchEmulator, touchEmulatorState);
        fastRepeatTimer = millis() + repeatInterval * 3;
      }
    }
    
    if(!digitalRead(squarePushbutton)){
      touchEmulatorState = HIGH;
      digitalWrite(touchEmulator, touchEmulatorState);
    }
  }else{
    digitalWrite(touchEmulator, LOW);
  }
}

void curveballMode(){
  if(!digitalRead(yellowPushbutton)){
    digitalWrite(touchEmulator, HIGH);
    int curveballSpeed = analogRead(leftPotentiometer);
    curveballSpeed = map(curveballSpeed, 0, 1023, 30, 2);
    chargeCurveball(curveballSpeed);
    if(verboseFeedback){
      Serial.print("Speed: ");
      Serial.print(curveballSpeed);
      Serial.print("\n");
    }
  }else{
    digitalWrite(touchEmulator, LOW);
  }
}

void scanInputs(){
  
  if(!digitalRead(leftPushbutton)){
    currentMode--;
    modeChange = true;
  }
  if(!digitalRead(rightPushbutton)){
    currentMode++;
    modeChange = true;
  }
  if(modeChange){
    battleBoolean = false;
    modeChange = false;
    if(currentMode>=totalModes){
      currentMode = 0;
    }
    if(currentMode<0){
      currentMode = totalModes-1;
    }
    if(verboseFeedback){
      Serial.print("Mode: ");
      Serial.print(currentMode);
      Serial.print("\n");
    }
    singleWhitePixel(currentMode);
    delay(500);
  }
}

void chargeCurveball(int funServoSpeed){
  for(int i=0; i<4; i++){
    for(int k=0; k<funServoSpeed; k++){
      digitalWrite(leftServo, HIGH);
      delayMicroseconds(curveballLocations[0][i]);
      digitalWrite(leftServo, LOW);
      digitalWrite(rightServo, HIGH);
      delayMicroseconds(curveballLocations[1][i]);
      digitalWrite(rightServo, LOW);
      delay(15);
    }
  }
}

void readPushbuttons(int funLowerInput, int funUpperInput){
  for(int i=funLowerInput; i <=funUpperInput; i++){
    if(!digitalRead(i)){
      Serial.print(i);
      Serial.print("\t");
    }
  }
}

int basicControl(int funPot, int funServo){
  int funPotPos = analogRead(funPot);
  funPotPos = map(funPotPos, 0, 1024, potentiometerMaximum, potentiometerMinimum);
  digitalWrite(funServo, HIGH);
  delayMicroseconds(funPotPos);
  digitalWrite(funServo, LOW);
  if (verboseFeedback){
    Serial.print(funPot);
    Serial.print(":\t");
    Serial.print(funPotPos);
    Serial.print("\t");
  }
  return funPotPos;
}

void pixelStart(){
  // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(maxBrightness, 0, 0));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(50);
  }
  
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0, maxBrightness, 0));
    pixels.show();
    delay(50);
  }
  
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, maxBrightness));
    pixels.show();
    delay(50);
  }
  
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
    delay(50);
  }
}

void singleWhitePixel(int funSinglePixel){
    pixels.clear();
    pixels.setPixelColor(funSinglePixel, pixels.Color(maxBrightness, maxBrightness, maxBrightness));
    pixels.show();
}

void singleGreenPixel(int funSinglePixel){
    pixels.clear();
    pixels.setPixelColor(funSinglePixel, pixels.Color(0, maxBrightness, 0));
    pixels.show();
}
