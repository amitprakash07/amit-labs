
#include "engine_math.h"
#include "vector3.h"

using namespace amit::maths;

// Given segment ab and point c, computes closest point d on ab.
// Also returns t for the position of d, d(t) = a + t*(b - a)
void maths::ClosestPointFromPointOnLineSegment(Vector3 c, Vector3 a, Vector3 b, float& t, Vector3& d)
{
    Vector3 ab = b - a;
    // Project c onto ab, computing parameterized position d(t) = a + t*(b – a)
    t = (c - a).Dot(ab) / ab.Dot(ab);
    // If outside segment, clamp t (and therefore d) to the closest endpoint
    if (t < 0.0f)
        t = 0.0f;
    if (t > 1.0f)
        t = 1.0f;
    // Compute projected position from the clamped t
    d = a + ab * t;
}

// Returns the squared distance between point c and segment ab
float math::SqDistPointSegment(Vector3 a, Vector3 b, Vector3 c)
{
    Vector3 ab = b - a;
    Vector3 ac = c - a;
    Vector3 bc = c - b;
    float   e  = ac.Dot(ab);
    // Handle cases where c projects outside ab
    if (e <= 0.0f)
        return ac.Dot(ac);
    float f = ab.Dot(ab);
    if (e >= f)
        return bc.Dot(bc);
    // Handle cases where c projects onto ab
    return ac.Dot(ac) - e * e / f;
}

float math::ConvertDegreesToRadians(const float i_degrees)
{
    return i_degrees * kPi / 180.0f;
}

bool math::InRange(float valToCheck, float min, float max, bool minInclusive, bool maxInclusive)
{
    if (minInclusive && maxInclusive)
        return (valToCheck >= min && valToCheck <= max) ? true : false;
    else if (minInclusive)
        return (valToCheck >= min && valToCheck < max) ? true : false;
    else
        return (valToCheck > min && valToCheck <= max) ? true : false;
}

float math::scaleAndBias(float valueToConvert, float oldRangeMin, float oldRangeMax, float newRangeMin, float newRangeMax)
{
    /*
  Scale and Bias Operation Formulae
  old			New
  [a,b]		[c,d]
  newValue = (oldValue - oldRangeMin) * (ratio of new range difference to old
  range difference) + new Range Minimum newVal = ((oldVal - a) * ((d-c)/(b-a)))
  + c
  */
    float ratioToMaintain = (newRangeMax - newRangeMin) / (oldRangeMax - oldRangeMin);
    return (((valueToConvert - oldRangeMin) * ratioToMaintain) + newRangeMin);
}
