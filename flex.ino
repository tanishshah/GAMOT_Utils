#ifndef FLEXSENSOR
#define FLEXSENSOR

#include "SingleEMAFilterLib.h"

/*          - Flex Sensing Code -                                     */
static const byte numFingers = 4;
//handy array for the pins used to access the flex sensors
byte pinArray [numFingers] = {A2, A3, A4, A5}; 
// UPDATE pinArray AS NEEDED



//array of the max measured values when the flex sensor is straight
int maxMeasure [numFingers]= {400, 400, 400, 400}; 

// min measured values when flex sensors are bent
int minMeasure [numFingers]= {180, 180, 180, 180}; 
int prevFlex1Val=-1;

//used for flex sensor value updating
unsigned long prevTime = 0; 

/* Flex sensor reading vector definitions
    objects from SingleEMAFilterLib, allows for low pass filtering of analog inputs
*/
SingleEMAFilter<float> flex1(0.3);
SingleEMAFilter<float> flex2(0.3);
SingleEMAFilter<float> flex3(0.3);
SingleEMAFilter<float> flex4(0.3);

 //handy array to hold the flex sensor vectors
SingleEMAFilter<float> flexArray [numFingers] = {flex1, flex2, flex3, flex4};



//Set the maximum and minimum measurements values for each flex sensor, prompt the user
void initializeFlexSensors(){
  
  unsigned long start = millis();
  
  Serial.println();
  Serial.print("Starting Finger Calibration \nPlease straighten your fingers\n");
  delay(2000);
  for(int i =0; i<numFingers+1; i++){
    maxMeasure[i]=getRawData(i);
    Serial.print("\nMax Value finger: ");
    Serial.println(i);
    Serial.print(maxMeasure[i]);
  }

  delay(2000);
  /*int prevMinMeasure [numFingers];
  memcpy(prevMinMeasure, minMeasure, numFingers);*/
  Serial.print("\nPlease close your hand\n");

  //may want to include check for value changes
  delay(2000);

  for(int i =0; i<numFingers+1; i++){
    minMeasure[i]=getRawData(i);
    Serial.print("\nMax Value finger: ");
    Serial.println(i);
    Serial.print(minMeasure[i]);
  }

  Serial.print("\nCallibration Complete!\n\n");
}

//read the raw data from a finger
float getRawData(byte fingerIndex){
  return (analogRead(pinArray[fingerIndex]));
}

//read and linearize the flex sensor readings
float getMeasure(int i) 
{
  
  //(xi – min(x)) / (max(x) – min(x)) * 100
  int temp = ((getRawData(i)-minMeasure[i])/(maxMeasure[i]/minMeasure[i]));

    //saturate values
    if(temp>100){
      temp=100;
    }else if(temp <0){
      temp=0;
    }

	return temp;
}






//update the finger value array for filtering
void updateFlexReadings(){
   //add sensor readings to their vectors at set frequency

  int checkPeriod = 1;  //[ms] the frequency that the flex sensor value is updated
  unsigned long curTime = millis();

  if(curTime-prevTime>=checkPeriod){
    prevTime = curTime;
    for (int i = 0; i<(numFingers+1); i++){
      flexArray[i].AddValue(getMeasure(i));
    }
    
  }

}

/*
bool fingerClose(byte finger, unsigned int val, bool close){
  //returns true if a finger is sufficiently closed
  updateFlexReadings();
  int reading = flexArray[finger].GetLowPass();
  if (reading<val){
    return true;
  }

  return false;
  
}

bool fingerOpen(byte finger, unsigned int val, bool close){
  //returns true if a finger is sufficiently closed
  updateFlexReadings();
  int reading = flexArray[finger].GetLowPass();
  if (reading>val){
    return true;
  }

  return false;
  
}*/


int prevPosn = -1;


//Generic example which outputs the readings from finger 0
void fingerDemo(){
  updateFlexReadings();
  int temp = flexArray[0].GetLowPass();

  if(temp!=prevFlex1Val&&millis()>3000){
    prevFlex1Val=temp;
    Serial.print(temp);
    Serial.println();
  }

}


#endif