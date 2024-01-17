#include "SingleEMAFilterLib.h"


int maxMeasure [4]= {400, 400, 400, 400}; //array of the max measured values when the flex sensor is straight
int minMeasure [4]= {180, 180, 180, 180}; // min measured values when flex sensors are bent
int prevFlex1Val=-1;

 unsigned long prevTime = 0; //used for flex sensor value updating

/* Flex sensor reading vector definitions
    objects from SingleEMAFilterLib, allows for low pass filtering of analog inputs
*/
SingleEMAFilter<float> flex1(0.3);
SingleEMAFilter<float> flex2(0.3);
SingleEMAFilter<float>  flex3(0.3);
SingleEMAFilter<float>  flex4(0.3);


 SingleEMAFilter<float> flexArray [4] = {flex1, flex2, flex3, flex4}; //handy array to hold the flex sensor vectors
 byte pinArray [4] = {A2, A2, A2, A2}; //handy array for the pins used to access the flex sensors

void initializeFlexSSensors(){
  //Set the maximum and minimum measurements values for each flex sensor, prompt the user

  Serial.println();
  
}


float getMeasure(int i) 
{
  //read and linearize the flex sensor readings
  //(xi – min(x)) / (max(x) – min(x)) * 100
	return ((analogRead(pinArray[i])-minMeasure[i])/(maxMeasure[i]/minMeasure[i]));
}



void setup() {
  #define flex1Pin A2
	Serial.begin(9600);
  pinMode(flex1Pin,INPUT);
	
}



void updateFlexReadings(){
   //add sensor readings to their vectors at set frequency

  int checkPeriod = 1;  //[ms] the frequency that the flex sensor value is updated
  unsigned long curTime = millis();

  if(curTime-prevTime>=checkPeriod){
    prevTime = curTime;
    for (int i = 0; i<5; i++)
    flexArray[i].AddValue(getMeasure(i));
  }

}



void loop()
{  

  updateFlexReadings();
  int temp = flexArray[0].GetLowPass();
  if(temp>100){temp=100;}
  if(temp!=prevFlex1Val){
    prevFlex1Val=temp;
    Serial.print(temp);
    Serial.println();
  }


}
