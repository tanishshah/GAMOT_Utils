#include <Wire.h>
#include "Adafruit_DRV2605.h"


Adafruit_DRV2605 drv;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  hapticSetup();
}

void hapticSetup(){
    while (! drv.begin()) {
    Serial.println("Error: Could not find DRV2605");
    
  }

  drv.selectLibrary(1);
  
  // I2C trigger by sending 'go' command 
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG); 

  //vectorSetup();
  

}


//Vibrates the specified pattern. Input an int coresponding to the vibration pattern numbers from section 11.2 - https://cdn-shop.adafruit.com/datasheets/DRV2605.pdf
void vibrate(int effect){
  drv.setWaveform(0, effect);
  drv.setWaveform(1, 0);
  drv.go();
}

//Vibrates in the instructed sequence. Input an array of ints corresponding to the vibration pattern numbers from section 11.2 - https://cdn-shop.adafruit.com/datasheets/DRV2605.pdf 
void vibrate(uint8_t effect[]){
  int length = sizeof(effect)/sizeof(effect[0])+1;

//board can only store 7 sequences. This removes any extra sequences.
  if (length>7){
    length=7;
  }

//set the waveform into the buffer
  for(int i =0; i<length; i++){
    drv.setWaveform(i,effect[i]);
    if(i==length-1){
      drv.setWaveform(i+1,0);//end waveform
    }
  }


  // play the effect!
  drv.go();

}



void hapticTest(){
  uint8_t testArray[2]={15,71};
  vibrate(testArray);
  delay(2000);
}



void loop() {
  // put your main code here, to run repeatedly:
hapticTest();

}
