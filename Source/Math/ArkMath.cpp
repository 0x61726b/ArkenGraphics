//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkMath.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkMath.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkSphere3 ArkMath::ComputeBoundingSphereFromPoints(const XMFLOAT3* points, UINT numPoints, UINT stride)
{
	ArkSphere3 sphere;

    /*XMASSERT(numPoints > 0);
    XMASSERT(points);*/

    // Find the points with minimum and maximum x, y, and z
    XMVECTOR MinX, MaxX, MinY, MaxY, MinZ, MaxZ;

    MinX = MaxX = MinY = MaxY = MinZ = MaxZ = XMLoadFloat3(points);

    for(UINT i = 1; i < numPoints; i++)
    {
        XMVECTOR Point = XMLoadFloat3((XMFLOAT3*)((BYTE*)points + i * stride));

        float px = XMVectorGetX(Point);
        float py = XMVectorGetY(Point);
        float pz = XMVectorGetZ(Point);

        if(px < XMVectorGetX(MinX))
            MinX = Point;

        if(px > XMVectorGetX(MaxX))
            MaxX = Point;

        if(py < XMVectorGetY(MinY))
            MinY = Point;

        if(py > XMVectorGetY(MaxY))
            MaxY = Point;

        if(pz < XMVectorGetZ(MinZ))
            MinZ = Point;

        if(pz > XMVectorGetZ(MaxZ))
            MaxZ = Point;
    }

    // Use the min/max pair that are farthest apart to form the initial sphere.
    XMVECTOR DeltaX = MaxX - MinX;
    XMVECTOR DistX = XMVector3Length(DeltaX);

    XMVECTOR DeltaY = MaxY - MinY;
    XMVECTOR DistY = XMVector3Length(DeltaY);

    XMVECTOR DeltaZ = MaxZ - MinZ;
    XMVECTOR DistZ = XMVector3Length(DeltaZ);

    XMVECTOR Center;
    XMVECTOR Radius;

    if(XMVector3Greater(DistX, DistY))
    {
        if(XMVector3Greater(DistX, DistZ))
        {
            // Use min/max x.
            Center = (MaxX + MinX) * 0.5f;
            Radius = DistX * 0.5f;
        }
        else
        {
            // Use min/max z.
            Center = (MaxZ + MinZ) * 0.5f;
            Radius = DistZ * 0.5f;
        }
    }
    else // Y >= X
    {
        if(XMVector3Greater(DistY, DistZ))
        {
            // Use min/max y.
            Center = (MaxY + MinY) * 0.5f;
            Radius = DistY * 0.5f;
        }
        else
        {
            // Use min/max z.
            Center = (MaxZ + MinZ) * 0.5f;
            Radius = DistZ * 0.5f;
        }
    }

    // Add any points not inside the sphere.
    for(UINT i = 0; i < numPoints; i++)
    {
        XMVECTOR Point = XMLoadFloat3((XMFLOAT3*)((BYTE*)points + i * stride));

        XMVECTOR Delta = Point - Center;

        XMVECTOR Dist = XMVector3Length(Delta);

        if(XMVector3Greater(Dist, Radius))
        {
            // Adjust sphere to include the new point.
            Radius = (Radius + Dist) * 0.5f;
            Center += (XMVectorReplicate(1.0f) - Radius * XMVectorReciprocal(Dist)) * Delta;
        }
    }

    sphere.Center =  Center;
	XMFLOAT3 r;
	XMStoreFloat3( &r, Radius );
    sphere.Radius =  r.x;

    return sphere;
}
//--------------------------------------------------------------------------------
void ArkMath::ComputeFrustum( const XMMATRIX& viewProj, ArkFrustum& frustum )
{
	XMVECTOR det;
    XMMATRIX invViewProj = XMMatrixInverse(&det, viewProj);

    // Corners in homogeneous clip space
    XMVECTOR corners[8] =
    {                                               //                         7--------6
        XMVectorSet( 1.0f, -1.0f, 0.0f, 1.0f),      //                        /|       /|
        XMVectorSet(-1.0f, -1.0f, 0.0f, 1.0f),      //     Y ^               / |      / |
        XMVectorSet( 1.0f,  1.0f, 0.0f, 1.0f),      //     | _              3--------2  |
        XMVectorSet(-1.0f,  1.0f, 0.0f, 1.0f),      //     | /' Z           |  |     |  |
        XMVectorSet( 1.0f, -1.0f, 1.0f, 1.0f),      //     |/               |  5-----|--4
        XMVectorSet(-1.0f, -1.0f, 1.0f, 1.0f),      //     + ---> X         | /      | /
        XMVectorSet( 1.0f,  1.0f, 1.0f, 1.0f),      //                      |/       |/
        XMVectorSet(-1.0f,  1.0f, 1.0f, 1.0f),      //                      1--------0
    };

    // Convert to world space
    for(UINT i = 0; i < 8; ++i)
        corners[i] = XMVector3TransformCoord(corners[i], invViewProj);



    // Calculate the 6 planes
    frustum.m_Planes[0] = ArkPlane( XMPlaneFromPoints(corners[0], corners[4], corners[2]) );
    frustum.m_Planes[1] = ArkPlane( XMPlaneFromPoints(corners[1], corners[3], corners[5]) );
    frustum.m_Planes[2] = ArkPlane( XMPlaneFromPoints(corners[3], corners[2], corners[7]) );
    frustum.m_Planes[3] = ArkPlane( XMPlaneFromPoints(corners[1], corners[5], corners[0]) );
    frustum.m_Planes[4] = ArkPlane( XMPlaneFromPoints(corners[5], corners[7], corners[4]) );
    frustum.m_Planes[5] = ArkPlane( XMPlaneFromPoints(corners[1], corners[0], corners[3]) );
}
//--------------------------------------------------------------------------------
UINT ArkMath::TestFrustumSphere( const ArkFrustum& frustum, const ArkSphere3& sphere )
{
	XMVECTOR sphereCenter = sphere.Center;

    UINT result = 1;
    for(UINT i = 0; i < 6; i++) {
		ArkPlane p = frustum.m_Planes[i];
		XMVECTOR pV = XMVectorSet(p.a(),p.b(),p.c(),p.d());
        float distance = XMVectorGetX(XMPlaneDotCoord(pV, sphereCenter));

        if (distance < -sphere.Radius)
            return 0;
        else if (distance < sphere.Radius)
            result =  1;
    }

    return result;
}
//--------------------------------------------------------------------------------