  #define flex1Pin A4 //13
  #define flex2Pin A5 //27
  #define flex3Pin A17 //25
  #define flex4Pin A18

const int LED_BUILTIN = 2;
void setup() {


	Serial.begin(115200);
  wifiSetup();
  pinMode(flex1Pin,INPUT);
  pinMode(flex2Pin,INPUT);
  pinMode(flex3Pin,INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(flex4Pin,INPUT);
 // analogReadResolution(9);

  long start = millis();// yes this should be unsigned long, but ESP32 doesnt like calculating negative numbers with these
  bool first = true;

  while(!initializeFlexSensors(start, first)){
    first = false;
  }
  hapticSetup();
  peltierSetup();
	digitalWrite(LED_BUILTIN, HIGH);
}

int osc_data[6] = {0,0,0,0, 500, 0};

void updateFings()
{
  updateFlexReadings();
  for (int i = 0; i<4; i++)
  {
    osc_data[i] = getReading(i);
  }
}
void loop()
{  
  updateFings();
  sendSigState(osc_data);
  int action = makeGetRequest();
  if (action == 1)
  {
    updateFings();
    osc_data[5] = 1000;
    sendSigState(osc_data);
    hapticTest();
    peltierHardOff();
    osc_data[5] = 0;
    sendSigState(osc_data);
  } 
  else if (action == 2)
  {
    updateFings();
    osc_data[4] = 0;
    sendSigState(osc_data);
    peltierH();
    osc_data[4] = 500;
    sendSigState(osc_data);
  } else if (action ==3 )
  {
    updateFings();
    osc_data[4] = 1000;
    sendSigState(osc_data);
    peltierC();
    osc_data[4] = 500;
    sendSigState(osc_data);
  }
  else {
    updateFings();
    sendSigState(osc_data);
  }
}
