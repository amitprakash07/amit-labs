#include "utils.h"

float amit::maths::ToRadians(const float i_degrees)
{
    return i_degrees * amit::maths::kPi / 180.0f;
}

bool amit::maths::InRange(float valToCheck, float min, float max, bool minInclusive, bool maxInclusive)
{
    if (minInclusive && maxInclusive)
        return (valToCheck >= min && valToCheck <= max) ? true : false;
    else if (minInclusive)
        return (valToCheck >= min && valToCheck < max) ? true : false;
    else
        return (valToCheck > min && valToCheck <= max) ? true : false;
}

float amit::maths::scaleAndBias(float valueToConvert,
                                float oldRangeMin,
                                float oldRangeMax,
                                float newRangeMin,
                                float newRangeMax)
{
    /*
  Scale and Bias Operation Formulae
  old		New
  [a,b]		[c,d]
  newValue = (oldValue - oldRangeMin) * (ratio of new range difference to old
  range difference) + new Range Minimum newVal = ((oldVal - a) * ((d-c)/(b-a)))
  + c
  */
    float ratioToMaintain = (newRangeMax - newRangeMin) / (oldRangeMax - oldRangeMin);
    return (((valueToConvert - oldRangeMin) * ratioToMaintain) + newRangeMin);
}