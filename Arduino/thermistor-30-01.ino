#ifndef THERMISTOR
#define THERMISTOR
#define THERM_PIN A0

#include "SingleEMAFilterLib.h"



//SingleEMAFilter<float> temperature(1);
SingleEMAFilter<int> A_Reading(0.4);
unsigned long prevTime = 0;


float TEMP=0;

void therm_setup(){
  pinMode(THERM_PIN, INPUT);

}

//Converts resistance to deg C
float calcTemp(int ohm){
//should use steinhart & hart formula but couldn't decipher data sheet. This includes less computations and R^2 = 0.9989 from -5 -> 45 [deg]

  float A = -26.4;
  float B = 268.89;

  return (A*log(ohm)+B);
}

  
//updates the thermistor resistance reading
void updateOhmReading(){

  unsigned long curTime = 10*millis();
  unsigned long checkPeriod = 1; //[ms]

  if(curTime-prevTime>=checkPeriod){
    prevTime=curTime;
    A_Reading.AddValue(analogRead(THERM_PIN));
  }

}



//calculate the thermistor resistance
float calcThermOhm(){
  const float R1 = 10000;
  const float MaxADC = 1023; //CHANGE FOR ESP32
  float Vo;

  
  //Vo = analogRead(THERM_PIN);
  Vo=A_Reading.GetLowPass();

  return R1 * (MaxADC / (float)Vo - 1.0);

}

//returns temperature reading
float getTemperature(){
  return calcTemp(calcThermOhm());
}


  float prevReading=-1;

  //generic example function, prints temperature readings
void temperatureExample(){
  analogWrite(8, 255);

  updateOhmReading();
  float T = getTemperature();
  
  if((prevReading!=T)){ //avoids redundant temperature prints

    prevReading=T;
    Serial.print("Temperature: "); 
    Serial.print(T);
    Serial.println(" C"); 

  }
  //T=temperature.GetLowPass();



}

int iteration = 0;
int  prevResistance = -1;

void resistance_test(){
  int PWM = 0;
  int resistance =0;
  
  int difference = 5;
  

  
      analogWrite(8, 17*iteration);
;

      updateOhmReading();
      resistance = calcThermOhm();

    /* Serial.print("reading: ");
      Serial.print(resistance);
      Serial.println();

       Serial.print("Prev reading: ");
      Serial.print(prevResistance);
      Serial.println();
*/
      if(abs(resistance-prevResistance)<difference){//stable temperature
        Serial.print("Resistance: ");
        Serial.print(resistance);
        Serial.print("\n");
        Serial.print("PWM: ");
       Serial.print(17*iteration);
        Serial.println();

        iteration++;

      }

    prevResistance = resistance;

    delay(15000);
    if(iteration>15){
      iteration =0;
    }


}

void setup(){
  Serial.begin(9600);
  therm_setup();
  pinMode(8, OUTPUT);
  PID_setup();
  set_PID(40);
}

void loop(){
  temperatureExample();
  

  //int val = update_PID_output(getTemperature());

  /*
  Serial.print("val: ");
  Serial.println(val);

  analogWrite(8, val);*/

  //resistance_test();

 // Serial.print("here");
   

}

#endif
