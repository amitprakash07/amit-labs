#ifndef CORE_MATHS_LINEAR_ALGEBRA_VECTOR4_H_
#define CORE_MATHS_LINEAR_ALGEBRA_VECTOR4_H_

#include <string>

namespace amit::maths
{
    class Vector4
    {
    public:
        Vector4  operator+(const Vector4& other) const;
        Vector4& operator+=(const Vector4& other);

        Vector4  operator-(const Vector4& other) const;
        Vector4& operator-=(const Vector4& other);
        Vector4  operator-() const;

        Vector4  operator*(float scalar) const;
        Vector4& operator*=(float scalar);

        Vector4  operator/(float scalar) const;
        Vector4& operator/=(float scalar);

        Vector4& operator=(const Vector4& other);

        float   GetLength() const;
        void    Normalize();
        Vector4 GetNormalizedVec() const;

        float Dot(const Vector4& other) const;

        bool operator==(const Vector4& other) const;
        bool operator!=(const Vector4& other) const;

        const float* AsFloatArray() const;

        Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);

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

        float w() const
        {
            return data_[3];            
        }

    private:
        float data_[4];
    };

}  // namespace amit::maths

#endif  // CORE_MATHS_VECTOR_4_H_
