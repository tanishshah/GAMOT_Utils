#define IA 13
#define IB 14

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

void peltierH(){
  unsigned long curTime = millis();
  digitalWrite(IA, LOW);
  digitalWrite(IB, HIGH);
  Serial.println("HOT");
  peltPrevTime = curTime;
  delay(5500);
  digitalWrite(IA, LOW);
  digitalWrite(IB, LOW);
}

void peltierC(){
  unsigned long curTime = millis();
  digitalWrite(IA, HIGH);
  digitalWrite(IB, LOW);
  Serial.println("COLD");
  peltPrevTime = curTime;
  delay(5500);
  digitalWrite(IA, LOW);
  digitalWrite(IB, LOW);
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

void peltierHardOff()
{
    unsigned long curTime = millis();
    if(curTime-peltPrevTime>2000) 
    {
      digitalWrite(IA, LOW);
      digitalWrite(IB, LOW);
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

void peltierPulseCold()
{
  peltierCold(4000);
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
