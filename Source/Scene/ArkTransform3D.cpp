//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkTransform3D.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkTransform3D.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
using namespace DirectX;
//--------------------------------------------------------------------------------
ArkTransform3D::ArkTransform3D()
{
	m_vTranslation = XMVectorZero();
	XMMATRIX i = XMMatrixIdentity();
	m_mRotation = i;
	m_vScale = XMVectorSet(1.0f,1.0f,1.0f,1.0f);

	m_mWorld = i;
	m_mLocal = i;
	
}
//--------------------------------------------------------------------------------
ArkTransform3D::~ArkTransform3D()
{
	
}
//--------------------------------------------------------------------------------
void ArkTransform3D::SetPosition(const XMVECTOR& V)
{
	m_vTranslation = V;
}
//--------------------------------------------------------------------------------
XMVECTOR& ArkTransform3D::Position()
{
	return(m_vTranslation);
}
//--------------------------------------------------------------------------------
XMMATRIX& ArkTransform3D::Rotation()
{
	return(m_mRotation);
}
//--------------------------------------------------------------------------------
XMVECTOR& ArkTransform3D::Scale()
{
	return(m_vScale);
}
//--------------------------------------------------------------------------------
void ArkTransform3D::UpdateLocal()
{
	// Load the local space matrix with the rotation and translation components.
	XMMATRIX identity = XMMatrixIdentity();
	m_mLocal = identity;

	XMMATRIX scale = XMMatrixScalingFromVector(m_vScale);

	XMMATRIX translate = XMMatrixTranslationFromVector(m_vTranslation);

	
	XMMATRIX mRot = m_mRotation;
	m_mLocal = scale*mRot*translate;
	
}
//--------------------------------------------------------------------------------
void ArkTransform3D::UpdateWorld(const XMMATRIX& parent)
{
	XMMATRIX w = m_mLocal;
	m_mWorld = w*parent;
}
//--------------------------------------------------------------------------------
void ArkTransform3D::UpdateWorld()
{
	// If no parent matrix is available, then simply make the world matrix the
	// local matrix.
	m_mWorld = m_mLocal;
}
//--------------------------------------------------------------------------------
const XMMATRIX& ArkTransform3D::WorldMatrix() const
{
	return(m_mWorld);
}
//--------------------------------------------------------------------------------
const XMMATRIX& ArkTransform3D::LocalMatrix() const
{
	return(m_mLocal);
}
//--------------------------------------------------------------------------------
XMMATRIX& ArkTransform3D::WorldMatrix()
{
	return(m_mWorld);
}
//--------------------------------------------------------------------------------
XMMATRIX& ArkTransform3D::LocalMatrix()
{
	return(m_mLocal);
}
//--------------------------------------------------------------------------------
XMMATRIX ArkTransform3D::GetView() const
{
	XMVECTOR Eye;
	XMVECTOR At;
	XMVECTOR Up;

	XMMATRIX m = m_mWorld;
	XMVECTOR scale;
	XMVECTOR translation;
	XMVECTOR rotation;
	XMMatrixDecompose(&scale,&rotation,&translation,m);

	XMFLOAT4X4 tmp;
	XMStoreFloat4x4( &tmp,m_mWorld );

	XMFLOAT3 zBasis = XMFLOAT3(tmp._31,tmp._32,tmp._33 );

	XMFLOAT3 up = XMFLOAT3(tmp._21,tmp._22,tmp._23);

	Eye = translation;
	At = translation + XMVectorSet(zBasis.x,zBasis.y,zBasis.z,1.0f );
	Up = XMVectorSet(up.x,up.y,up.z,0.0f);

	
	return( XMMatrixLookAtLH(Eye,At,Up) );
}
//--------------------------------------------------------------------------------
XMVECTOR ArkTransform3D::LocalToWorldSpace(const XMVECTOR& input)
{
	XMMATRIX m = m_mWorld;
	XMVECTOR result = XMVector4Transform(input,m);

	return(result);
}
//--------------------------------------------------------------------------------
XMVECTOR ArkTransform3D::WorldToLocalSpace(const XMVECTOR& input)
{
	XMMATRIX m = m_mWorld;
	XMVECTOR det;
	XMMATRIX inverseView = XMMatrixInverse(&det,m);

	XMVECTOR result = XMVector4Transform(input,inverseView);

	return(result);
}
//--------------------------------------------------------------------------------
XMVECTOR ArkTransform3D::LocalVectorToWorldSpace(const XMVECTOR& input) //Vector3
{
	XMFLOAT3 r;
	XMStoreFloat3(&r,input);
	XMVECTOR result = LocalToWorldSpace(XMVectorSet(r.x,r.y,r.z,0.0f));

	return(result);
}
//--------------------------------------------------------------------------------
XMVECTOR ArkTransform3D::LocalPointToWorldSpace(const XMVECTOR& input) //Vector3
{
	XMFLOAT3 r;
	XMStoreFloat3(&r,input);
	XMVECTOR result = LocalToWorldSpace(XMVectorSet(r.x,r.y,r.z,1.0f));

	return(result);
}
//--------------------------------------------------------------------------------
XMVECTOR ArkTransform3D::WorldVectorToLocalSpace(const XMVECTOR& input) //Vector3
{
	XMFLOAT3 r;
	XMStoreFloat3(&r,input);
	XMVECTOR result = WorldToLocalSpace(XMVectorSet(r.x,r.y,r.z,0.0f));

	return(result);
}
//--------------------------------------------------------------------------------
XMVECTOR ArkTransform3D::WorldPointToLocalSpace(const XMVECTOR& input)
{
	XMFLOAT3 r;
	XMStoreFloat3(&r,input);
	XMVECTOR result = WorldToLocalSpace(XMVectorSet(r.x,r.y,r.z,1.0f));

	return(result);
}
//--------------------------------------------------------------------------------