#define IA 32
#define IB 33

// TODO: change isOn to an integer state to better allow for hot and cold
unsigned long peltPrevTime = 0;
bool peltIsOn = 0;

void peltierSetup(){
  pinMode(IA, OUTPUT);
  pinMode(IB, OUTPUT);

}

void peltierHot(int duration){
  unsigned long curTime = millis();
  if(curTime-peltPrevTime>duration && !peltIsOn){
    digitalWrite(IA, LOW);
    digitalWrite(IB, HIGH);
    Serial.println("HOT");
    peltPrevTime = curTime;
    peltIsOn = true;
  }
}

void peltierOff(int duration){
  unsigned long curTime = millis();
  if(curTime-peltPrevTime > duration && peltIsOn) {
    digitalWrite(IA, LOW);
    digitalWrite(IB, LOW);
    peltIsOn = false;
    peltPrevTime = curTime;
    Serial.println("OFF");
  }
}

void peltierCold(int duration){
  unsigned long curTime = millis();
  if(curTime-peltPrevTime>duration && !peltIsOn){
    digitalWrite(IA, HIGH);
    digitalWrite(IB, LOW);
    Serial.println("COLD");
    peltPrevTime = curTime;
    peltIsOn = true;
  }
}

void peltierPulseHot()
{
  peltierHot(3000);
  peltierOff(5500);
}

void peltierPulseCold()
{
  peltierCold(8000);
  peltierOff(4000);
}

/*
Isolated use case to test the peltier code

void setup()
{
  Serial.begin(115200);
  peltierSetup();
}
void loop() {
  peltierHot(3000);
  peltierOff(5500); // 5 second difference
}
*/
