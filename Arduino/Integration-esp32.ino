  #define flex1Pin A0
  #define flex2Pin A3
  #define flex3Pin A6
  #define flex4Pin A7
void setup() {


	Serial.begin(115200);
  pinMode(flex1Pin,INPUT);
  pinMode(flex2Pin,INPUT);
  pinMode(flex3Pin,INPUT);
  pinMode(flex4Pin,INPUT);
 // analogReadResolution(9);

  long start = millis();// yes this should be unsigned long, but ESP32 doesnt like calculating negative numbers with these
  bool first = true;

  while(!initializeFlexSensors(start, first)){
    first = false;
  }
	
}

void loop()
{  
//rawDataDemo();
  //fingerDemo();
  delay(100);
  Serial.println(analogRead(A7));

}
