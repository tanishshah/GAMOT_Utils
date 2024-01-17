#ifndef flexSensor.h
#define flexSensor.h

#include "SingleEMAFilterLib.h"

class FlexSensor : public SingleEMAFilter {

  private:
int maxMeasure;
int minMeasure;
int prevFlex1Val;
unsigned long prevTime; //used for flex sensor value updating


SingleEMAFilter<float> flex1(const double alpha);


float getMeasure();

void updateFlexReadings();

};


#endif