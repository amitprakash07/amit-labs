

/*
Link http://www.euclideanspace.com/
*/

#include <cmath>
#include <iostream>

#include "matrix_4x4.h"
#include "plane.h"
#include "quaternion.h"
#include "vector3.h"
#include "vector4.h"

using namespace amit::maths;

Matrix4x4 Matrix4x4::CreateWorldToViewTransform(const Quaternion& camera_orientation, const Vector3& camera_position)
{
    Matrix4x4 transform_viewToWorld(camera_orientation, camera_position);
    // A camera can only ever have rotation and translation
    // and so a lot of simplifying assumptions can be made in order to create the
    // inverse
    return Matrix4x4{transform_viewToWorld.m_00,
                     transform_viewToWorld.m_01,
                     transform_viewToWorld.m_02,
                     -(transform_viewToWorld.m_30 * transform_viewToWorld.m_00) - (transform_viewToWorld.m_31 * transform_viewToWorld.m_01) -
                         (transform_viewToWorld.m_32 * transform_viewToWorld.m_02),
                     transform_viewToWorld.m_10,
                     transform_viewToWorld.m_11,
                     transform_viewToWorld.m_12,
                     -(transform_viewToWorld.m_30 * transform_viewToWorld.m_10) - (transform_viewToWorld.m_31 * transform_viewToWorld.m_11) -
                         (transform_viewToWorld.m_32 * transform_viewToWorld.m_12),
                     transform_viewToWorld.m_20,
                     transform_viewToWorld.m_21,
                     transform_viewToWorld.m_22,
                     -(transform_viewToWorld.m_30 * transform_viewToWorld.m_20) - (transform_viewToWorld.m_31 * transform_viewToWorld.m_21) -
                         (transform_viewToWorld.m_32 * transform_viewToWorld.m_22),
                     0.0f,
                     0.0f,
                     0.0f,
                     1.0f};
}

Matrix4x4 Matrix4x4::GetIdentityMatrix()
{
    return Matrix4x4{};
}

Matrix4x4 Matrix4x4::CreateLookAtMatrix(const Vector3& eye, const Vector3& target, const Vector3& up_vector)
{
    Vector3 forward     = (target - eye).CreateNormalized();            // F
    Vector3 left        = up_vector.Cross(forward).CreateNormalized();  // L
    Vector3 up          = forward.Cross(left);                          // U
    Vector3 Translation = Vector3(                                      // T
        -left.Dot(eye),
        -up_vector.Dot(eye),
        -forward.Dot(eye));

    Matrix4x4 lookAtMatrix{left.x(),
                           left.y(),
                           left.z(),
                           0,
                           up.x(),
                           up.y(),
                           up.z(),
                           0,
                           forward.x(),
                           forward.y(),
                           forward.z(),
                           0,
                           Translation.x(),
                           Translation.y(),
                           Translation.z(),
                           1};

    return lookAtMatrix;
}

Matrix4x4 Matrix4x4::CreateScaleMatrix(float x_scale, float y_scale, float z_scale)
{
    Matrix4x4 scaleMatrix = Matrix4x4();
    scaleMatrix.m_00      = x_scale;
    scaleMatrix.m_11      = y_scale;
    scaleMatrix.m_22      = z_scale;
    return scaleMatrix;
}

Matrix4x4 Matrix4x4::CreateScaleMatrix(const Vector3& scale_factor)
{
    return CreateScaleMatrix(scale_factor.x(), scale_factor.y(), scale_factor.z());
}

Matrix4x4 Matrix4x4::CreateNormalMatrix(const Matrix4x4& model_matrix)
{
    return (model_matrix.GetTranspose().GetInverse());
}

Matrix4x4 Matrix4x4::CreateViewToScreenTransform(const float field_of_View, const float aspect_ratio, const float z_near_plane, const float z_far_plane)
{
    const float yScale = 1.0f / std::tan(field_of_View * 0.5f);
    const float xScale = yScale / aspect_ratio;

    const float zDistanceScale = z_far_plane / (z_near_plane - z_far_plane);
    return Matrix4x4{xScale, 0.0f, 0.0f, 0.0f, 0.0f, yScale, 0.0f, 0.0f, 0.0f, 0.0f, zDistanceScale, z_near_plane * zDistanceScale, 0.0f, 0.0f, -1.0f, 0.0f};

#if defined(PLATFORM_OPEN_GL)
    const float zDistanceScale = 1.0f / (z_near_plane - z_far_plane);
    return Matrix4x4(xScale,
                     0.0f,
                     0.0f,
                     0.0f,
                     0.0f,
                     yScale,
                     0.0f,
                     0.0f,
                     0.0f,
                     0.0f,
                     (z_near_plane + z_far_plane) * zDistanceScale,
                     (2.0f * z_near_plane * z_far_plane) * zDistanceScale,
                     0.0f,
                     0.0f,
                     -1.0f,
                     0.0f);
#endif
}

Matrix4x4 Matrix4x4::CreateOrthographicViewToScreenTransform(float left, float right, float top, float bottom, float near, float far)
{
    float rightMinusLeft = right - left;
    float topMinusBottom = top - bottom;
    float nearMinusFar   = near - far;
    float leftPlusRight  = left + right;
    float topPlusBottom  = top + bottom;
    float nearPlusFar    = near + far;
    return Matrix4x4(2 / rightMinusLeft,
                     0.0f,
                     0.0f,
                     0.0f,
                     0.0f,
                     2 / topMinusBottom,
                     0.0f,
                     0.0f,
                     0.0f,
                     0.0f,
                     2 / nearMinusFar,
                     0.0f,
                     leftPlusRight / (-rightMinusLeft),
                     topPlusBottom / (-topMinusBottom),
                     nearPlusFar / (-nearMinusFar),
                     1.0f);
}

Matrix4x4::Matrix4x4()
    : m_00(1.0f)
    , m_10(0.0f)
    , m_20(0.0f)
    , m_30(0.0f)
    , m_01(0.0f)
    , m_11(1.0f)
    , m_21(0.0f)
    , m_31(0.0f)
    , m_02(0.0f)
    , m_12(0.0f)
    , m_22(1.0f)
    , m_32(0.0f)
    , m_03(0.0f)
    , m_13(0.0f)
    , m_23(0.0f)
    , m_33(1.0f)
{
}

/*
float  - storage column major
m_00, m_10, m_20, m_30,
m_01, m_11, m_21, m_31,
m_02, m_12, m_22, m_32,
m_03, m_13, m_23, m_33


matrix
m00 m01 m02 m03
m10 m11 m12 m13
m20 m21 m22 m23
m30 m31 m32 m33
*/

Matrix4x4 Matrix4x4::GetInverse() const
{
    // http://www.euclideanspace.com/maths/algebra/matrix/resources/code/index.htm

    Matrix4x4 inverseMatrix;
    inverseMatrix.m_00 = m_12 * m_23 * m_31 - m_13 * m_22 * m_31 + m_13 * m_21 * m_32 - m_11 * m_23 * m_32 - m_12 * m_21 * m_33 + m_11 * m_22 * m_33;
    inverseMatrix.m_01 = m_03 * m_22 * m_31 - m_02 * m_23 * m_31 - m_03 * m_21 * m_32 + m_01 * m_23 * m_32 + m_02 * m_21 * m_33 - m_01 * m_22 * m_33;
    inverseMatrix.m_02 = m_02 * m_13 * m_31 - m_03 * m_12 * m_31 + m_03 * m_11 * m_32 - m_01 * m_13 * m_32 - m_02 * m_11 * m_33 + m_01 * m_12 * m_33;
    inverseMatrix.m_03 = m_03 * m_12 * m_21 - m_02 * m_13 * m_21 - m_03 * m_11 * m_22 + m_01 * m_13 * m_22 + m_02 * m_11 * m_23 - m_01 * m_12 * m_23;
    inverseMatrix.m_10 = m_13 * m_22 * m_30 - m_12 * m_23 * m_30 - m_13 * m_20 * m_32 + m_10 * m_23 * m_32 + m_12 * m_20 * m_33 - m_10 * m_22 * m_33;
    inverseMatrix.m_11 = m_02 * m_23 * m_30 - m_03 * m_22 * m_30 + m_03 * m_20 * m_32 - m_00 * m_23 * m_32 - m_02 * m_20 * m_33 + m_00 * m_22 * m_33;
    inverseMatrix.m_12 = m_03 * m_12 * m_30 - m_02 * m_13 * m_30 - m_03 * m_10 * m_32 + m_00 * m_13 * m_32 + m_02 * m_10 * m_33 - m_00 * m_12 * m_33;
    inverseMatrix.m_13 = m_02 * m_13 * m_20 - m_03 * m_12 * m_20 + m_03 * m_10 * m_22 - m_00 * m_13 * m_22 - m_02 * m_10 * m_23 + m_00 * m_12 * m_23;
    inverseMatrix.m_20 = m_11 * m_23 * m_30 - m_13 * m_21 * m_30 + m_13 * m_20 * m_31 - m_10 * m_23 * m_31 - m_11 * m_20 * m_33 + m_10 * m_21 * m_33;
    inverseMatrix.m_21 = m_03 * m_21 * m_30 - m_01 * m_23 * m_30 - m_03 * m_20 * m_31 + m_00 * m_23 * m_31 + m_01 * m_20 * m_33 - m_00 * m_21 * m_33;
    inverseMatrix.m_22 = m_01 * m_13 * m_30 - m_03 * m_11 * m_30 + m_03 * m_10 * m_31 - m_00 * m_13 * m_31 - m_01 * m_10 * m_33 + m_00 * m_11 * m_33;
    inverseMatrix.m_23 = m_03 * m_11 * m_20 - m_01 * m_13 * m_20 - m_03 * m_10 * m_21 + m_00 * m_13 * m_21 + m_01 * m_10 * m_23 - m_00 * m_11 * m_23;
    inverseMatrix.m_30 = m_12 * m_21 * m_30 - m_11 * m_22 * m_30 - m_12 * m_20 * m_31 + m_10 * m_22 * m_31 + m_11 * m_20 * m_32 - m_10 * m_21 * m_32;
    inverseMatrix.m_31 = m_01 * m_22 * m_30 - m_02 * m_21 * m_30 + m_02 * m_20 * m_31 - m_00 * m_22 * m_31 - m_01 * m_20 * m_32 + m_00 * m_21 * m_32;
    inverseMatrix.m_32 = m_02 * m_11 * m_30 - m_01 * m_12 * m_30 - m_02 * m_10 * m_31 + m_00 * m_12 * m_31 + m_01 * m_10 * m_32 - m_00 * m_11 * m_32;
    inverseMatrix.m_33 = m_01 * m_12 * m_20 - m_02 * m_11 * m_20 + m_02 * m_10 * m_21 - m_00 * m_12 * m_21 - m_01 * m_10 * m_22 + m_00 * m_11 * m_22;
    inverseMatrix      = inverseMatrix * this->Determinant();
    // scale(1 / m_1.Determinant());
    return inverseMatrix;
}

Matrix4x4 Matrix4x4::operator*(float scalar) const
{
    /*
  http://www.euclideanspace.com/maths/algebra/matrix/resources/code/sftranslation_cpp.htm
  */

    Matrix4x4 tempMatrix;
    tempMatrix.m_00 = m_00 * scalar;
    tempMatrix.m_01 = m_01 * scalar;
    tempMatrix.m_02 = m_02 * scalar;
    tempMatrix.m_03 = m_03 * scalar;
    tempMatrix.m_10 = m_10 * scalar;
    tempMatrix.m_11 = m_11 * scalar;
    tempMatrix.m_12 = m_12 * scalar;
    tempMatrix.m_13 = m_13 * scalar;
    tempMatrix.m_20 = m_20 * scalar;
    tempMatrix.m_21 = m_21 * scalar;
    tempMatrix.m_22 = m_22 * scalar;
    tempMatrix.m_23 = m_23 * scalar;
    tempMatrix.m_30 = m_30 * scalar;
    tempMatrix.m_31 = m_31 * scalar;
    tempMatrix.m_32 = m_32 * scalar;
    tempMatrix.m_33 = m_33 * scalar;
    return tempMatrix;
}

Matrix4x4 Matrix4x4::operator*(Matrix4x4 other) const
{
    /*
  http://www.euclideanspace.com/maths/algebra/matrix/resources/code/index.htm#cpp
  */
    Matrix4x4 product_matrix;
    product_matrix.m_00 = m_00 * other.m_00 + m_01 * other.m_10 + m_02 * other.m_20 + m_03 * other.m_30;
    product_matrix.m_01 = m_00 * other.m_01 + m_01 * other.m_11 + m_02 * other.m_21 + m_03 * other.m_31;
    product_matrix.m_02 = m_00 * other.m_02 + m_01 * other.m_12 + m_02 * other.m_22 + m_03 * other.m_32;
    product_matrix.m_03 = m_00 * other.m_03 + m_01 * other.m_13 + m_02 * other.m_23 + m_03 * other.m_33;

    product_matrix.m_10 = m_10 * other.m_00 + m_11 * other.m_10 + m_12 * other.m_20 + m_13 * other.m_30;
    product_matrix.m_11 = m_10 * other.m_01 + m_11 * other.m_11 + m_12 * other.m_21 + m_13 * other.m_31;
    product_matrix.m_12 = m_10 * other.m_02 + m_11 * other.m_12 + m_12 * other.m_22 + m_13 * other.m_32;
    product_matrix.m_13 = m_10 * other.m_03 + m_11 * other.m_13 + m_12 * other.m_23 + m_13 * other.m_33;

    product_matrix.m_20 = m_20 * other.m_00 + m_21 * other.m_10 + m_22 * other.m_20 + m_23 * other.m_30;
    product_matrix.m_21 = m_20 * other.m_01 + m_21 * other.m_11 + m_22 * other.m_21 + m_23 * other.m_31;
    product_matrix.m_22 = m_20 * other.m_02 + m_21 * other.m_12 + m_22 * other.m_22 + m_23 * other.m_32;
    product_matrix.m_23 = m_20 * other.m_03 + m_21 * other.m_13 + m_22 * other.m_23 + m_23 * other.m_33;

    product_matrix.m_30 = m_30 * other.m_00 + m_31 * other.m_10 + m_32 * other.m_20 + m_33 * other.m_30;
    product_matrix.m_31 = m_30 * other.m_01 + m_31 * other.m_11 + m_32 * other.m_21 + m_33 * other.m_31;
    product_matrix.m_32 = m_30 * other.m_02 + m_31 * other.m_12 + m_32 * other.m_22 + m_33 * other.m_32;
    product_matrix.m_33 = m_30 * other.m_03 + m_31 * other.m_13 + m_32 * other.m_23 + m_33 * other.m_33;
    return product_matrix;
}

Vector3 Matrix4x4::Mul(const Vector3& vector, bool is_vector) const
{
    Vector3 translateVector;
    if (!is_vector)
    {
        translateVector[0] = vector.x() * m_00 + vector.y() * m_10 + vector.z() * m_20 + m_30;
        translateVector[1] = vector.x() * m_01 + vector.y() * m_11 + vector.z() * m_21 + m_31;
        translateVector[2] = vector.x() * m_02 + vector.y() * m_12 + vector.z() * m_22 + m_32;
    }
    else
    {
        translateVector[0] = vector.x() * m_00 + vector.y() * m_10 + vector.z() * m_20;
        translateVector[1] = vector.x() * m_01 + vector.y() * m_11 + vector.z() * m_21;
        translateVector[2] = vector.x() * m_02 + vector.y() * m_12 + vector.z() * m_22;
    }

    return translateVector;
}

Vector4 Matrix4x4::Mul(const Vector4& vector) const
{
    const float* const components = vector.AsFloatArray();
    const float        x          = components[0];
    const float        y          = components[1];
    const float        z          = components[2];
    const float        w          = components[3];

    return Vector4{x * m_00 + y * m_10 + z * m_20 + w * m_30,
                   x * m_01 + y * m_11 + z * m_21 + w * m_31,
                   x * m_02 + y * m_12 + z * m_22 + w * m_32,
                   x * m_03 + y * m_13 + z * m_23 + w * m_33};
}

void Matrix4x4::operator*=(const Matrix4x4& other)
{
    *this = (*(this)) * other;
}

void Matrix4x4::Inverse()
{
    *this = this->GetInverse();
}

float Matrix4x4::Determinant() const
{
    float value;
    value = m_03 * m_12 * m_21 * m_30 - m_02 * m_13 * m_21 * m_30 - m_03 * m_11 * m_22 * m_30 + m_01 * m_13 * m_22 * m_30 + m_02 * m_11 * m_23 * m_30 -
            m_01 * m_12 * m_23 * m_30 - m_03 * m_12 * m_20 * m_31 + m_02 * m_13 * m_20 * m_31 + m_03 * m_10 * m_22 * m_31 - m_00 * m_13 * m_22 * m_31 -
            m_02 * m_10 * m_23 * m_31 + m_00 * m_12 * m_23 * m_31 + m_03 * m_11 * m_20 * m_32 - m_01 * m_13 * m_20 * m_32 - m_03 * m_10 * m_21 * m_32 +
            m_00 * m_13 * m_21 * m_32 + m_01 * m_10 * m_23 * m_32 - m_00 * m_11 * m_23 * m_32 - m_02 * m_11 * m_20 * m_33 + m_01 * m_12 * m_20 * m_33 +
            m_02 * m_10 * m_21 * m_33 - m_00 * m_12 * m_21 * m_33 - m_01 * m_10 * m_22 * m_33 + m_00 * m_11 * m_22 * m_33;
    return value;
}

Matrix4x4 Matrix4x4::GetTranspose() const
{
    Matrix4x4 transpose_matrix = Matrix4x4(m_00, m_01, m_02, m_03, m_10, m_11, m_12, m_13, m_20, m_21, m_22, m_23, m_30, m_31, m_32, m_33);
    return transpose_matrix;
}

void Matrix4x4::Transpose()
{
    *this = this->GetTranspose();
}

Matrix4x4::Matrix4x4(const Quaternion& rotation, const Vector3& translation)
    : m_30(translation.x())
    , m_31(translation.y())
    , m_32(translation.z())
    , m_03(0.0f)
    , m_13(0.0f)
    , m_23(0.0f)
    , m_33(1.0f)
{
    /*
  http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
  */
    /*
  1−2(y^2+ z^2)	 2(xy−wz)		 2(xz+wy)
  2(xy+wz)		 1−2(x^2+z^2)	 2(yz−wx)
  2(xz−wy)		 2(yz+wx)		 1−2(x^2+y^2)
  */

    // Only rotation part Assignment below
    const float _2x  = rotation.x() + rotation.x();
    const float _2y  = rotation.y() + rotation.y();
    const float _2z  = rotation.z() + rotation.z();
    const float _2xx = rotation.x() * _2x;
    const float _2xy = _2x * rotation.y();
    const float _2xz = _2x * rotation.z();
    const float _2xw = _2x * rotation.w();
    const float _2yy = _2y * rotation.y();
    const float _2yz = _2y * rotation.z();
    const float _2yw = _2y * rotation.w();
    const float _2zz = _2z * rotation.z();
    const float _2zw = _2z * rotation.w();

    m_00 = 1.0f - _2yy - _2zz;
    m_10 = _2xy + _2zw;
    m_20 = _2xz - _2yw;

    m_01 = _2xy - _2zw;
    m_11 = 1.0f - _2xx - _2zz;
    m_21 = _2yz + _2xw;

    m_02 = _2xz + _2yw;
    m_12 = _2yz - _2xw;
    m_22 = 1.0f - _2xx - _2yy;
}

Matrix4x4::Matrix4x4(const Vector3& translation, bool direction)
{
    m_00 = 1.0f;
    m_01 = 0.0f;
    m_02 = 0.0f;
    m_03 = 0.0f;
    m_10 = 0.0f;
    m_11 = 1.0f;
    m_12 = 0.0f;
    m_13 = 0.0f;
    m_20 = 0.0f;
    m_21 = 0.0f;
    m_22 = 1.0f;
    m_23 = 0.0f;
    m_30 = 0.0f;
    m_31 = 0.0f;
    m_32 = 0.0f;
    m_33 = 1.0f;

    if (direction)
    {
        m_33 = 0.0f;
    }

    m_30 = translation.x();
    m_31 = translation.y();
    m_32 = translation.z();
}

Matrix4x4::Matrix4x4(const Quaternion& rotation, bool direction)
{
    /*
  http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
  */
    /*
  1−2(y^2+ z^2)	 2(xy−wz)		 2(xz+wy)
  2(xy+wz)		 1−2(x^2+z^2)	 2(yz−wx)
  2(xz−wy)		 2(yz+wx)		 1−2(x^2+y^2)
  */

    m_00 = 1.0f;
    m_01 = 0.0f;
    m_02 = 0.0f;
    m_03 = 0.0f;
    m_10 = 0.0f;
    m_11 = 1.0f;
    m_12 = 0.0f;
    m_13 = 0.0f;
    m_20 = 0.0f;
    m_21 = 0.0f;
    m_22 = 1.0f;
    m_23 = 0.0f;
    m_30 = 0.0f;
    m_31 = 0.0f;
    m_32 = 0.0f;
    m_33 = 1.0f;

    if (direction)
        m_33 = 0.0f;

    // Only rotation part Assignment below
    const float _2x  = rotation.x() + rotation.x();
    const float _2y  = rotation.y() + rotation.y();
    const float _2z  = rotation.z() + rotation.z();
    const float _2xx = rotation.x() * _2x;
    const float _2xy = _2x * rotation.y();
    const float _2xz = _2x * rotation.z();
    const float _2xw = _2x * rotation.w();
    const float _2yy = _2y * rotation.y();
    const float _2yz = _2y * rotation.z();
    const float _2yw = _2y * rotation.w();
    const float _2zz = _2z * rotation.z();
    const float _2zw = _2z * rotation.w();

    m_00 = 1.0f - _2yy - _2zz;
    m_10 = _2xy + _2zw;
    m_20 = _2xz - _2yw;

    m_01 = _2xy - _2zw;
    m_11 = 1.0f - _2xx - _2zz;
    m_21 = _2yz + _2xw;

    m_02 = _2xz + _2yw;
    m_12 = _2yz - _2xw;
    m_22 = 1.0f - _2xx - _2yy;
}

Matrix4x4::Matrix4x4(const float i_00,
                     const float i_10,
                     const float i_20,
                     const float i_30,
                     const float i_01,
                     const float i_11,
                     const float i_21,
                     const float i_31,
                     const float i_02,
                     const float i_12,
                     const float i_22,
                     const float i_32,
                     const float i_03,
                     const float i_13,
                     const float i_23,
                     const float i_33)
    : m_00(i_00)
    , m_10(i_10)
    , m_20(i_20)
    , m_30(i_30)
    , m_01(i_01)
    , m_11(i_11)
    , m_21(i_21)
    , m_31(i_31)
    , m_02(i_02)
    , m_12(i_12)
    , m_22(i_22)
    , m_32(i_32)
    , m_03(i_03)
    , m_13(i_13)
    , m_23(i_23)
    , m_33(i_33)
{
}

void Matrix4x4::PrintMatrix4x4() const
{
    std::cout << "Matrix values is =\n";
    std::cout << m_00 << ", " << m_01 << ", " << m_02 << ", " << m_03 << std::endl;
    std::cout << m_10 << ", " << m_11 << ", " << m_12 << ", " << m_13 << std::endl;
    std::cout << m_20 << ", " << m_21 << ", " << m_22 << ", " << m_23 << std::endl;
    std::cout << m_30 << ", " << m_31 << ", " << m_32 << ", " << m_33 << std::endl;
}

Matrix4x4 Matrix4x4::CreateShadowMatrix(const Vector3& light_position, const geometry::Plane& plane)
{
    Matrix4x4 shadow_matrix;
    float     nDotL = light_position.Dot(plane.Normal());
    float     d     = plane.Distance();
    Vector3   l     = light_position;
    Vector3   n     = plane.Normal();

    /*
          n = plane normal, d = distance from origin, l= light position

          n.l + d - l.x*n.x		  -l.x*n.y			   -l.x*n.z
     -l.x * d
              -l.y*n.x			n.l + d - l.y*n.y      -l.y*n.z			-l.y *
     d
                  -l.z*n.x			  -l.z*n.y			n.l + d
     -l.z*n.z	-l.z * d -n.x					-n.y
     -n.z			 n.l
  */

    shadow_matrix.m_00 = nDotL + d - (l.x() * n.x());
    shadow_matrix.m_01 = -(l.x() * n.y());
    shadow_matrix.m_02 = -(l.x() * n.z());
    shadow_matrix.m_03 = -(l.x() * d);

    shadow_matrix.m_10 = -(l.y() * n.x());
    shadow_matrix.m_11 = nDotL + d - (l.y() * n.y());
    shadow_matrix.m_12 = -(l.y() * n.z());
    shadow_matrix.m_13 = -(l.y() * d);

    shadow_matrix.m_20 = -(l.z() * n.x());
    shadow_matrix.m_21 = -(l.z() * n.y());
    shadow_matrix.m_22 = nDotL + d - (l.z() * n.z());
    shadow_matrix.m_23 = -(l.z() * d);

    shadow_matrix.m_30 = -n.x();
    shadow_matrix.m_31 = -n.y();
    shadow_matrix.m_32 = -n.z();
    shadow_matrix.m_33 = nDotL;

    return shadow_matrix;
}
