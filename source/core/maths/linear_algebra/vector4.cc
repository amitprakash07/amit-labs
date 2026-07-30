#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>

#include "vector4.h"

#include "engine_math.h"
#include "utils.h"

using namespace amit::maths;

Vector4 Vector4::operator+(const Vector4& other) const
{
    return Vector4{data_[0] + other.data_[0],
                   data_[1] + other.data_[1],
                   data_[2] + other.data_[2],
                   data_[3] + other.data_[3]};
}

Vector4& Vector4::operator+=(const Vector4& other)
{
    data_[0] += other.data_[0];
    data_[1] += other.data_[1];
    data_[2] += other.data_[2];
    data_[3] += other.data_[3];
    return *this;
}

Vector4 Vector4::operator-(const Vector4& other) const
{
    return Vector4{data_[0] - other.data_[0],
                   data_[1] - other.data_[1],
                   data_[2] - other.data_[2],
                   data_[3] - other.data_[3]};
}

Vector4& Vector4::operator-=(const Vector4& other)
{
    data_[0] -= other.data_[0];
    data_[1] -= other.data_[1];
    data_[2] -= other.data_[2];
    data_[3] -= other.data_[3];
    return *this;
}

Vector4 Vector4::operator-() const
{
    return Vector4{-data_[0], -data_[1], -data_[2], -data_[3]};
}

Vector4 Vector4::operator*(float scalar) const
{
    return Vector4{data_[0] * scalar, data_[1] * scalar, data_[2] * scalar, data_[3] * scalar};
}

Vector4& Vector4::operator*=(float scalar)
{
    data_[0] *= scalar;
    data_[1] *= scalar;
    data_[2] *= scalar;
    data_[3] *= scalar;
    return *this;
}

Vector4 operator*(const float i_lhs, const Vector4& other)
{
    return other * i_lhs;
}

Vector4 Vector4::operator/(float scalar) const
{
    assert(std::abs(scalar) > kEpsilon);
    const float rhs_reciprocal = 1.0f / scalar;
    return Vector4{data_[0] * rhs_reciprocal,
                   data_[1] * rhs_reciprocal,
                   data_[2] * rhs_reciprocal,
                   data_[3] * rhs_reciprocal};
}

Vector4& Vector4::operator/=(float scalar)
{
    assert(std::abs(scalar) > kEpsilon);
    const float rhs_reciprocal = 1.0f / scalar;
    data_[0] *= rhs_reciprocal;
    data_[1] *= rhs_reciprocal;
    data_[2] *= rhs_reciprocal;
    data_[3] *= rhs_reciprocal;
    return *this;
}

Vector4& Vector4::operator=(const Vector4& other)
{
    if (this != &other)
    {
        memcpy(data_, other.data_, sizeof(float) * 4);
    }

    return *this;
}

float Vector4::GetLength() const
{
    return std::sqrt((data_[0] * data_[0]) + (data_[1] * data_[1]) + (data_[2] * data_[2]) + (data_[3] * data_[3]));
}

void Vector4::Normalize()
{
    const float length = GetLength();
    assert(length > kEpsilon);
    operator/=(length);
}

Vector4 Vector4::GetNormalizedVec() const
{
    const float length = GetLength();
    assert(length > kEpsilon);
    const float length_reciprocal = 1.0f / length;
    return Vector4{data_[0] * length_reciprocal,
                   data_[1] * length_reciprocal,
                   data_[2] * length_reciprocal,
                   data_[3] * length_reciprocal};
}

float Vector4::Dot(const Vector4& other) const
{
    return (data_[0] * other.data_[0] + data_[1] * other.data_[1] + data_[2] * other.data_[2] +
            data_[3] * other.data_[3]);
}

bool Vector4::operator==(const Vector4& other) const
{
    return (data_[0] == other.data_[0]) & (data_[1] == other.data_[1]) & (data_[2] == other.data_[2]) &
           (data_[3] == other.data_[3]);
}

bool Vector4::operator!=(const Vector4& other) const
{
    return (data_[0] != other.data_[0]) | (data_[1] != other.data_[1]) | (data_[2] != other.data_[2]) |
           (data_[3] != other.data_[3]);
}

Vector4::Vector4(float x, float y, float z, float w)
    : data_{x, y, z, w}
{
}

std::string Vector4::ToString() const
{
    return std::to_string(data_[0]) + std::to_string(data_[1]) + std::to_string(data_[2]) + std::to_string(data_[3]);
}

const float* Vector4::AsFloatArray() const
{
    return data_;
}
