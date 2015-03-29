//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkFrustum.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
using namespace DirectX;
//--------------------------------------------------------------------------------
ArkFrustum::ArkFrustum()
{
	for (int i = 0; i < 6; i++)
	{
		m_Planes[i].a() = 0.0f;
		m_Planes[i].b() = 0.0f;
		m_Planes[i].c() = 0.0f;
		m_Planes[i].d() = 0.0f;
	}
}
//--------------------------------------------------------------------------------
ArkFrustum::ArkFrustum( const XMMATRIX& ViewProjection )
{
	Update( ViewProjection, true );
}
//--------------------------------------------------------------------------------
ArkFrustum::~ArkFrustum()
{
}
//--------------------------------------------------------------------------------
void ArkFrustum::Update( const XMMATRIX& ViewProj, bool bNormalize )
{
	XMFLOAT4X4 tmp;
	XMStoreFloat4x4( &tmp, ViewProj );
	float j = tmp.m[0][0]; // or float j = tmp._11

	// Left Plane
	float a = tmp.m[0][3] + tmp.m[0][0];
	float b = tmp.m[1][3] + tmp.m[1][0];
	float c = tmp.m[2][3] + tmp.m[2][0];
	float d = tmp.m[3][3] + tmp.m[3][0];

	m_Planes[0].a() = a;
	m_Planes[0].b() = b;
	m_Planes[0].c() = c;
	m_Planes[0].d() = d;

	

	//// Right Plane
	a = tmp.m[0][3] + tmp.m[0][0];
	b = tmp.m[1][3] + tmp.m[1][0];
	c = tmp.m[2][3] + tmp.m[2][0];
	d = tmp.m[3][3] + tmp.m[3][0];
	m_Planes[1].a() = a;
	m_Planes[1].b() = b;
	m_Planes[1].c() = c;
	m_Planes[1].d() = d;

	//// Top Plane
	a = tmp.m[0][3] + tmp.m[0][1];
	b = tmp.m[1][3] + tmp.m[1][1];
	c = tmp.m[2][3] + tmp.m[2][1];
	d = tmp.m[3][3] + tmp.m[3][1];
	m_Planes[2].a() = a;
	m_Planes[2].b() = b;
	m_Planes[2].c() = c;
	m_Planes[2].d() = d;


	//// Bottom Plane
	a = tmp.m[0][3] + tmp.m[0][1];
	b = tmp.m[1][3] + tmp.m[1][1];
	c = tmp.m[2][3] + tmp.m[2][1];
	d = tmp.m[3][3] + tmp.m[3][1];
	m_Planes[3].a() = a;
	m_Planes[3].b() = b;
	m_Planes[3].c() = c;
	m_Planes[3].d() = d;

	//// Near Plane
	a = tmp.m[0][2];
	b = tmp.m[1][2];
	c = tmp.m[2][2];
	d = tmp.m[3][2];
	m_Planes[4].a() = a;
	m_Planes[4].b() = b;
	m_Planes[4].c() = c;
	m_Planes[4].d() = d;

	//// Far Plane
	a = tmp.m[0][3] - tmp.m[0][2];
	b = tmp.m[1][3] - tmp.m[1][2];
	c = tmp.m[2][3] - tmp.m[2][2];
	d = tmp.m[3][3] - tmp.m[3][2];
	m_Planes[5].a() = a;
	m_Planes[5].b() = b;
	m_Planes[5].c() = c;
	m_Planes[5].d() = d;


	// Normalize all planes
	if ( bNormalize )
	{
		for (int i = 0; i < 6; i++)
		{
			m_Planes[i].Normalize();
		}
	}
}
//--------------------------------------------------------------------------------
bool ArkFrustum::Test( const XMVECTOR& TestPoint ) const
{
	// Test the point against each plane
	if ( m_Planes[0].DistanceToPoint( TestPoint ) < 0 )
		return(false);

	if ( m_Planes[1].DistanceToPoint( TestPoint ) < 0 )
		return(false);

	if ( m_Planes[2].DistanceToPoint( TestPoint ) < 0 )
		return(false);

	if ( m_Planes[3].DistanceToPoint( TestPoint ) < 0 )
		return(false);

	if ( m_Planes[4].DistanceToPoint( TestPoint ) < 0 )
		return(false);

	if ( m_Planes[5].DistanceToPoint( TestPoint ) < 0 )
		return(false);

	// If all tests passed, point is within the frustum
	return( true );
}
//--------------------------------------------------------------------------------
bool ArkFrustum::Test( const ArkSphere3& TestSphere ) const
{
	// Test the center against each plane and compare the radius
	
	float fTemp = 0.0f;
	
	for (int i = 0; i < 6; i++)
	{
		fTemp = m_Planes[i].DistanceToPoint( TestSphere.Center );
		if ( fTemp < -TestSphere.Radius )
			return( false );

		if ( float(fabs(fTemp)) < TestSphere.Radius )
			return( true );
	}

	// If all tests passed, sphere is at least intersecting the frustum
	return( true );
}
//--------------------------------------------------------------------------------
bool ArkFrustum::Intersects( const ArkSphere3& bounds ) const
{
	// distance to point plus the radius must be greater than zero to be intersecting!!!

	for ( int i = 0; i < 6; i++ )
	{
		if ( m_Planes[i].DistanceToPoint( bounds.Center ) + bounds.Radius < 0 )
			return( false );
	}

	// must not be enveloped if it is intersecting

	//if ( Envelops( bounds ) )
	//	return( false );

	// if all tests passed, then sphere is enveloped

	return( true );
}
//--------------------------------------------------------------------------------
bool ArkFrustum::Envelops( const ArkSphere3& bounds ) const
{
	// distance to point minus the radius must be greater than zero to be enveloped!!!

	for ( int i = 0; i < 6; i++ )
	{
		if (m_Planes[i].DistanceToPoint( bounds.Center ) - bounds.Radius < 0)
			return( false );
	}

	// If all tests passed, sphere is enveloped

	return( true );
}
//--------------------------------------------------------------------------------
eEShape ArkFrustum::GetShapeType( ) const
{
	return( FRUSTUM );
}
//--------------------------------------------------------------------------------