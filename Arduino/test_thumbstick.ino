#include <ezButton.h>
#include <math.h>

#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin
#define SW_PIN   2  // Arduino pin connected to SW  pin
#define NEUTRAL_POSITION_UPPER_BOUND 350 
#define NEUTRAL_POSITION_LOWER_BOUND 300

ezButton button(SW_PIN);

int valueX = 0; // To store value of the X axis
int valueY = 0; // To store value of the Y axis
int rawValueX = 0;
int rawValueY = 0;
int avgOffsetX = 0, avgOffsetY = 0;
int avgForwardX = 0, avgForwardY = 0;
float forwardAngle = 0;
// offset applied to ALL angles
float angleOffset = 0;
float angle = 0;
float radius = 0;
int bValue = 0; // To store value of the button
bool calibrationComplete = false;

unsigned long currentTime; // To store start of timer for calibrate
unsigned long startTime; // To store end of timer for calibrate
const unsigned long holdTimeForCalibrate = 3000;  //the value is a number of milliseconds

void debug() {

  // read analog X and Y analog values
  valueX = analogRead(VRX_PIN) - avgOffsetX;
  valueY = analogRead(VRY_PIN) - avgOffsetY;

  // Read the button value
  //bValue = button.getState();

  /*if (digitalRead(SW_PIN) == 0) {
    Serial.println("The button is pressed");
    // TODO do something here
  }

  if (digitalRead(SW_PIN) == 1) {
    Serial.println("The button is released");
    // TODO do something here
  }*/

  // print data to Serial Monitor on Arduino IDE
  // Serial.print("x = ");
  // Serial.print(valueX);
  // Serial.print(", y = ");
  // Serial.println(valueY);
  /*Serial.print(" : button = ");
  Serial.println(bValue);*/
  // delay(200);
  // Boundary for Neutral position using radius of polar coordinates
  radius = sqrt(valueX*valueX + valueY*valueY);
  // Old cartesion check
  // if(abs(valueX) <= 15 && abs(valueY) <= 15) 
  if(radius <= sqrt(15*15 + 15*15)){
    Serial.println("NEUTRAL");
  }
  else{
    angle = atan2(valueY, valueX) * 180 / PI + 180 - angleOffset;
    if(angle < 0) {
      angle += 360;
    }
    if(angle > 360) {
      angle -= 360;
    }
    Serial.print("ANGLE: ");
    Serial.print(angle);
    if(angle > 0 && angle < 90) {
      Serial.println(" -> FORWARD");
    }
    if(angle > 90 && angle < 180) {
      Serial.println(" -> RIGHT");
    }
    if(angle > 180 && angle < 270) {
      Serial.println(" -> BACKWARD");
    }
    if(angle > 270 && angle < 360) {
      Serial.println(" -> LEFT");
    }
  }
  delay(200);
}

void checkCalibrate() {
  startTime = millis();
  currentTime = millis();
  rawValueX = analogRead(VRX_PIN);
  rawValueY = analogRead(VRY_PIN);
  if(rawValueX < NEUTRAL_POSITION_UPPER_BOUND && rawValueX > NEUTRAL_POSITION_LOWER_BOUND && rawValueY < NEUTRAL_POSITION_UPPER_BOUND && rawValueY > NEUTRAL_POSITION_LOWER_BOUND){
    while(digitalRead(SW_PIN) == 0){
      currentTime = millis();
      if(currentTime - startTime == 900) { Serial.println("3..."); }
      if(currentTime - startTime == 1800) { Serial.println("2..."); }
      if(currentTime - startTime == 2700) { Serial.println("1..."); }
      if(currentTime - startTime >= holdTimeForCalibrate) {
        calibrationComplete = false;
        Serial.println("CALIBRATION MODE BEGIN, LIFT THUMB OFF JOYSTICK");
        delay(500);
        if(digitalRead(SW_PIN) == 1) {
          calibrateNeutral();
        }
      }
    }
  }
}

void calibrateNeutral() {
  int samples = 0;
  unsigned long summationX = 0, summationY = 0;
  startTime = millis();
  currentTime = millis();
  rawValueX = analogRead(VRX_PIN);
  rawValueY = analogRead(VRY_PIN);
  while(rawValueX > NEUTRAL_POSITION_UPPER_BOUND || rawValueX < NEUTRAL_POSITION_LOWER_BOUND || rawValueY > NEUTRAL_POSITION_UPPER_BOUND || rawValueY < NEUTRAL_POSITION_LOWER_BOUND)
  {
    rawValueX = analogRead(VRX_PIN);
    rawValueY = analogRead(VRY_PIN);
    Serial.println("PLEASE LET GO OF THE JOYSTICK");
    delay(500);
  }
  Serial.println("START NEUTRAL POSITION CALIBRATION");
  delay(1000);
  while(currentTime - startTime < 2000){
    currentTime = millis();
    summationX += analogRead(VRX_PIN);
    summationY += analogRead(VRY_PIN);
    samples++;
  }
  /*
  Serial.println(samples);
  Serial.println(summationX);
  Serial.print("AVERAGE X NEUTRAL: ");*/
  avgOffsetX = summationX/samples;
  //Serial.println(avgOffsetX);
  /*Serial.println(summationY);
  Serial.print("AVERAGE Y NEUTRAL: ");*/
  avgOffsetY = summationY/samples;
  //Serial.println(avgOffsetY);
  
  Serial.println("NEAUTRAL POSITION DONE");
  calibrateForward();
}

void calibrateForward() {
  valueX = analogRead(VRX_PIN) - avgOffsetX;
  valueY = analogRead(VRY_PIN) - avgOffsetY;
  // Old cartesion check
  // while(abs(valueX) <= 100 && abs(valueY) <= 100)
  // polar coordinate check
  while(sqrt(valueX*valueX + valueY*valueY) < sqrt(100*100 + 100*100))
  {
    valueX = analogRead(VRX_PIN) - avgOffsetX;
    valueY = analogRead(VRY_PIN) - avgOffsetY;
    Serial.println("PLEASE MOVE FORWARD ON JOYSTICK");
    delay(500);
  }
  int samples = 0;
  unsigned long summationX = 0, summationY = 0;
  startTime = millis();
  currentTime = millis();
  Serial.println("START FORWARD CALIBRATION, HOLD STILL");
  delay(1000);
  while(currentTime - startTime < 3000){
    currentTime = millis();
    summationX += analogRead(VRX_PIN);
    summationY += analogRead(VRY_PIN);
    samples++;
  }

  /*Serial.println(samples);
  Serial.println(summationX);
  Serial.print("AVERAGE X FORWARD: ");*/
  avgForwardX = summationX/samples - avgOffsetX;
  /*Serial.println(avgForwardX);
  Serial.println(summationY);
  Serial.print("AVERAGE Y FORWARD: ");*/
  avgForwardY = summationY/samples - avgOffsetY;
  /*Serial.println(avgForwardY);
  Serial.print("ACTUAL FORWARD ANGLE: ");*/
  forwardAngle = atan2(avgForwardY, avgForwardX) * 180 / PI + 180;
  /*Serial.println(forwardAngle);
  Serial.print("FORWARD ANGLE OFFSET: ");*/
  angleOffset = forwardAngle - 45;
  //Serial.println(angleOffset);
  Serial.println("FORWARD POSITION DONE, CALIBRATION COMPLETE!");
  calibrationComplete = true;
}
 
void setup() {
  Serial.begin(9600) ;
}

void loop() {
  checkCalibrate();
  if(calibrationComplete) { debug(); }
}
