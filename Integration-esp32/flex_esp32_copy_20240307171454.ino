#ifndef FLEXSENSOR
#define FLEXSENSOR

  #define flex1Pin A4
  #define flex2Pin A5
  #define flex3Pin A17
  #define flex4Pin A18

#include "SingleEMAFilterLib.h"
#include<string>
/*          - Flex Sensing Code -                                     */
static const byte numFingers = 4;
//handy array for the pins used to access the flex sensors
byte pinArray [numFingers] = {flex1Pin, flex2Pin, flex3Pin, flex4Pin}; 
// UPDATE pinArray AS NEEDED

//array of the max measured values when the flex sensor is straight
int maxMeasure [numFingers]= {400, 400, 400, 400}; 

// min measured values when flex sensors are bent
int minMeasure [numFingers]= {180, 180, 180, 180}; 
int prevFlexVal[numFingers]={-1, -1, -1, -1};

//used for flex sensor value updating
unsigned long prevTime = 0; 

/* Flex sensor reading vector definitions
    objects from SingleEMAFilterLib, allows for low pass filtering of analog inputs
    Set to 0.3 for arduino UNO
*/
SingleEMAFilter<float> flex1(0.3);
SingleEMAFilter<float> flex2(0.3);
SingleEMAFilter<float> flex3(0.3);
SingleEMAFilter<float> flex4(0.3);

 //handy array to hold the flex sensor vectors
SingleEMAFilter<float> flexArray [numFingers] = {flex1, flex2, flex3, flex4};

bool next1 = false;
bool next2 = false;
bool next3 = false;

//Set the maximum and minimum measurements values for each flex sensor, prompt the user

bool calibrationErrorFlag = false;
bool initializeFlexSensors( long start, bool first){
  bool complete = false;
  
  //unsigned long start = millis();
  long curTime = millis();
  if (true){
      Serial.println();
      Serial.print("Starting Finger Calibration \nPlease straighten your fingers\n");
  }
delay(1000);
  if(true/*(curTime-start>5000)&&(curTime-start<7000)&& !next1*/){
    for(int i =0; i<numFingers; i++){
      //Divide by 0 protection
      
        maxMeasure[i]=getRawData(i);
      
      
     /* Serial.print("\nMax Value finger: ");
      Serial.println(i+1);
      Serial.print(maxMeasure[i]);*/
    }
    bool next1 = true;
  }
  //delay(5000);


  delay(2000);
  if(true/*(curTime-start>7000)&&((curTime-start<13000))&&!next2*/){
      Serial.print("\nPlease close your hand\n");
      next2 = true;
  }
  /*int prevMinMeasure [numFingers];
  memcpy(prevMinMeasure, minMeasure, numFingers);*/

  delay(5000);
  if(true/*(curTime-start>13000)&&!next3*/){
      for(int i =0; i<numFingers; i++){
        //divide by 0 protection
        if(getRawData(i)!=maxMeasure[i]){
          //Uncomment below to actually read inputs
          minMeasure[i]=getRawData(i);
        }else{

          Serial.print("\n\n\nCALIBRATION ERROR\n\n\n");
          calibrationErrorFlag=true;
          minMeasure[i]=maxMeasure[i]-1;
        }
      
      /*Serial.print("\nMin Value finger: ");
      Serial.println(i+1);
      Serial.print(minMeasure[i]);*/
    }

    Serial.print("\nCallibration Complete!\n\n");
    complete=true;
    next3 = true;
  }

  return complete;
}

//read the raw data from a finger
float getRawData(byte fingerIndex){
  return (analogRead(pinArray[fingerIndex]));
}

//read and linearize the flex sensor readings
float getMeasure(int i) 
{
  
  //(xi – min(x)) / (max(x) – min(x)) * 100
  int temp = ((getRawData(i)-minMeasure[i])/(maxMeasure[i]-minMeasure[i]))*1000;

    //saturate values
    if(temp>1000){
      temp=1000;
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
     /* Serial.print("Here ");
      Serial.print(i);
      Serial.print(": of");
      Serial.print(numFingers+1);
      Serial.print("\n");*/
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


void rawDataDemo(){
  for(int i=0;i<numFingers;i++){
    Serial.print(getRawData(i));
    Serial.print("   ");
  }
  Serial.println();
}

int getReading(int index){
  int temp = flexArray[index].GetLowPass();

    if(temp>1000){
      temp=1000;
    }else if(temp <0){
      temp=0;
    }
    return temp;
}

int prevPosn = -1;

//Generic example which outputs the readings from finger 0

void fingerDemo(){
  int temp[4];
  updateFlexReadings();

  int avg = 0;
  for(int i = 0; i<numFingers; i++){
    temp[i] = getReading(i);

    if(temp[i]!=prevFlexVal[i] && millis()>3000){
      prevFlexVal[i]=temp[i];
      for(int j = 0; j<numFingers; j++){
        Serial.print(j+1);
        Serial.print(": ");
        Serial.print(temp[j]);
        avg+=temp[j]/numFingers;
        Serial.print("     ");
        if(j==3){
          Serial.print("\n");
          if(avg<550){
            hapticTest();
            peltierPulseCold(); // uncomment this to enable peltier in the demo
          }
        }
      }
    }
  }
}

#endif