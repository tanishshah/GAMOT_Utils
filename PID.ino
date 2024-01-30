
#include "PIDController.h"
PIDController pid;

//setup function
void PID_setup(){
  pid.begin();          // initialize the PID instance
 // pid.setpoint(50);    // The "goal" the PID controller tries to "reach"
  //pid.tune(10, 2, 8);    // Tune the PID, arguments: kP, kI, kD
  pid.tune(300,1,60);
  pid.limit(0, 255);    // Limit the PID output between 0 and 255, this is important to get rid of integral windup!

}

//set the desired value
void set_PID(int val){
  pid.setpoint(val);
}

//change the PID ouput based on the new input value
int update_PID_output(int val){

  int temp = pid.compute(val);

//saturators
  if (temp>254){
    temp = 254;
  }else if(temp<0){
    temp = 0;
  }

  return temp;

}