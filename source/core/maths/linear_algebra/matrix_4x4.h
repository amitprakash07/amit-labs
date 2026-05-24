#ifndef CORE_MATHS_LINEAR_ALGEBRA_MATRIX_4X4_H_
#define CORE_MATHS_LINEAR_ALGEBRA_MATRIX_4X4_H_

/*
        This class represents a full Transform
        (i.e. a 4x4 matrix composed of a 3x3 rotation matrix and a 3 element
   translation vector)

        Matrix Facts:
                * The float elements are stored as columns
                        * In other words, the second float stored is the first
   element in the second row
                        * This is so that all 16 floats can be copied directly
   to Direct3D or OpenGL (Both HLSL and GLSL interpret a matrix's registers to
   be columns)
                * The vectors are the rows
                        * In other words, the first three rows are the rotation
   and the last row is the translation
                        * This is an arbitrary choice, but it is arguably the
   most common one in computer graphics (every other field outside of computer
   graphics that I know of uses column vectors, though)
                        * This matches standard Direct3D but is backwards from
   standard OpenGL
                * The vectors are right-handed
                        * In other words, the first vector is right, the second
   vector is up, and the third vector is back
                        * This is an arbitrary choice; it matches Maya and
   standard OpenGL, and is backwards from standard Direct3D
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
        static Matrix4x4 CreateWorldToViewTransform(const Quaternion& camera_orientation, const Vector3& camera_position);
        static Matrix4x4 CreateViewToScreenTransform(float field_of_View, float aspect_ratio, float z_near_plane = 0.1f, float z_far_plane = 100.0f);
        static Matrix4x4 CreateOrthographicViewToScreenTransform(float left, float right, float top, float bottom, float near, float far);
        static Matrix4x4 GetIdentityMatrix();
        static Matrix4x4 CreateScaleMatrix(float x_scale, float y_scale, float z_scale);
        static Matrix4x4 CreateLookAtMatrix(const Vector3& eye, const Vector3& target, const Vector3& up_vector);
        static Matrix4x4 CreateScaleMatrix(const Vector3& scale_factor);
        static Matrix4x4 CreateNormalMatrix(const Matrix4x4& model_matrix);
        static Matrix4x4 CreateShadowMatrix(const Vector3& light_position, const geometry::Plane& plane);
        Matrix4x4        operator*(float scalar) const;
        Matrix4x4        operator*(Matrix4x4 other) const;
        Vector3          Mul(const Vector3& vector, bool is_vector = false) const;
        Vector4          Mul(const Vector4& vector) const;
        void             operator*=(const Matrix4x4& other);
        Matrix4x4        GetTranspose() const;
        void             Transpose();
        Matrix4x4        GetInverse() const;
        void             Inverse();
        float            Determinant() const;
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
        void PrintMatrix4x4() const;

    private:
        // Storage is column-major; see notes at the top of the file
        float m_00, m_10, m_20, m_30, m_01, m_11, m_21, m_31, m_02, m_12, m_22, m_32, m_03, m_13, m_23, m_33;
    };

}  // namespace amit::maths

#endif
