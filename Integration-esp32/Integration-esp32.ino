  #define flex1Pin A14 //13
  #define flex2Pin A17 //27
  #define flex3Pin A18 //25
  #define flex4Pin A14

void setup() {


	Serial.begin(115200);
  wifiSetup();
  pinMode(flex1Pin,INPUT);
  pinMode(flex2Pin,INPUT);
  pinMode(flex3Pin,INPUT);
  //pinMode(flex4Pin,INPUT);
 // analogReadResolution(9);

  long start = millis();// yes this should be unsigned long, but ESP32 doesnt like calculating negative numbers with these
  bool first = true;

  while(!initializeFlexSensors(start, first)){
    first = false;
  }
  hapticSetup();
  peltierSetup();
	
}

void loop()
{  
  //rawDataDemo();
  //fingerDemo();
  //hapticTest();
  //delay(100);
  //Serial.println(analogRead(A7));

  int action = makeGetRequest();
  if (action == 1)
  {
    hapticTest();
    peltierHardOff();
  } 
  else if (action == 2)
  {
    peltierH();
  } else if (action ==3 )
  {
    peltierC();
  }
}
