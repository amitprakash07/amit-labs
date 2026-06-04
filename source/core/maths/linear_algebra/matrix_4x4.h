#ifndef CORE_MATHS_LINEAR_ALGEBRA_MATRIX_4X4_H_
#define CORE_MATHS_LINEAR_ALGEBRA_MATRIX_4X4_H_

#include <cstdint>

/*
        This class represents a full Transform
        (i.e. a 4x4 matrix composed of a 3x3 rotation matrix and a 3 element
   translation vector)

        Matrix convention:
                * Storage layout is column-major.
                * Vector convention is row-vector pre-multiply:
                        v' = v * M
                * Transform composition follows the same row-vector order:
                        clip = local * model * view * projection
                        MVP  = model * view * projection
                * Translation lives in m_30, m_31, and m_32.
                * The basis vectors are right-handed:
                        first vector is right, second vector is up, third vector is back.
*/

#include "vector3.h"
#include "vector4.h"

namespace amit::geometry
{
    class Plane;
}

namespace amit::maths
{
    class Quaternion;

    class Matrix4x4
    {
    public:
        enum class StorageLayout : std::uint8_t
        {
            kColumnMajor
        };

        enum class VectorConvention : std::uint8_t
        {
            kRowVectorPreMultiply
        };

        static constexpr StorageLayout    kStorageLayout    = StorageLayout::kColumnMajor;
        static constexpr VectorConvention kVectorConvention = VectorConvention::kRowVectorPreMultiply;

        static Matrix4x4 GetIdentityMatrix();
        static Matrix4x4 CreateNormalMatrix(const Matrix4x4& model_matrix);
        static Matrix4x4 CreateShadowMatrix(const Vector3& light_position, const geometry::Plane& plane);

        Matrix4x4 operator*(float scalar) const;
        Matrix4x4 operator*(const Matrix4x4& other) const;
        void      operator*=(const Matrix4x4& other);

        // Applies this matrix using row-vector convention: result = vector * matrix.
        Vector3 Mul(const Vector3& vector, bool is_vector = false) const;

        // Applies this matrix using row-vector convention: result = vector * matrix.
        Vector4 Mul(const Vector4& vector) const;

        Matrix4x4 GetTranspose() const;
        void      Transpose();
        Matrix4x4 GetInverse() const;
        void      Inverse();
        float     Determinant() const;

        Matrix4x4();
        Matrix4x4(const Quaternion& rotation, const Vector3& translation);
        Matrix4x4(const Vector3& translation, bool direction = false);
        Matrix4x4(const Quaternion& rotation, bool direction = false);
        Matrix4x4(float i_00,
                  float i_10,
                  float i_20,
                  float i_30,
                  float i_01,
                  float i_11,
                  float i_21,
                  float i_31,
                  float i_02,
                  float i_12,
                  float i_22,
                  float i_32,
                  float i_03,
                  float i_13,
                  float i_23,
                  float i_33);

        std::string ToString() const;

    private:
        // Storage is column-major; see notes at the top of the file
        float m_00, m_10, m_20, m_30, m_01, m_11, m_21, m_31, m_02, m_12, m_22, m_32, m_03, m_13, m_23, m_33;
    };

}  // namespace amit::maths

#endif
