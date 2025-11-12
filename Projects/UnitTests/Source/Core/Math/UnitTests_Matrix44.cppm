// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Matrix44;

import jpt.Matrix44;
import jpt.Vector3;
import jpt.Vector4;
import jpt.Math;
import jpt.Utilities;
import jpt.String;

// https://www.andre-gaschler.com/rotationconverter/

bool UnitTests_Matrix44_Translation()
{
    // Test basic translation
    Matrix44 translation = Matrix44::Translate(Vec3f(5.0f, 10.0f, 15.0f));
    Vec4f point(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4f result = translation * point;
    JPT_ENSURE(jpt::AreValuesClose(result.x, 6.0f));
    JPT_ENSURE(jpt::AreValuesClose(result.y, 12.0f));
    JPT_ENSURE(jpt::AreValuesClose(result.z, 18.0f));
    JPT_ENSURE(jpt::AreValuesClose(result.w, 1.0f));
    JPT_ENSURE(translation.GetPosition() == Vec3f(5.0f, 10.0f, 15.0f));

    // Test zero translation (should equal identity)
    Matrix44 zeroTranslation = Matrix44::Translate(Vec3f(0.0f, 0.0f, 0.0f));
    JPT_ENSURE(zeroTranslation == Matrix44::Identity());

    // Test negative translation
    Matrix44 negTranslation = Matrix44::Translate(Vec3f(-3.0f, -6.0f, -9.0f));
    Vec4f point2(10.0f, 20.0f, 30.0f, 1.0f);
    Vec4f result2 = negTranslation * point2;
    JPT_ENSURE(jpt::AreValuesClose(result2.x, 7.0f));
    JPT_ENSURE(jpt::AreValuesClose(result2.y, 14.0f));
    JPT_ENSURE(jpt::AreValuesClose(result2.z, 21.0f));
    JPT_ENSURE(jpt::AreValuesClose(result2.w, 1.0f));
    JPT_ENSURE(negTranslation.GetPosition() == Vec3f(-3.0f, -6.0f, -9.0f));

    // Test combined translations
    Matrix44 trans1 = Matrix44::Translate(Vec3f(1.0f, 2.0f, 3.0f));
    Matrix44 trans2 = Matrix44::Translate(Vec3f(4.0f, 5.0f, 6.0f));
    Matrix44 combined = trans1 * trans2;
    Vec4f point3(0.0f, 0.0f, 0.0f, 1.0f);
    Vec4f result3 = combined * point3;
    JPT_ENSURE(jpt::AreValuesClose(result3.x, 5.0f));
    JPT_ENSURE(jpt::AreValuesClose(result3.y, 7.0f));
    JPT_ENSURE(jpt::AreValuesClose(result3.z, 9.0f));

    trans1 *= trans2;
    JPT_ENSURE(trans1 == combined);

    return true;
}

bool UnitTests_Matrix44_Rotation()
{
    // Test rotation around X-axis (90 degrees)
    Matrix44 rotX = Matrix44::FromEulerAngles(Vec3f(HalfPi, 0.0f, 0.0f));
    Vec4f pointY(0.0f, 1.0f, 0.0f, 1.0f);
    Vec4f resultX = rotX * pointY;
    JPT_ENSURE(jpt::AreValuesClose(resultX.x, 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(resultX.y, 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(resultX.z, 1.0f));

    // Test rotation around Y-axis (90 degrees)
    Matrix44 rotY = Matrix44::FromEulerAngles(Vec3f(0.0f, HalfPi, 0.0f));
    Vec4f pointZ(0.0f, 0.0f, 1.0f, 1.0f);
    Vec4f resultY = rotY * pointZ;
    JPT_ENSURE(jpt::AreValuesClose(resultY.x, 1.0f));
    JPT_ENSURE(jpt::AreValuesClose(resultY.y, 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(resultY.z, 0.0f));

    // Test rotation around Z-axis (90 degrees)
    Matrix44 rotZ = Matrix44::FromEulerAngles(Vec3f(0.0f, 0.0f, HalfPi));
    Vec4f pointX(1.0f, 0.0f, 0.0f, 1.0f);
    Vec4f resultZ = rotZ * pointX;
    JPT_ENSURE(jpt::AreValuesClose(resultZ.x, 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(resultZ.y, 1.0f));
    JPT_ENSURE(jpt::AreValuesClose(resultZ.z, 0.0f));

    // Test zero rotation (should equal identity)
    Matrix44 zeroRot = Matrix44::FromEulerAngles(Vec3f(0.0f, 0.0f, 0.0f));
    JPT_ENSURE(zeroRot == Matrix44::Identity());

    // Test 360-degree rotation (should equal identity)
    Matrix44 fullRot = Matrix44::FromEulerAngles(Vec3f(TwoPi, 0.0f, 0.0f));
    Vec4f point360(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4f result360 = fullRot * point360;
    JPT_ENSURE(jpt::AreValuesClose(result360.x, 1.0f));
    JPT_ENSURE(jpt::AreValuesClose(result360.y, 2.0f));
    JPT_ENSURE(jpt::AreValuesClose(result360.z, 3.0f));

    // Test combined rotations
    Matrix44 rot1 = Matrix44::FromEulerAngles(Vec3f(jpt::ToRadians(45.0f), 0.0f, 0.0f));
    Matrix44 rot2 = Matrix44::FromEulerAngles(Vec3f(jpt::ToRadians(45.0f), 0.0f, 0.0f));
    Matrix44 combinedRot = rot1 * rot2;
    Matrix44 rot90 = Matrix44::FromEulerAngles(Vec3f(jpt::ToRadians(90.0f), 0.0f, 0.0f));
    
    // Combined 45+45 should be approximately equal to 90
    Vec4f testPoint(0.0f, 1.0f, 0.0f, 1.0f);
    Vec4f resultCombined = combinedRot * testPoint;
    Vec4f result90 = rot90 * testPoint;
    JPT_ENSURE(jpt::AreValuesClose(resultCombined.x, result90.x));
    JPT_ENSURE(jpt::AreValuesClose(resultCombined.y, result90.y));
    JPT_ENSURE(jpt::AreValuesClose(resultCombined.z, result90.z));

    return true;
}

bool UnitTests_Matrix44_Quaternion()
{
    // Both matrix44 and quaternion rotate the same eulerAngles, see if they are equal
    auto tester = [](Vec3 eulerAngles)
    {
        const Quaternion quaterion = Quaternion::FromEulerAngles(eulerAngles);
        const Matrix44 matrixFromQuat = Matrix44::FromQuaternion(quaterion);
        const Matrix44 matrixFromEuler = Matrix44::FromEulerAngles(eulerAngles);

        return matrixFromQuat == matrixFromEuler;
    };

    JPT_ENSURE(tester(Vec3(jpt::ToRadians(30.0f), jpt::ToRadians(45.0f), jpt::ToRadians(60.0f))));
    JPT_ENSURE(tester(Vec3(jpt::ToRadians(-90.0f), jpt::ToRadians(0.0f), jpt::ToRadians(90.0f))));
    JPT_ENSURE(tester(Vec3(jpt::ToRadians(180.0f), jpt::ToRadians(-45.0f), jpt::ToRadians(135.0f))));
    JPT_ENSURE(tester(Vec3(jpt::ToRadians(0.0f), jpt::ToRadians(0.0f), jpt::ToRadians(0.0f))));
    JPT_ENSURE(tester(Vec3(jpt::ToRadians(360.0f), jpt::ToRadians(360.0f), jpt::ToRadians(360.0f))));
    JPT_ENSURE(tester(Vec3(jpt::ToRadians(-180.0f), jpt::ToRadians(-180.0f), jpt::ToRadians(-180.0f))));

    // Test quaternion identity
    Quaternion identityQuat = Quaternion::FromEulerAngles(Vec3(0.0f, 0.0f, 0.0f));
    Matrix44 matrixFromIdentityQuat = Matrix44::FromQuaternion(identityQuat);
    JPT_ENSURE(matrixFromIdentityQuat == Matrix44::Identity());

    // Test that quaternion and matrix produce same transformation
    Vec3 eulerAngles(jpt::ToRadians(30.0f), jpt::ToRadians(60.0f), jpt::ToRadians(90.0f));
    Quaternion quat = Quaternion::FromEulerAngles(eulerAngles);
    Matrix44 matrixFromQuat = Matrix44::FromQuaternion(quat);
    Matrix44 matrixFromEuler = Matrix44::FromEulerAngles(eulerAngles);
    
    Vec4f testPoint(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4f resultQuat = matrixFromQuat * testPoint;
    Vec4f resultEuler = matrixFromEuler * testPoint;
    
    JPT_ENSURE(jpt::AreValuesClose(resultQuat.x, resultEuler.x));
    JPT_ENSURE(jpt::AreValuesClose(resultQuat.y, resultEuler.y));
    JPT_ENSURE(jpt::AreValuesClose(resultQuat.z, resultEuler.z));

    return true;
}

bool UnitTests_Matrix44_Scaling()
{
    // Test uniform scaling
    Matrix44 uniformScale = Matrix44::Scale(Vec3f(2.0f, 2.0f, 2.0f));
    Vec4f point(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4f result = uniformScale * point;
    JPT_ENSURE(jpt::AreValuesClose(result.x, 2.0f));
    JPT_ENSURE(jpt::AreValuesClose(result.y, 4.0f));
    JPT_ENSURE(jpt::AreValuesClose(result.z, 6.0f));
    JPT_ENSURE(jpt::AreValuesClose(result.w, 1.0f));
    JPT_ENSURE(uniformScale.GetScale() == Vec3f(2.0f, 2.0f, 2.0f));

    // Test non-uniform scaling
    Matrix44 nonUniformScale = Matrix44::Scale(Vec3f(2.0f, 3.0f, 4.0f));
    Vec4f point2(1.0f, 1.0f, 1.0f, 1.0f);
    Vec4f result2 = nonUniformScale * point2;
    JPT_ENSURE(jpt::AreValuesClose(result2.x, 2.0f));
    JPT_ENSURE(jpt::AreValuesClose(result2.y, 3.0f));
    JPT_ENSURE(jpt::AreValuesClose(result2.z, 4.0f));
    JPT_ENSURE(jpt::AreValuesClose(result2.w, 1.0f));
    JPT_ENSURE(nonUniformScale.GetScale() == Vec3f(2.0f, 3.0f, 4.0f));

    // Test identity scaling
    Matrix44 identityScale = Matrix44::Scale(Vec3f(1.0f, 1.0f, 1.0f));
    JPT_ENSURE(identityScale == Matrix44::Identity());

    // Test zero scaling (edge case)
    Matrix44 zeroScale = Matrix44::Scale(Vec3f(0.0f, 0.0f, 0.0f));
    Vec4f point3(5.0f, 10.0f, 15.0f, 1.0f);
    Vec4f result3 = zeroScale * point3;
    JPT_ENSURE(jpt::AreValuesClose(result3.x, 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(result3.y, 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(result3.z, 0.0f));

    // Test negative scaling (mirroring)
    Matrix44 negativeScale = Matrix44::Scale(Vec3f(-1.0f, 1.0f, 1.0f));
    Vec4f point4(3.0f, 4.0f, 5.0f, 1.0f);
    Vec4f result4 = negativeScale * point4;
    JPT_ENSURE(jpt::AreValuesClose(result4.x, -3.0f));
    JPT_ENSURE(jpt::AreValuesClose(result4.y, 4.0f));
    JPT_ENSURE(jpt::AreValuesClose(result4.z, 5.0f));

    // Test combined scaling
    Matrix44 scale1 = Matrix44::Scale(Vec3f(2.0f, 2.0f, 2.0f));
    Matrix44 scale2 = Matrix44::Scale(Vec3f(3.0f, 3.0f, 3.0f));
    Matrix44 combined = scale1 * scale2;
    Vec4f point5(1.0f, 1.0f, 1.0f, 1.0f);
    Vec4f result5 = combined * point5;
    JPT_ENSURE(jpt::AreValuesClose(result5.x, 6.0f));
    JPT_ENSURE(jpt::AreValuesClose(result5.y, 6.0f));
    JPT_ENSURE(jpt::AreValuesClose(result5.z, 6.0f));

    scale1 *= scale2;
    JPT_ENSURE(scale1 == combined);

    return true;
}

bool UnitTests_Matrix44_Transpose()
{
    // Test transpose of identity
    Matrix44 identity = Matrix44::Identity();
    Matrix44 transposedIdentity = Matrix44::Transpose(identity);
    JPT_ENSURE(transposedIdentity == identity);

    // Test double transpose returns original
    Matrix44 translation = Matrix44::Translate(Vec3f(1.0f, 2.0f, 3.0f));
    Matrix44 transposed = Matrix44::Transpose(translation);
    Matrix44 doubleTransposed = Matrix44::Transpose(transposed);
    JPT_ENSURE(doubleTransposed == translation);

    // Test transpose of rotation matrix
    Matrix44 rotation = Matrix44::FromEulerAngles(Vec3f(jpt::ToRadians(45.0f), jpt::ToRadians(30.0f), jpt::ToRadians(60.0f)));
    Matrix44 rotationTransposed = Matrix44::Transpose(rotation);
    
    // For rotation matrices, transpose should equal inverse
    Matrix44 combined = rotation * rotationTransposed;
    
    // Result should be close to identity
    Vec4f testPoint(1.0f, 0.0f, 0.0f, 1.0f);
    Vec4f result = combined * testPoint;
    JPT_ENSURE(jpt::AreValuesClose(result.x, 1.0f, 0.01f));
    JPT_ENSURE(jpt::AreValuesClose(result.y, 0.0f, 0.01f));
    JPT_ENSURE(jpt::AreValuesClose(result.z, 0.0f, 0.01f));

    // Test transpose elements are swapped correctly
    Matrix44 testMatrix = Matrix44::Identity();
    testMatrix[0][1] = 5.0f;
    testMatrix[1][0] = 3.0f;
    Matrix44 testTransposed = Matrix44::Transpose(testMatrix);
    JPT_ENSURE(jpt::AreValuesClose(testTransposed[0][1], 3.0f));
    JPT_ENSURE(jpt::AreValuesClose(testTransposed[1][0], 5.0f));

    return true;
}

bool UnitTests_Matrix44_Determinant()
{
    // Test determinant of identity matrix (should be 1)
    Matrix44 identity = Matrix44::Identity();
    JPT_ENSURE(jpt::AreValuesClose(identity.Determinant(), 1.0f));

    // Test determinant of zero matrix (should be 0)
    Matrix44 zero = Matrix44::Zero();
    JPT_ENSURE(jpt::AreValuesClose(zero.Determinant(), 0.0f));

    // Test determinant of uniform scaling matrix
    Matrix44 scale2 = Matrix44::Scale(Vec3f(2.0f, 2.0f, 2.0f));
    JPT_ENSURE(jpt::AreValuesClose(scale2.Determinant(), 8.0f)); // 2^3 = 8

    // Test determinant of non-uniform scaling
    Matrix44 scaleNonUniform = Matrix44::Scale(Vec3f(2.0f, 3.0f, 4.0f));
    JPT_ENSURE(jpt::AreValuesClose(scaleNonUniform.Determinant(), 24.0f)); // 2*3*4 = 24

    // Test determinant of rotation matrix (should be 1 for pure rotation)
    Matrix44 rotation = Matrix44::FromEulerAngles(Vec3f(jpt::ToRadians(45.0f), jpt::ToRadians(30.0f), jpt::ToRadians(60.0f)));
    JPT_ENSURE(jpt::AreValuesClose(rotation.Determinant(), 1.0f, 0.001f));

    // Test determinant of translation matrix (should be 1)
    Matrix44 translation = Matrix44::Translate(Vec3f(5.0f, 10.0f, 15.0f));
    JPT_ENSURE(jpt::AreValuesClose(translation.Determinant(), 1.0f));

    // Test determinant of combined transformations
    Matrix44 combined = Matrix44::Scale(Vec3f(2.0f, 2.0f, 2.0f)) *
        Matrix44::FromEulerAngles(Vec3f(jpt::ToRadians(45.0f), 0.0f, 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(combined.Determinant(), 8.0f, 0.01f));

    // Test determinant with negative scale (mirroring, determinant should be negative)
    Matrix44 negativeScale = Matrix44::Scale(Vec3f(-1.0f, 1.0f, 1.0f));
    JPT_ENSURE(jpt::AreValuesClose(negativeScale.Determinant(), -1.0f));

    return true;
}

bool UnitTests_Matrix44_Inverse()
{
    // Test inverse of identity (should be identity)
    Matrix44 identity = Matrix44::Identity();
    Matrix44 identityInv = Matrix44::Inverse(identity);
    JPT_ENSURE(identityInv == identity);

    // Test that matrix * inverse = identity
    Matrix44 translation = Matrix44::Translate(Vec3f(5.0f, 10.0f, 15.0f));
    Matrix44 translationInv = Matrix44::Inverse(translation);
    Matrix44 shouldBeIdentity = translation * translationInv;

    Vec4f testPoint(1.0f, 2.0f, 3.0f, 1.0f);
    Vec4f result = shouldBeIdentity * testPoint;
    JPT_ENSURE(jpt::AreValuesClose(result.x, 1.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(result.y, 2.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(result.z, 3.0f, 0.001f));

    // Test inverse of rotation
    Matrix44 rotation = Matrix44::FromEulerAngles(Vec3f(jpt::ToRadians(45.0f), jpt::ToRadians(30.0f), jpt::ToRadians(60.0f)));
    Matrix44 rotationInv = Matrix44::Inverse(rotation);
    Matrix44 rotationCombined = rotation * rotationInv;

    Vec4f rotTestPoint(1.0f, 0.0f, 0.0f, 1.0f);
    Vec4f rotResult = rotationCombined * rotTestPoint;
    JPT_ENSURE(jpt::AreValuesClose(rotResult.x, 1.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(rotResult.y, 0.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(rotResult.z, 0.0f, 0.001f));

    // Test inverse of scaling
    Matrix44 scale = Matrix44::Scale(Vec3f(2.0f, 3.0f, 4.0f));
    Matrix44 scaleInv = Matrix44::Inverse(scale);
    Matrix44 scaleCombined = scale * scaleInv;

    Vec4f scaleTestPoint(5.0f, 10.0f, 15.0f, 1.0f);
    Vec4f scaleResult = scaleCombined * scaleTestPoint;
    JPT_ENSURE(jpt::AreValuesClose(scaleResult.x, 5.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(scaleResult.y, 10.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(scaleResult.z, 15.0f, 0.001f));

    // Test that applying transform then inverse returns original point
    Matrix44 transform = Matrix44::Translate(Vec3f(10.0f, 20.0f, 30.0f)) *
        Matrix44::FromEulerAngles(Vec3f(jpt::ToRadians(30.0f), jpt::ToRadians(45.0f), jpt::ToRadians(60.0f))) *
        Matrix44::Scale(Vec3f(2.0f, 2.0f, 2.0f));
    Matrix44 transformInv = Matrix44::Inverse(transform);

    Vec4f originalPoint(3.0f, 7.0f, 11.0f, 1.0f);
    Vec4f transformed = transform * originalPoint;
    Vec4f backToOriginal = transformInv * transformed;

    JPT_ENSURE(jpt::AreValuesClose(backToOriginal.x, originalPoint.x, 0.01f));
    JPT_ENSURE(jpt::AreValuesClose(backToOriginal.y, originalPoint.y, 0.01f));
    JPT_ENSURE(jpt::AreValuesClose(backToOriginal.z, originalPoint.z, 0.01f));

    // Test inverse of zero matrix returns identity (as per your implementation)
    Matrix44 zero = Matrix44::Zero();
    Matrix44 zeroInv = Matrix44::Inverse(zero);
    JPT_ENSURE(zeroInv == Matrix44::Identity());

    return true;
}

bool UnitTests_Matrix44_IsOrthogonal()
{
    // Test that identity is orthogonal
    Matrix44 identity = Matrix44::Identity();
    JPT_ENSURE(identity.IsOrthogonal());

    // Test that pure rotation matrices are orthogonal
    Matrix44 rotationX = Matrix44::FromEulerAngles(Vec3f(jpt::ToRadians(45.0f), 0.0f, 0.0f));
    JPT_ENSURE(rotationX.IsOrthogonal());

    Matrix44 rotationY = Matrix44::FromEulerAngles(Vec3f(0.0f, jpt::ToRadians(30.0f), 0.0f));
    JPT_ENSURE(rotationY.IsOrthogonal());

    Matrix44 rotationZ = Matrix44::FromEulerAngles(Vec3f(0.0f, 0.0f, jpt::ToRadians(60.0f)));
    JPT_ENSURE(rotationZ.IsOrthogonal());

    // Test combined rotations are orthogonal
    Matrix44 combinedRotation = Matrix44::FromEulerAngles(Vec3f(jpt::ToRadians(45.0f), jpt::ToRadians(30.0f), jpt::ToRadians(60.0f)));
    JPT_ENSURE(combinedRotation.IsOrthogonal());

    // Test that rotation from quaternion is orthogonal
    Quaternion quat = Quaternion::FromEulerAngles(Vec3f(jpt::ToRadians(25.0f), jpt::ToRadians(35.0f), jpt::ToRadians(55.0f)));
    Matrix44 quatMatrix = Matrix44::FromQuaternion(quat);
    JPT_ENSURE(quatMatrix.IsOrthogonal());

    // Test that scaling makes matrix non-orthogonal
    Matrix44 scaled = Matrix44::Scale(Vec3f(2.0f, 2.0f, 2.0f));
    JPT_ENSURE(!scaled.IsOrthogonal());

    // Test that translation makes matrix non-orthogonal
    Matrix44 translation = Matrix44::Translate(Vec3f(10.0f, 20.0f, 30.0f));
    JPT_ENSURE(!translation.IsOrthogonal());

    // Test that rotation + translation is not orthogonal
    Matrix44 rotationAndTranslation = Matrix44::FromEulerAngles(Vec3f(jpt::ToRadians(45.0f), 0.0f, 0.0f)) *
        Matrix44::Translate(Vec3f(5.0f, 10.0f, 15.0f));
    JPT_ENSURE(!rotationAndTranslation.IsOrthogonal());

    // Test that rotation + scale is not orthogonal
    Matrix44 rotationAndScale = Matrix44::FromEulerAngles(Vec3f(jpt::ToRadians(45.0f), 0.0f, 0.0f)) *
        Matrix44::Scale(Vec3f(2.0f, 2.0f, 2.0f));
    JPT_ENSURE(!rotationAndScale.IsOrthogonal());

    // Test that non-uniform scaling is not orthogonal
    Matrix44 nonUniformScale = Matrix44::Scale(Vec3f(1.0f, 2.0f, 3.0f));
    JPT_ENSURE(!nonUniformScale.IsOrthogonal());

    return true;
}

bool UnitTests_Matrix44_Orthographic()
{
    // Test basic orthographic projection
    Matrix44 ortho = Matrix44::Orthographic(-10.0f, 10.0f, -5.0f, 5.0f, 0.1f, 100.0f);

    // Test that points within the view volume are mapped to [-1, 1] range
    // Center point should map to origin
    Vec4f center(0.0f, 0.0f, -50.0f, 1.0f);
    Vec4f centerProjected = ortho * center;
    centerProjected /= centerProjected.w;
    JPT_ENSURE(jpt::AreValuesClose(centerProjected.x, 0.0f, 0.01f));
    JPT_ENSURE(jpt::AreValuesClose(centerProjected.y, 0.0f, 0.01f));
    JPT_ENSURE(jpt::AreValuesClose(centerProjected.z, 0.0f, 0.1f)); // Depth mapping varies by convention

    // Test right edge maps to 1
    Vec4f rightEdge(10.0f, 0.0f, -50.0f, 1.0f);
    Vec4f rightProjected = ortho * rightEdge;
    rightProjected /= rightProjected.w;
    JPT_ENSURE(jpt::AreValuesClose(rightProjected.x, 1.0f, 0.01f));

    // Test left edge maps to -1
    Vec4f leftEdge(-10.0f, 0.0f, -50.0f, 1.0f);
    Vec4f leftProjected = ortho * leftEdge;
    leftProjected /= leftProjected.w;
    JPT_ENSURE(jpt::AreValuesClose(leftProjected.x, -1.0f, 0.01f));

    // Test top edge maps to 1
    Vec4f topEdge(0.0f, 5.0f, -50.0f, 1.0f);
    Vec4f topProjected = ortho * topEdge;
    topProjected /= topProjected.w;
    JPT_ENSURE(jpt::AreValuesClose(topProjected.y, 1.0f, 0.01f));

    // Test bottom edge maps to -1
    Vec4f bottomEdge(0.0f, -5.0f, -50.0f, 1.0f);
    Vec4f bottomProjected = ortho * bottomEdge;
    bottomProjected /= bottomProjected.w;
    JPT_ENSURE(jpt::AreValuesClose(bottomProjected.y, -1.0f, 0.01f));

    // Test symmetric orthographic projection
    Matrix44 orthoSymmetric = Matrix44::Orthographic(-5.0f, 5.0f, -5.0f, 5.0f, 1.0f, 10.0f);
    Vec4f symmetricCenter(0.0f, 0.0f, -5.5f, 1.0f);
    Vec4f symmetricProjected = orthoSymmetric * symmetricCenter;
    symmetricProjected /= symmetricProjected.w;
    JPT_ENSURE(jpt::AreValuesClose(symmetricProjected.x, 0.0f, 0.01f));
    JPT_ENSURE(jpt::AreValuesClose(symmetricProjected.y, 0.0f, 0.01f));

    return true;
}

bool UnitTests_Matrix44_Perspective()
{
    // Test basic perspective projection
    Precision fov = jpt::ToRadians(90.0f);
    Precision aspect = 16.0f / 9.0f;
    Matrix44 persp = Matrix44::Perspective(fov, aspect, 0.1f, 100.0f);

    // Test that w component becomes -z (perspective divide)
    Vec4f point(0.0f, 0.0f, -10.0f, 1.0f);
    Vec4f projected = persp * point;
    JPT_ENSURE(jpt::AreValuesClose(projected.w, 10.0f, 0.01f)); // w should be -(-10) = 10

    // Test center point projects to center after perspective divide
    Vec4f centerPoint(0.0f, 0.0f, -5.0f, 1.0f);
    Vec4f centerProjected = persp * centerPoint;
    centerProjected /= centerProjected.w;
    JPT_ENSURE(jpt::AreValuesClose(centerProjected.x, 0.0f, 0.01f));
    JPT_ENSURE(jpt::AreValuesClose(centerProjected.y, 0.0f, 0.01f));

    // Test different field of view
    Matrix44 perspNarrow = Matrix44::Perspective(jpt::ToRadians(45.0f), 1.0f, 0.1f, 100.0f);
    Vec4f testPoint(1.0f, 0.0f, -5.0f, 1.0f);
    Vec4f narrowProjected = perspNarrow * testPoint;
    narrowProjected /= narrowProjected.w;

    // Narrower FOV should result in larger x value (less magnification)
    Matrix44 perspWide = Matrix44::Perspective(jpt::ToRadians(90.0f), 1.0f, 0.1f, 100.0f);
    Vec4f wideProjected = perspWide * testPoint;
    wideProjected /= wideProjected.w;

    JPT_ENSURE(narrowProjected.x > wideProjected.x);

    // Test aspect ratio affects horizontal vs vertical scaling
    Matrix44 perspWidescreen = Matrix44::Perspective(jpt::ToRadians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    Matrix44 perspSquare = Matrix44::Perspective(jpt::ToRadians(90.0f), 1.0f, 0.1f, 100.0f);

    Vec4f horizontalPoint(1.0f, 0.0f, -5.0f, 1.0f);
    Vec4f widescreenH = perspWidescreen * horizontalPoint;
    Vec4f squareH = perspSquare * horizontalPoint;

    // Widescreen aspect should compress horizontal more than square
    JPT_ENSURE((widescreenH.x / widescreenH.w) < (squareH.x / squareH.w));

    return true;
}

bool UnitTests_Matrix44_LookAt()
{
    // Test basic look-at pointing down -Z axis (standard camera orientation)
    Vec3f eye(0.0f, 0.0f, 0.0f);
    Vec3f center(0.0f, 0.0f, -1.0f);
    Vec3f up(0.0f, 1.0f, 0.0f);
    Matrix44 lookAt = Matrix44::LookAt(eye, center, up);

    // Point in front of camera should have negative z after transformation
    Vec4f frontPoint(0.0f, 0.0f, -5.0f, 1.0f);
    Vec4f transformed = lookAt * frontPoint;
    JPT_ENSURE(transformed.z < 0.0f);

    // Test camera positioned at origin looking at positive Z
    Vec3f eye2(0.0f, 0.0f, 0.0f);
    Vec3f center2(0.0f, 0.0f, 1.0f);
    Matrix44 lookAt2 = Matrix44::LookAt(eye2, center2, Vec3f::Up());

    Vec4f point2(0.0f, 0.0f, 5.0f, 1.0f);
    Vec4f transformed2 = lookAt2 * point2;
    JPT_ENSURE(transformed2.z < 0.0f); // Should be in front (negative z in view space)

    // Test camera looking from elevated position
    Vec3f eyeElevated(0.0f, 10.0f, 10.0f);
    Vec3f centerGround(0.0f, 0.0f, 0.0f);
    Matrix44 lookAtElevated = Matrix44::LookAt(eyeElevated, centerGround, Vec3f::Up());

    // Transform the center point - should be in front of camera
    Vec4f centerInView = lookAtElevated * Vec4f(centerGround, 1.0f);
    JPT_ENSURE(centerInView.z < 0.0f);

    // Test that looking at the same point as eye position gives identity-like behavior
    Vec3f samePos(5.0f, 5.0f, 5.0f);
    // This is an edge case, but shouldn't crash
    Matrix44 lookAtSame = Matrix44::LookAt(samePos, samePos + Vec3f(0.0f, 0.0f, -0.001f), Vec3f::Up());

    // Test camera positioned to the right, looking left
    Vec3f eyeRight(10.0f, 0.0f, 0.0f);
    Vec3f centerLeft(-10.0f, 0.0f, 0.0f);
    Matrix44 lookAtSide = Matrix44::LookAt(eyeRight, centerLeft, Vec3f::Up());

    Vec4f leftPoint(-5.0f, 0.0f, 0.0f, 1.0f);
    Vec4f transformedSide = lookAtSide * leftPoint;
    JPT_ENSURE(transformedSide.z < 0.0f); // Point between camera and target

    // Test that right vector is perpendicular to forward and up
    Vec3f eye3(0.0f, 0.0f, 5.0f);
    Vec3f center3(0.0f, 0.0f, 0.0f);
    Matrix44 lookAt3 = Matrix44::LookAt(eye3, center3, Vec3f::Up());

    // Extract right vector from first column
    Vec3f right(lookAt3[0][0], lookAt3[1][0], lookAt3[2][0]);
    Vec3f forward = (center3 - eye3).Normalized();

    // Right should be perpendicular to forward (dot product near zero)
    Precision dot = right.Dot(forward);
    JPT_ENSURE(jpt::AreValuesClose(dot, 0.0f, 0.01f));

    return true;
}

export bool RunUnitTests_Matrix44()
{
    JPT_ENSURE(UnitTests_Matrix44_Translation());
    JPT_ENSURE(UnitTests_Matrix44_Rotation());
    JPT_ENSURE(UnitTests_Matrix44_Quaternion());
    JPT_ENSURE(UnitTests_Matrix44_Scaling());
    JPT_ENSURE(UnitTests_Matrix44_Transpose());
    JPT_ENSURE(UnitTests_Matrix44_Determinant());
    JPT_ENSURE(UnitTests_Matrix44_Inverse());
    JPT_ENSURE(UnitTests_Matrix44_IsOrthogonal());
    JPT_ENSURE(UnitTests_Matrix44_Orthographic());
    JPT_ENSURE(UnitTests_Matrix44_Perspective());
    JPT_ENSURE(UnitTests_Matrix44_LookAt());

    return true;
}