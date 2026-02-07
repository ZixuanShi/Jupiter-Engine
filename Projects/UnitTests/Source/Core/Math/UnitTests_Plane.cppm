// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreHeaders.h"

export module UnitTests_Plane;

import jpt.Plane;
import jpt.Vector3;
import jpt.Math;
import jpt.Constants;

static bool UnitTests_Plane_Construction()
{
    // Default constructor
    Planef defaultPlane;
    JPT_ENSURE(defaultPlane.m_normal == Vec3f(0.0f, 1.0f, 0.0f));
    JPT_ENSURE(defaultPlane.m_distance == 0.0f);

    // Constructor with normal and distance
    Planef plane1(Vec3f(0.0f, 1.0f, 0.0f), 5.0f);
    JPT_ENSURE(plane1.m_normal == Vec3f(0.0f, 1.0f, 0.0f));
    JPT_ENSURE(plane1.m_distance == 5.0f);

    // Constructor with normal and point
    Vec3f normal(0.0f, 1.0f, 0.0f);
    Vec3f point(10.0f, 3.0f, 5.0f);
    Planef plane2(normal, point);
    JPT_ENSURE(plane2.m_normal == normal);
    JPT_ENSURE(plane2.m_distance == -3.0f);

    // Constructor with three points
    // Right-hand rule: (b-a) x (c-a) = (1,0,0) x (0,0,1) = (0,-1,0)
    Vec3f a(0.0f, 0.0f, 0.0f);
    Vec3f b(1.0f, 0.0f, 0.0f);
    Vec3f c(0.0f, 0.0f, 1.0f);
    Planef plane3(a, b, c);
    JPT_ENSURE(jpt::AreValuesClose(plane3.m_normal.x, 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(plane3.m_normal.y, -1.0f));
    JPT_ENSURE(jpt::AreValuesClose(plane3.m_normal.z, 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(plane3.m_distance, 0.0f));

    return true;
}

static bool UnitTests_Plane_Distance()
{
    // Horizontal plane at y = 0
    Planef plane(Vec3::Up(), 0.0f);
    Vec3f pointAbove(0.0f, 5.0f, 0.0f);
    Vec3f pointBelow(0.0f, -5.0f, 0.0f);
    Vec3f pointOnPlane(1.0f, 0.0f, 1.0f);

    JPT_ENSURE(plane.DistanceSigned(pointAbove) == 5.0f);
    JPT_ENSURE(plane.DistanceSigned(pointBelow) == -5.0f);
    JPT_ENSURE(plane.DistanceSigned(pointOnPlane) == 0.0f);

    // Unsigned distance
    JPT_ENSURE(plane.Distance(pointAbove) == 5.0f);
    JPT_ENSURE(plane.Distance(pointBelow) == 5.0f);
    JPT_ENSURE(plane.Distance(pointOnPlane) == 0.0f);

    return true;
}

static bool UnitTests_Plane_DistanceSigned()
{
    // Vertical plane along X axis at x = 3
    Vec3f normal = Vec3::Right();
    Vec3f pointOnPlane(3.0f, 0.0f, 0.0f);
    Planef plane(normal, pointOnPlane);

    // Points on positive side (x > 3)
    Vec3f positiveSide(5.0f, 0.0f, 0.0f);
    JPT_ENSURE(plane.DistanceSigned(positiveSide) == 2.0f);

    Vec3f positiveSide2(10.0f, 5.0f, 7.0f);
    JPT_ENSURE(plane.DistanceSigned(positiveSide2) == 7.0f);

    // Points on negative side (x < 3)
    Vec3f negativeSide(1.0f, 0.0f, 0.0f);
    JPT_ENSURE(plane.DistanceSigned(negativeSide) == -2.0f);

    Vec3f negativeSide2(-5.0f, 10.0f, -3.0f);
    JPT_ENSURE(plane.DistanceSigned(negativeSide2) == -8.0f);

    // Point on plane
    Vec3f onPlane(3.0f, 100.0f, -50.0f);
    JPT_ENSURE(jpt::AreValuesClose(plane.DistanceSigned(onPlane), 0.0f));

    return true;
}

static bool UnitTests_Plane_DistanceUnsigned()
{
    // Diagonal plane
    Vec3f normal = Vec3f(1.0f, 1.0f, 0.0f).Normalized();
    Planef plane(normal, 0.0f);

    Vec3f point1(5.0f, 5.0f, 0.0f);
    Vec3f point2(-5.0f, -5.0f, 0.0f);
    Vec3f point3(0.0f, 0.0f, 0.0f);

    // Both sides should have same unsigned distance
    Precision dist1 = plane.Distance(point1);
    Precision dist2 = plane.Distance(point2);
    JPT_ENSURE(jpt::AreValuesClose(dist1, dist2));

    // Point on plane
    JPT_ENSURE(jpt::AreValuesClose(plane.Distance(point3), 0.0f));

    return true;
}

static bool UnitTests_Plane_ProjectPoint()
{
    // Horizontal plane at y = 0
    Planef plane(Vec3::Up(), 0.0f);

    // Project point above plane
    Vec3f pointAbove(3.0f, 5.0f, 7.0f);
    Vec3f projected = plane.ProjectPoint(pointAbove);
    JPT_ENSURE(projected == Vec3f(3.0f, 0.0f, 7.0f));

    // Project point below plane
    Vec3f pointBelow(2.0f, -8.0f, 4.0f);
    projected = plane.ProjectPoint(pointBelow);
    JPT_ENSURE(projected == Vec3f(2.0f, 0.0f, 4.0f));

    // Point already on plane
    Vec3f pointOnPlane(10.0f, 0.0f, -5.0f);
    projected = plane.ProjectPoint(pointOnPlane);
    JPT_ENSURE(jpt::AreValuesClose(projected.x, pointOnPlane.x));
    JPT_ENSURE(jpt::AreValuesClose(projected.y, pointOnPlane.y));
    JPT_ENSURE(jpt::AreValuesClose(projected.z, pointOnPlane.z));

    return true;
}

static bool UnitTests_Plane_ProjectPointVertical()
{
    // Vertical plane along Z axis at x = 5
    Vec3f normal = Vec3::Right();
    Vec3f pointOnPlane(5.0f, 0.0f, 0.0f);
    Planef plane(normal, pointOnPlane);

    Vec3f point(10.0f, 3.0f, 7.0f);
    Vec3f projected = plane.ProjectPoint(point);
    JPT_ENSURE(jpt::AreValuesClose(projected.x, 5.0f));
    JPT_ENSURE(jpt::AreValuesClose(projected.y, 3.0f));
    JPT_ENSURE(jpt::AreValuesClose(projected.z, 7.0f));

    Vec3f point2(-2.0f, -5.0f, 9.0f);
    Vec3f projected2 = plane.ProjectPoint(point2);
    JPT_ENSURE(jpt::AreValuesClose(projected2.x, 5.0f));
    JPT_ENSURE(jpt::AreValuesClose(projected2.y, -5.0f));
    JPT_ENSURE(jpt::AreValuesClose(projected2.z, 9.0f));

    return true;
}

static bool UnitTests_Plane_ProjectPointDiagonal()
{
    // Diagonal plane through origin
    Vec3f normal = Vec3f(1.0f, 1.0f, 0.0f).Normalized();
    Planef plane(normal, 0.0f);

    Vec3f point(5.0f, 0.0f, 0.0f);
    Vec3f projected = plane.ProjectPoint(point);

    // Projected point should be on the plane
    JPT_ENSURE(jpt::AreValuesClose(plane.DistanceSigned(projected), 0.0f, static_cast<Precision>(0.001f)));

    // Distance from original point to projected point should equal the plane distance
    Precision distanceToPlane = plane.Distance(point);
    Precision distanceToProjected = point.Distance(projected);
    JPT_ENSURE(jpt::AreValuesClose(distanceToPlane, distanceToProjected, static_cast<Precision>(0.001f)));

    return true;
}

static bool UnitTests_Plane_ProjectVec3()
{
    // Horizontal plane
    Planef plane(Vec3::Up(), 0.0f);

    // Project vector pointing up
    Vec3f vecUp(0.0f, 5.0f, 0.0f);
    Vec3f projectedUp = plane.ProjectVec3(vecUp);
    JPT_ENSURE(jpt::AreValuesClose(projectedUp.Length(), 0.0f));

    // Project vector pointing diagonally
    Vec3f vecDiag(3.0f, 4.0f, 5.0f);
    Vec3f projectedDiag = plane.ProjectVec3(vecDiag);
    JPT_ENSURE(jpt::AreValuesClose(projectedDiag.x, 3.0f));
    JPT_ENSURE(jpt::AreValuesClose(projectedDiag.y, 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(projectedDiag.z, 5.0f));

    // Project vector already in plane
    Vec3f vecInPlane(7.0f, 0.0f, -3.0f);
    Vec3f projectedInPlane = plane.ProjectVec3(vecInPlane);
    JPT_ENSURE(jpt::AreValuesClose(projectedInPlane.x, vecInPlane.x));
    JPT_ENSURE(jpt::AreValuesClose(projectedInPlane.y, vecInPlane.y));
    JPT_ENSURE(jpt::AreValuesClose(projectedInPlane.z, vecInPlane.z));

    return true;
}

static bool UnitTests_Plane_ProjectVec3Orthogonality()
{
    // Vertical plane
    Vec3f normal = Vec3::Right();
    Planef plane(normal, 0.0f);

    Vec3f vec(5.0f, 3.0f, 7.0f);
    Vec3f projected = plane.ProjectVec3(vec);

    // Projected vector should be orthogonal to plane normal
    Precision dotProduct = projected.Dot(plane.m_normal);
    JPT_ENSURE(jpt::AreValuesClose(dotProduct, 0.0f, static_cast<Precision>(0.001f)));

    // Projected vector should have no component along normal
    JPT_ENSURE(jpt::AreValuesClose(projected.x, 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(projected.y, 3.0f));
    JPT_ENSURE(jpt::AreValuesClose(projected.z, 7.0f));

    return true;
}

static bool UnitTests_Plane_ThreePoints()
{
    // Right-hand rule: (b-a) x (c-a) gives normal
    // (1,0,0) x (0,0,1) = (0,-1,0) pointing down
    Vec3f a(0.0f, 0.0f, 0.0f);
    Vec3f b(1.0f, 0.0f, 0.0f);
    Vec3f c(0.0f, 0.0f, 1.0f);
    Planef plane(a, b, c);

    // Should be horizontal plane at y = 0, normal pointing down
    JPT_ENSURE(jpt::AreValuesClose(plane.m_normal.x, 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(plane.m_normal.y, -1.0f));
    JPT_ENSURE(jpt::AreValuesClose(plane.m_normal.z, 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(plane.m_distance, 0.0f));

    // All three points should be on the plane
    JPT_ENSURE(jpt::AreValuesClose(plane.DistanceSigned(a), 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(plane.DistanceSigned(b), 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(plane.DistanceSigned(c), 0.0f));

    return true;
}

static bool UnitTests_Plane_ThreePointsElevated()
{
    // Triangle at y = 3
    // (b-a) x (c-a) = (5,0,0) x (2,0,7) = (0,-35,0) normalized to (0,-1,0)
    Vec3f a(0.0f, 3.0f, 0.0f);
    Vec3f b(5.0f, 3.0f, 0.0f);
    Vec3f c(2.0f, 3.0f, 7.0f);
    Planef plane(a, b, c);

    // Should be horizontal plane at y = 3, normal pointing down
    JPT_ENSURE(jpt::AreValuesClose(plane.m_normal.y, -1.0f, static_cast<Precision>(0.001f)));
    JPT_ENSURE(jpt::AreValuesClose(plane.m_distance, 3.0f, static_cast<Precision>(0.001f)));

    // All three points should be on the plane
    JPT_ENSURE(jpt::AreValuesClose(plane.DistanceSigned(a), 0.0f, static_cast<Precision>(0.001f)));
    JPT_ENSURE(jpt::AreValuesClose(plane.DistanceSigned(b), 0.0f, static_cast<Precision>(0.001f)));
    JPT_ENSURE(jpt::AreValuesClose(plane.DistanceSigned(c), 0.0f, static_cast<Precision>(0.001f)));

    return true;
}

static bool UnitTests_Plane_ThreePointsDiagonal()
{
    // Diagonal plane
    Vec3f a(0.0f, 0.0f, 0.0f);
    Vec3f b(1.0f, 1.0f, 0.0f);
    Vec3f c(0.0f, 1.0f, 1.0f);
    Planef plane(a, b, c);

    // All three points should be on the plane
    JPT_ENSURE(jpt::AreValuesClose(plane.DistanceSigned(a), 0.0f, static_cast<Precision>(0.001f)));
    JPT_ENSURE(jpt::AreValuesClose(plane.DistanceSigned(b), 0.0f, static_cast<Precision>(0.001f)));
    JPT_ENSURE(jpt::AreValuesClose(plane.DistanceSigned(c), 0.0f, static_cast<Precision>(0.001f)));

    // Normal should be perpendicular to plane
    Vec3f ab = b - a;
    Vec3f ac = c - a;
    JPT_ENSURE(jpt::AreValuesClose(plane.m_normal.Dot(ab), 0.0f, static_cast<Precision>(0.001f)));
    JPT_ENSURE(jpt::AreValuesClose(plane.m_normal.Dot(ac), 0.0f, static_cast<Precision>(0.001f)));

    return true;
}

static bool UnitTests_Plane_AxisAlignedPlanes()
{
    // XY plane (normal = Z)
    Planef xyPlane(Vec3::Forward(), 0.0f);
    Vec3f pointInXY(5.0f, 3.0f, 0.0f);
    Vec3f pointOffXY(5.0f, 3.0f, 7.0f);
    JPT_ENSURE(jpt::AreValuesClose(xyPlane.DistanceSigned(pointInXY), 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(xyPlane.DistanceSigned(pointOffXY), 7.0f));

    // XZ plane (normal = Y)
    Planef xzPlane(Vec3::Up(), 0.0f);
    Vec3f pointInXZ(3.0f, 0.0f, 8.0f);
    Vec3f pointOffXZ(3.0f, -4.0f, 8.0f);
    JPT_ENSURE(jpt::AreValuesClose(xzPlane.DistanceSigned(pointInXZ), 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(xzPlane.DistanceSigned(pointOffXZ), -4.0f));

    // YZ plane (normal = X)
    Planef yzPlane(Vec3::Right(), 0.0f);
    Vec3f pointInYZ(0.0f, 7.0f, 2.0f);
    Vec3f pointOffYZ(6.0f, 7.0f, 2.0f);
    JPT_ENSURE(jpt::AreValuesClose(yzPlane.DistanceSigned(pointInYZ), 0.0f));
    JPT_ENSURE(jpt::AreValuesClose(yzPlane.DistanceSigned(pointOffYZ), 6.0f));

    return true;
}

export bool RunUnitTests_Plane()
{
    JPT_ENSURE(UnitTests_Plane_Construction());
    JPT_ENSURE(UnitTests_Plane_Distance());
    JPT_ENSURE(UnitTests_Plane_DistanceSigned());
    JPT_ENSURE(UnitTests_Plane_DistanceUnsigned());
    JPT_ENSURE(UnitTests_Plane_ProjectPoint());
    JPT_ENSURE(UnitTests_Plane_ProjectPointVertical());
    JPT_ENSURE(UnitTests_Plane_ProjectPointDiagonal());
    JPT_ENSURE(UnitTests_Plane_ProjectVec3());
    JPT_ENSURE(UnitTests_Plane_ProjectVec3Orthogonality());
    JPT_ENSURE(UnitTests_Plane_ThreePoints());
    JPT_ENSURE(UnitTests_Plane_ThreePointsElevated());
    JPT_ENSURE(UnitTests_Plane_ThreePointsDiagonal());
    JPT_ENSURE(UnitTests_Plane_AxisAlignedPlanes());

    return true;
}