//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkSphere3.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkSphere3.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
using namespace DirectX;
//--------------------------------------------------------------------------------
ArkSphere3::ArkSphere3( )
{
	Center = XMVectorSet( 0.0f, 0.0f, 0.0f,0.0f );
	Radius = 0.0f;
}
//--------------------------------------------------------------------------------
ArkSphere3::ArkSphere3( const XMVECTOR& center, float radius )
{
	Update( center, radius );
}
//--------------------------------------------------------------------------------
ArkSphere3::~ArkSphere3()
{
}
//--------------------------------------------------------------------------------
void ArkSphere3::Update( const XMVECTOR& center, float radius ) 
{
	Center = center;
	Radius = radius;
}
//--------------------------------------------------------------------------------
bool ArkSphere3::Intersects( const ArkSphere3& Test ) const
{
	XMVECTOR Dist;
	Dist = ( Center - Test.Center ); 
	
	XMVECTOR mag = XMVector3Length(Dist);
	float magnitude;
	XMStoreFloat( &magnitude,mag );

	return( magnitude < ( Radius + Test.Radius ) );
}
//--------------------------------------------------------------------------------
bool ArkSphere3::Envelops( const ArkSphere3& test ) const
{
	XMVECTOR Dist;
	Dist = ( Center - test.Center ); 

	XMVECTOR mag = XMVector3Length(Dist);
	float magnitude;
	XMStoreFloat( &magnitude,mag );
	
	return( Radius > test.Radius + magnitude );
}
//--------------------------------------------------------------------------------
void ArkSphere3::UpdateCenter( const XMVECTOR& center )
{
	Center = center;
}
//--------------------------------------------------------------------------------
void ArkSphere3::UpdateRadius( float radius )
{
	Radius = radius;
}
//--------------------------------------------------------------------------------
eEShape ArkSphere3::GetShapeType( ) const
{
	return( SPHERE );
}
//--------------------------------------------------------------------------------
void ArkSphere3::SamplePosition( XMVECTOR& position, float theta, float phi ) const
{
	XMFLOAT3 pos3;
	XMStoreFloat3(&pos3,position);

	XMFLOAT3 center3;
	XMStoreFloat3(&center3,Center);

	pos3.x = Radius * sinf( phi ) * cosf( theta ) + center3.x;
	pos3.y = Radius * cosf( phi ) + center3.y;
	pos3.z = Radius * sinf( phi ) * sinf( theta ) + center3.z;

	position = XMLoadFloat3( &pos3 );
}
//--------------------------------------------------------------------------------
void ArkSphere3::SampleNormal( XMVECTOR& normal, float theta, float phi ) const
{
	XMFLOAT3 norm3;
	XMStoreFloat3(&norm3,normal);

	norm3.x = Radius * sinf( phi ) * cosf( theta );
	norm3.y = Radius * cosf( phi );
	norm3.z = Radius * sinf( phi ) * sinf( theta );

	
	normal = XMLoadFloat3( &norm3);
	normal = XMVector3Normalize( normal );
}
//--------------------------------------------------------------------------------
void ArkSphere3::SamplePositionAndNormal( XMVECTOR& position, XMVECTOR& normal, float theta, float phi ) const
{
	XMFLOAT3 norm3;
	XMStoreFloat3(&norm3,normal);

	norm3.x = Radius * sinf( phi ) * cosf( theta );
	norm3.y = Radius * cosf( phi );
	norm3.z = Radius * sinf( phi ) * sinf( theta );
	normal = XMLoadFloat3( &norm3);

	position = normal + Center;

	normal = XMVector3Normalize( normal );
}
//--------------------------------------------------------------------------------