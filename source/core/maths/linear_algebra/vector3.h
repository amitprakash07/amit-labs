#ifndef CORE_MATHS_LINEAR_ALGEBRA_VECTOR3_H_
#define CORE_MATHS_LINEAR_ALGEBRA_VECTOR3_H_
#include <string>

namespace amit::maths
{
    class Vector3
    {
    public:
        Vector3  operator+(const Vector3& other) const;
        Vector3& operator+=(const Vector3& other);

        Vector3  operator-(const Vector3& other) const;
        Vector3& operator-=(const Vector3& other);
        Vector3  operator-() const;

        Vector3  operator*(float scalar) const;
        Vector3& operator*=(float scalar);

        Vector3 operator*(const Vector3& other) const;

        Vector3  operator/(float scalar) const;
        Vector3& operator/=(float scalar);

        Vector3& operator=(const Vector3& other);

        float   GetLength() const;
        float   Normalize();
        Vector3 CreateNormalized() const;

        float Dot(const Vector3& other) const;

        Vector3 Cross(const Vector3& other) const;

        bool operator==(const Vector3& other) const;
        bool operator!=(const Vector3& other) const;

        float operator[](int index) const;

        float& operator[](int index);

        float ScalarTripleProduct(const Vector3& vec_a, const Vector3& vec_b) const;

        const float* AsFloatArray() const;

        Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f);

        std::string ToString() const;

        float x() const
        {
            return data_[0];
        }

        float y() const
        {
            return data_[1];
        }

        float z() const
        {
            return data_[2];
        }

    private:
        float data_[3];
    };
}  // namespace amit::maths

#endif  // CORE_MATHS_VECTOR_3_H_
