#ifndef CORE_MATHS_LINEAR_ALGEBRA_QUATERNION_H_
#define CORE_MATHS_LINEAR_ALGEBRA_QUATERNION_H_

#include "vector3.h"

namespace amit::maths
{
    class Quaternion
    {
    public:
        Quaternion();

        static Quaternion GetIdentityQuaternion();
        static Quaternion Slerp(const Quaternion& from, const Quaternion& to, float interpolation_factor);

        float w() const;
        float x() const;
        float y() const;
        float z() const;

        void w(float iW)
        {
            w_ = iW;
        }
        void x(float iX)
        {
            x_ = iX;
        }
        void y(float iY)
        {
            y_ = iY;
        }
        void z(float iZ)
        {
            z_ = iZ;
        }

        Quaternion& operator=(const Quaternion& other);

        Quaternion operator*(const Quaternion& other) const;
        Quaternion cross(const Quaternion& other) const;

        Vector3 operator*(const Vector3& other);

        void       Inverse();
        Quaternion CreateInverse() const;

        bool operator==(const Quaternion& other) const;
        bool operator!=(const Quaternion& other) const;

        Quaternion operator-() const;
        void       Conjugate();

        float Magnitude() const;

        // Normalization
        /*
            q = [s,v] = [s,x,y,z]
            magnitude = length = sqrt(s*s + x*x + y*y + z*z)
            normalize = magnitude * q
        */
        void       Normalize();
        Quaternion CreateNormalized() const;

        // Angular Displacement
        /*
              Given orientations a and b, we can compute the angular displacement
              d which rotates from a to b i.e.
              ad = b => d(difference) = inverse(a)b
        */
        Quaternion  AngularDisplacement(const Quaternion& other) const;
        Quaternion  operator-(const Quaternion& other) const;
        Quaternion& operator-=(const Quaternion& other);

        // Dot Product
        /*
          The geometric interpretation of the quaternion Dot product is similar to
          the interpretation of the vector Dot product;
          The larger the absolute value of the quaternion Dot product ab, the more
          �similar� the angular displacements represented by a and b.
        */
        float Dot(const Quaternion& other) const;

        // Exponent
        /*
          Quaternion exponentiation is useful because it allows us to
          extract a �fraction� of an angular displacement.
          For example, to compute a quaternion that represents one-third
          of the angular displacement represented by the quaternion q,
          we would compute q1/3.

          The caveat we here is this: a quaternion represents angular displacements
          using the shortest arc. �Multiple spins� cannot be represented.
          For our example from above, q4 is not a 240� clockwise rotation about the
          x-axis as expected; it is an 80� counterclockwise rotation.
        */
        Quaternion operator^(const float power) const;

        Quaternion(float angle_in_radians, const Vector3& axis_of_rotation);
        static Quaternion GetYaw(float angle_in_radians);    // About y-axis
        static Quaternion GetPitch(float angle_in_radians);  // About x-axis
        static Quaternion GetRoll(float angle_in_radians);   // About z-axis
        static Quaternion GetYawPitchRoll(const float angle_in_radians_for_pitch, const float angle_in_radians_for_yaw, const float angle_in_radians_for_roll);

        std::string ToString() const;

    private:
        float   w_, x_, y_, z_;
        float   scalar;
        Vector3 vector;
        Quaternion(float w, float x, float y, float z);
        void SplitToScalarVector();
        void SplitFromScalarVector();
    };

}  // namespace amit::maths

#endif  // CORE_MATHS_QUATERNION_H_
