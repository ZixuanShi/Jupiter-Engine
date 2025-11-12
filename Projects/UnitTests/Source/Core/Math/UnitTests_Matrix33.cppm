// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"
#include "Debugging/Logger.h"

export module UnitTests_Matrix33;

import jpt.Matrix33;
import jpt.Vector3;
import jpt.Vector2;
import jpt.Utilities;
import jpt.Math;
import jpt.String;

bool UnitTests_Matrix33_Translation()
{
    // Test basic translation
    Matrix33 translation = Matrix33::Translate(Vec2(2.0f, 3.0f));
    Vec2 point(0.0f, 0.0f);
    Vec2 result = translation * point;

    JPT_ENSURE(jpt::AreValuesClose(result.x, 2.0f));
    JPT_ENSURE(jpt::AreValuesClose(result.y, 3.0f));
    JPT_ENSURE(translation.GetPosition() == Vec2(2.0f, 3.0f));

    // Test zero translation (should equal identity)
    Matrix33 zeroTranslation = Matrix33::Translate(Vec2(0.0f, 0.0f));
    JPT_ENSURE(zeroTranslation == Matrix33::Identity());

    // Test negative translation
    Matrix33 negTranslation = Matrix33::Translate(Vec2(-5.0f, -10.0f));
    Vec2 point2(10.0f, 20.0f);
    Vec2 result2 = negTranslation * point2;
    JPT_ENSURE(jpt::AreValuesClose(result2.x, 5.0f));
    JPT_ENSURE(jpt::AreValuesClose(result2.y, 10.0f));
    JPT_ENSURE(negTranslation.GetPosition() == Vec2(-5.0f, -10.0f));

    // Test combined translations
    Matrix33 trans1 = Matrix33::Translate(Vec2(3.0f, 4.0f));
    Matrix33 trans2 = Matrix33::Translate(Vec2(5.0f, 6.0f));
    Matrix33 combined = trans1 * trans2;
    Vec2 point3(0.0f, 0.0f);
    Vec2 result3 = combined * point3;
    JPT_ENSURE(jpt::AreValuesClose(result3.x, 8.0f));
    JPT_ENSURE(jpt::AreValuesClose(result3.y, 10.0f));

    return true;
}

bool UnitTests_Matrix33_Rotation()
{
    // Test 90-degree rotation
    Matrix33 rot90 = Matrix33::Rotate(jpt::ToRadians(90.0f));
    Vec2 pointX(1.0f, 0.0f);
    Vec2 result90 = rot90 * pointX;
    JPT_ENSURE(jpt::AreValuesClose(result90.x, 0.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(result90.y, 1.0f, 0.001f));

    // Test 180-degree rotation
    Matrix33 rot180 = Matrix33::Rotate(jpt::ToRadians(180.0f));
    Vec2 point(1.0f, 1.0f);
    Vec2 result180 = rot180 * point;
    JPT_ENSURE(jpt::AreValuesClose(result180.x, -1.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(result180.y, -1.0f, 0.001f));

    // Test 360-degree rotation (should equal identity)
    Matrix33 rot360 = Matrix33::Rotate(jpt::ToRadians(360.0f));
    Vec2 point360(3.0f, 4.0f);
    Vec2 result360 = rot360 * point360;
    JPT_ENSURE(jpt::AreValuesClose(result360.x, 3.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(result360.y, 4.0f, 0.001f));

    // Test zero rotation (should equal identity)
    Matrix33 rot0 = Matrix33::Rotate(0.0f);
    JPT_ENSURE(rot0 == Matrix33::Identity());

    // Test combined rotations (45 + 45 = 90)
    Matrix33 rot45a = Matrix33::Rotate(jpt::ToRadians(45.0f));
    Matrix33 rot45b = Matrix33::Rotate(jpt::ToRadians(45.0f));
    Matrix33 combinedRot = rot45a * rot45b;

    Vec2 testPoint(1.0f, 0.0f);
    Vec2 resultCombined = combinedRot * testPoint;
    Vec2 result90Direct = rot90 * testPoint;

    JPT_ENSURE(jpt::AreValuesClose(resultCombined.x, result90Direct.x, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(resultCombined.y, result90Direct.y, 0.001f));

    // Test negative rotation (clockwise)
    Matrix33 rotNeg90 = Matrix33::Rotate(jpt::ToRadians(-90.0f));
    Vec2 pointY(0.0f, 1.0f);
    Vec2 resultNeg90 = rotNeg90 * pointY;
    JPT_ENSURE(jpt::AreValuesClose(resultNeg90.x, 1.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(resultNeg90.y, 0.0f, 0.001f));

    return true;
}

bool UnitTests_Matrix33_Scaling()
{
    // Test uniform scaling
    Matrix33 uniformScale = Matrix33::Scale(Vec2(2.0f, 2.0f));
    Vec2 point(3.0f, 4.0f);
    Vec2 result = uniformScale * point;
    JPT_ENSURE(jpt::AreValuesClose(result.x, 6.0f));
    JPT_ENSURE(jpt::AreValuesClose(result.y, 8.0f));
    JPT_ENSURE(uniformScale.GetScale() == Vec2(2.0f, 2.0f));

    // Test non-uniform scaling
    Matrix33 nonUniformScale = Matrix33::Scale(Vec2(3.0f, 5.0f));
    Vec2 point2(2.0f, 4.0f);
    Vec2 result2 = nonUniformScale * point2;
    JPT_ENSURE(jpt::AreValuesClose(result2.x, 6.0f));
    JPT_ENSURE(jpt::AreValuesClose(result2.y, 20.0f));
    JPT_ENSURE(nonUniformScale.GetScale() == Vec2(3.0f, 5.0f));

    // Test identity scaling
    Matrix33 identityScale = Matrix33::Scale(Vec2(1.0f, 1.0f));
    JPT_ENSURE(identityScale == Matrix33::Identity());

    // Test zero scaling
    Matrix33 zeroScale = Matrix33::Scale(Vec2(0.0f, 0.0f));
    Vec2 point3(5.0f, 10.0f);
    Vec2 result3 = zeroScale * point3;
    JPT_ENSURE(jpt::AreValuesClose(result3.x, 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(result3.y, 0.0f));

    // Test negative scaling (mirroring)
    Matrix33 negativeScale = Matrix33::Scale(Vec2(-1.0f, 1.0f));
    Vec2 point4(3.0f, 4.0f);
    Vec2 result4 = negativeScale * point4;
    JPT_ENSURE(jpt::AreValuesClose(result4.x, -3.0f));
    JPT_ENSURE(jpt::AreValuesClose(result4.y, 4.0f));

    // Test combined scaling
    Matrix33 scale1 = Matrix33::Scale(Vec2(2.0f, 2.0f));
    Matrix33 scale2 = Matrix33::Scale(Vec2(3.0f, 3.0f));
    Matrix33 combined = scale1 * scale2;
    Vec2 point5(1.0f, 1.0f);
    Vec2 result5 = combined * point5;
    JPT_ENSURE(jpt::AreValuesClose(result5.x, 6.0f));
    JPT_ENSURE(jpt::AreValuesClose(result5.y, 6.0f));

    return true;
}

bool UnitTests_Matrix33_Transpose()
{
    // Test transpose of identity
    Matrix33 identity = Matrix33::Identity();
    Matrix33 transposedIdentity = Matrix33::Transpose(identity);
    JPT_ENSURE(transposedIdentity == identity);

    // Test double transpose returns original
    Matrix33 translation = Matrix33::Translate(Vec2(3.0f, 5.0f));
    Matrix33 transposed = Matrix33::Transpose(translation);
    Matrix33 doubleTransposed = Matrix33::Transpose(transposed);
    JPT_ENSURE(doubleTransposed == translation);

    // Test transpose of rotation matrix
    Matrix33 rotation = Matrix33::Rotate(jpt::ToRadians(45.0f));
    Matrix33 rotationTransposed = Matrix33::Transpose(rotation);

    // For rotation matrices, transpose should equal inverse
    Matrix33 combined = rotation * rotationTransposed;

    // Result should be close to identity (in the 2x2 rotation part)
    Vec2 testPoint(1.0f, 0.0f);
    Vec2 result = combined * testPoint;
    JPT_ENSURE(jpt::AreValuesClose(result.x, 1.0f, 0.01f));
    JPT_ENSURE(jpt::AreValuesClose(result.y, 0.0f, 0.01f));

    // Test transpose elements are swapped correctly
    Matrix33 testMatrix = Matrix33::Identity();
    testMatrix[0][1] = 5.0f;
    testMatrix[1][0] = 3.0f;
    Matrix33 testTransposed = Matrix33::Transpose(testMatrix);
    JPT_ENSURE(jpt::AreValuesClose(testTransposed[0][1], 3.0f));
    JPT_ENSURE(jpt::AreValuesClose(testTransposed[1][0], 5.0f));

    // Test specific rotation transpose
    Matrix33 rot90 = Matrix33::Rotate(jpt::ToRadians(90.0f));
    JPT_ENSURE(rot90[0] == Vec3(0.0f, 1.0f, 0.0f));
    JPT_ENSURE(rot90[1] == Vec3(-1.0f, 0.0f, 0.0f));
    JPT_ENSURE(rot90[2] == Vec3(0.0f, 0.0f, 1.0f));

    Matrix33 rot90Transposed = Matrix33::Transpose(rot90);
    JPT_ENSURE(rot90Transposed[0] == Vec3(0.0f, -1.0f, 0.0f));
    JPT_ENSURE(rot90Transposed[1] == Vec3(1.0f, 0.0f, 0.0f));
    JPT_ENSURE(rot90Transposed[2] == Vec3(0.0f, 0.0f, 1.0f));

    return true;
}

bool UnitTests_Matrix33_Determinant()
{
    // Test determinant of identity matrix (should be 1)
    Matrix33 identity = Matrix33::Identity();
    JPT_ENSURE(jpt::AreValuesClose(identity.Determinant(), 1.0f));

    // Test determinant of uniform scaling matrix
    Matrix33 scale2 = Matrix33::Scale(Vec2(2.0f, 2.0f));
    JPT_ENSURE(jpt::AreValuesClose(scale2.Determinant(), 4.0f)); // 2 * 2 = 4

    // Test determinant of non-uniform scaling
    Matrix33 scaleNonUniform = Matrix33::Scale(Vec2(3.0f, 4.0f));
    JPT_ENSURE(jpt::AreValuesClose(scaleNonUniform.Determinant(), 12.0f)); // 3 * 4 = 12

    // Test determinant of rotation matrix (should be 1 for pure rotation)
    Matrix33 rotation = Matrix33::Rotate(jpt::ToRadians(45.0f));
    JPT_ENSURE(jpt::AreValuesClose(rotation.Determinant(), 1.0f, 0.001f));

    // Test determinant of 90-degree rotation
    Matrix33 rot90 = Matrix33::Rotate(jpt::ToRadians(90.0f));
    JPT_ENSURE(jpt::AreValuesClose(rot90.Determinant(), 1.0f, 0.001f));

    // Test determinant of translation matrix (should be 1)
    Matrix33 translation = Matrix33::Translate(Vec2(5.0f, 10.0f));
    JPT_ENSURE(jpt::AreValuesClose(translation.Determinant(), 1.0f));

    // Test determinant of combined transformations
    Matrix33 combined = Matrix33::Scale(Vec2(2.0f, 3.0f)) * Matrix33::Rotate(jpt::ToRadians(45.0f));
    JPT_ENSURE(jpt::AreValuesClose(combined.Determinant(), 6.0f, 0.01f)); // scale determinant is 2*3=6

    // Test determinant with negative scale (mirroring, determinant should be negative)
    Matrix33 negativeScale = Matrix33::Scale(Vec2(-1.0f, 1.0f));
    JPT_ENSURE(jpt::AreValuesClose(negativeScale.Determinant(), -1.0f));

    // Test determinant of zero scale (should be 0, non-invertible)
    Matrix33 zeroScale = Matrix33::Scale(Vec2(0.0f, 1.0f));
    JPT_ENSURE(jpt::AreValuesClose(zeroScale.Determinant(), 0.0f));

    return true;
}

bool UnitTests_Matrix33_Inverse()
{
    // Test inverse of identity (should be identity)
    Matrix33 identity = Matrix33::Identity();
    Matrix33 identityInv = Matrix33::Inverse(identity);
    JPT_ENSURE(identityInv == identity);

    // Test that matrix * inverse = identity
    Matrix33 translation = Matrix33::Translate(Vec2(5.0f, 10.0f));
    Matrix33 translationInv = Matrix33::Inverse(translation);
    Matrix33 shouldBeIdentity = translation * translationInv;

    Vec2 testPoint(3.0f, 7.0f);
    Vec2 result = shouldBeIdentity * testPoint;
    JPT_ENSURE(jpt::AreValuesClose(result.x, 3.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(result.y, 7.0f, 0.001f));

    // Test inverse of rotation
    Matrix33 rotation = Matrix33::Rotate(jpt::ToRadians(45.0f));
    Matrix33 rotationInv = Matrix33::Inverse(rotation);
    Matrix33 rotationCombined = rotation * rotationInv;

    Vec2 rotTestPoint(1.0f, 0.0f);
    Vec2 rotResult = rotationCombined * rotTestPoint;
    JPT_ENSURE(jpt::AreValuesClose(rotResult.x, 1.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(rotResult.y, 0.0f, 0.001f));

    // Test inverse of scaling
    Matrix33 scale = Matrix33::Scale(Vec2(2.0f, 3.0f));
    Matrix33 scaleInv = Matrix33::Inverse(scale);
    Matrix33 scaleCombined = scale * scaleInv;

    Vec2 scaleTestPoint(5.0f, 10.0f);
    Vec2 scaleResult = scaleCombined * scaleTestPoint;
    JPT_ENSURE(jpt::AreValuesClose(scaleResult.x, 5.0f, 0.001f));
    JPT_ENSURE(jpt::AreValuesClose(scaleResult.y, 10.0f, 0.001f));

    // Test your original complex transformation
    Vec2 originV = Vec2(1.0f, 1.0f);
    Matrix33 transform = Matrix33::Identity();
    transform *= Matrix33::Translate(Vec2(2.0f, 3.0f));
    transform *= Matrix33::Rotate(jpt::ToRadians(90.0f));
    transform *= Matrix33::Scale(Vec2(2.0f, 2.0f));

    Vec2 transformed = transform * originV;
    Matrix33 transformInv = Matrix33::Inverse(transform);
    Vec2 backToOrigin = transformInv * transformed;

    JPT_ENSURE(jpt::AreValuesClose(backToOrigin.x, originV.x, 0.01f));
    JPT_ENSURE(jpt::AreValuesClose(backToOrigin.y, originV.y, 0.01f));

    // Test that applying transform then inverse returns original point
    Matrix33 complexTransform = Matrix33::Translate(Vec2(10.0f, 20.0f)) *
        Matrix33::Rotate(jpt::ToRadians(30.0f)) *
        Matrix33::Scale(Vec2(2.0f, 3.0f));
    Matrix33 complexInv = Matrix33::Inverse(complexTransform);

    Vec2 originalPoint(3.0f, 7.0f);
    Vec2 transformedPoint = complexTransform * originalPoint;
    Vec2 backToOriginal = complexInv * transformedPoint;

    JPT_ENSURE(jpt::AreValuesClose(backToOriginal.x, originalPoint.x, 0.01f));
    JPT_ENSURE(jpt::AreValuesClose(backToOriginal.y, originalPoint.y, 0.01f));

    return true;
}

bool UnitTests_Matrix33_IsOrthogonal()
{
    // Test that identity is orthogonal
    Matrix33 identity = Matrix33::Identity();
    JPT_ENSURE(identity.IsOrthogonal());

    // Test that pure rotation matrices are orthogonal
    Matrix33 rotation = Matrix33::Rotate(jpt::ToRadians(45.0f));
    JPT_ENSURE(rotation.IsOrthogonal());

    Matrix33 rot90 = Matrix33::Rotate(jpt::ToRadians(90.0f));
    JPT_ENSURE(rot90.IsOrthogonal());

    Matrix33 rot180 = Matrix33::Rotate(jpt::ToRadians(180.0f));
    JPT_ENSURE(rot180.IsOrthogonal());

    // Test that scaling makes matrix non-orthogonal
    Matrix33 scaled = Matrix33::Scale(Vec2(2.0f, 2.0f));
    JPT_ENSURE(!scaled.IsOrthogonal());

    // Test that non-uniform scaling is not orthogonal
    Matrix33 nonUniformScale = Matrix33::Scale(Vec2(2.0f, 3.0f));
    JPT_ENSURE(!nonUniformScale.IsOrthogonal());

    // Test that translation makes matrix non-orthogonal
    Matrix33 translation = Matrix33::Translate(Vec2(5.0f, 10.0f));
    JPT_ENSURE(!translation.IsOrthogonal());

    // Test that rotation + scale is not orthogonal
    Matrix33 rotationAndScale = Matrix33::Rotate(jpt::ToRadians(45.0f)) *
        Matrix33::Scale(Vec2(2.0f, 2.0f));
    JPT_ENSURE(!rotationAndScale.IsOrthogonal());

    // Test that rotation + translation is not orthogonal
    Matrix33 rotationAndTranslation = Matrix33::Rotate(jpt::ToRadians(45.0f)) *
        Matrix33::Translate(Vec2(5.0f, 10.0f));
    JPT_ENSURE(!rotationAndTranslation.IsOrthogonal());

    // Test combined transformation with rotation remains orthogonal only if pure rotation
    Matrix33 combinedRot = Matrix33::Rotate(jpt::ToRadians(30.0f)) *
        Matrix33::Rotate(jpt::ToRadians(60.0f));
    JPT_ENSURE(combinedRot.IsOrthogonal());

    return true;
}

export bool RunUnitTests_Matrix33()
{
    JPT_ENSURE(UnitTests_Matrix33_Translation());
    JPT_ENSURE(UnitTests_Matrix33_Rotation());
    JPT_ENSURE(UnitTests_Matrix33_Scaling());
    JPT_ENSURE(UnitTests_Matrix33_Transpose());
    JPT_ENSURE(UnitTests_Matrix33_Determinant());
    JPT_ENSURE(UnitTests_Matrix33_Inverse());
    JPT_ENSURE(UnitTests_Matrix33_IsOrthogonal());

    return true;
}