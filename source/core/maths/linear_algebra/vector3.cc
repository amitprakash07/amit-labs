#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>

#include "utils.h"
#include "vector3.h"

using namespace amit::maths;

Vector3 Vector3::operator+(const Vector3& other) const
{
    return Vector3{data_[0] + other.data_[0], data_[1] + other.data_[1], data_[2] + other.data_[2]};
}

Vector3& Vector3::operator+=(const Vector3& other)
{
    data_[0] += other.data_[0];
    data_[1] += other.data_[1];
    data_[2] += other.data_[2];
    return *this;
}

Vector3 Vector3::operator-(const Vector3& other) const
{
    return Vector3{data_[0] - other.data_[0], data_[1] - other.data_[1], data_[2] - other.data_[2]};
}

Vector3& Vector3::operator-=(const Vector3& other)
{
    data_[0] -= other.data_[0];
    data_[1] -= other.data_[1];
    data_[2] -= other.data_[2];
    return *this;
}

Vector3 Vector3::operator-() const
{
    return Vector3{-data_[0], -data_[1], -data_[2]};
}

Vector3 Vector3::operator*(float other) const
{
    return Vector3{data_[0] * other, data_[1] * other, data_[2] * other};
}

Vector3& Vector3::operator*=(float other)
{
    data_[0] *= other;
    data_[1] *= other;
    data_[2] *= other;
    return *this;
}

Vector3 Vector3::operator*(const Vector3& other) const
{
    return Vector3{data_[0] * other.data_[0], data_[1] * other.data_[1], data_[2] * other.data_[2]};
}

Vector3 operator*(const float i_lhs, const Vector3& other)
{
    return other * i_lhs;
}

Vector3 Vector3::operator/(float scalar) const
{
    assert(std::abs(scalar) > kEpsilon);
    const float rhs_reciprocal = 1.0f / scalar;
    return Vector3{data_[0] * rhs_reciprocal, data_[1] * rhs_reciprocal, data_[2] * rhs_reciprocal};
}

Vector3& Vector3::operator/=(float scalar)
{
    assert(std::abs(scalar) > kEpsilon);
    const float rhs_reciprocal = 1.0f / scalar;
    data_[0] *= rhs_reciprocal;
    data_[1] *= rhs_reciprocal;
    data_[2] *= rhs_reciprocal;
    return *this;
}

Vector3& Vector3::operator=(const Vector3& other)
{
    if (this != &other)
    {
        memcpy(data_, other.data_, sizeof(float) * 3);
    }

    return *this;
}

float Vector3::GetLength() const
{
    return std::sqrt((data_[0] * data_[0]) + (data_[1] * data_[1]) + (data_[2] * data_[2]));
}

float Vector3::Normalize()
{
    const float length = GetLength();
    assert(length > kEpsilon);
    operator/=(length);
    return length;
}

Vector3 Vector3::CreateNormalized() const
{
    const float length = GetLength();
    assert(length > kEpsilon);
    const float length_reciprocal = 1.0f / length;
    return Vector3{data_[0] * length_reciprocal, data_[1] * length_reciprocal, data_[2] * length_reciprocal};
}

float Vector3::Dot(const Vector3& other) const
{
    return (data_[0] * other.data_[0] + data_[1] * other.data_[1] + data_[2] * other.data_[2]);
}

Vector3 Vector3::Cross(const Vector3& other) const
{
    return Vector3{(data_[1] * other.data_[2]) - (data_[2] * other.data_[1]),
                   (data_[2] * other.data_[0]) - (data_[0] * other.data_[2]),
                   (data_[0] * other.data_[1]) - (data_[1] * other.data_[0])};
}

bool Vector3::operator==(const Vector3& other) const
{
    // Use & rather than && to prevent branches (all three comparisons will be evaluated)
    return (data_[0] == other.data_[0]) & (data_[1] == other.data_[1]) & (data_[2] == other.data_[2]);
}

bool Vector3::operator!=(const Vector3& other) const
{
    // Use | rather than || to prevent branches (all three comparisons will be evaluated)
    return (data_[0] != other.data_[0]) | (data_[1] != other.data_[1]) | (data_[2] != other.data_[2]);
}

Vector3::Vector3(float x, float y, float z)
    : data_{x, y, z}
{
}

std::string Vector3::ToString() const
{
    return std::to_string(data_[0]) + std::to_string(data_[1]) + std::to_string(data_[2]);
}

const float* Vector3::AsFloatArray() const
{
    return data_;
}

float Vector3::operator[](int index) const
{
    if (index < 3 && index >= 0)
    {
        return data_[index];
    }

    return 0xFFFF;
}

float& Vector3::operator[](int index)
{
    if (index < 3 && index >= 0)
    {
        return data_[index];
    }

    static float invalid_index = 0.0f;
    return invalid_index;
}

float Vector3::ScalarTripleProduct(const Vector3& vec_a, const Vector3& vec_b) const
{
    const Vector3 cross_product = Cross(vec_a);
    return vec_b.Dot(cross_product);
}
