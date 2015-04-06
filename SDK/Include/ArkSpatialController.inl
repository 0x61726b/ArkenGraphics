//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkSpatialController.inl
//--------------------------------------------------------------------------------
#include "ArkLog.h"

//--------------------------------------------------------------------------------
template <typename T>
ArkSpatialController<T>::ArkSpatialController():
m_vRotation(XMVectorSet(0.0f,0.0f,0.0f,0.0f)),
m_vTranslation(XMVectorSet(0.0f,0.0f,0.0f,0.0f))
{
}
//--------------------------------------------------------------------------------
template <typename T>
ArkSpatialController<T>::ArkSpatialController(const XMVECTOR& translation,const XMVECTOR& rotation):
m_vRotation(rotation),
m_vTranslation(translation)
{
}
//--------------------------------------------------------------------------------
template <typename T>
ArkSpatialController<T>::~ArkSpatialController()
{
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::Update(float fTime)
{
	// Set the translation and rotation parameters according to what the user has
	// set in this controller.

	if(m_pEntity)
	{

		XMFLOAT3 rot3;
		XMStoreFloat3(&rot3,m_vRotation);

		XMFLOAT3X3 mRot1;
		XMFLOAT3X3 mRot2;

		/*mRot1.RotationZ(Rot.z);*/
		float fSin = sinf(rot3.z);
		float fCos = cosf(rot3.z);

		mRot1.m[0][0] = fCos;
		mRot1.m[0][1] = fSin;
		mRot1.m[0][2] = 0.0f;

		mRot1.m[1][0] = -fSin;
		mRot1.m[1][1] = fCos;
		mRot1.m[1][2] = 0.0f;

		mRot1.m[2][0] = 0.0f;
		mRot1.m[2][1] = 0.0f;
		mRot1.m[2][2] = 1.0f;

		/*mRot2.RotationX(Rot.x);*/
		fSin = sinf(rot3.x);
		fCos = cosf(rot3.x);

		mRot2.m[0][0] = 1.0f;
		mRot2.m[0][1] = 0.0f;
		mRot2.m[0][2] = 0.0f;

		mRot2.m[1][0] = 0.0f;
		mRot2.m[1][1] = fCos;
		mRot2.m[1][2] = fSin;

		mRot2.m[2][0] = 0.0f;
		mRot2.m[2][1] = -fSin;
		mRot2.m[2][2] = fCos;


		XMMATRIX rot1;
		XMMATRIX rot2;

		rot1 = DirectX::XMLoadFloat3x3(&mRot1);
		rot2 = DirectX::XMLoadFloat3x3(&mRot2);

		rot1 = rot1*rot2;

		XMStoreFloat3x3( &mRot1,rot1 );

		/*mRot2.RotationY(Rot.y);*/
		fSin = sinf(rot3.y);
		fCos = cosf(rot3.y);

		mRot2.m[0][0] = fCos;
		mRot2.m[0][1] = 0.0f;
		mRot2.m[0][2] = -fSin;

		mRot2.m[1][0] = 0.0f;
		mRot2.m[1][1] = 1.0f;
		mRot2.m[1][2] = 0.0f;

		mRot2.m[2][0] = fSin;
		mRot2.m[2][1] = 0.0f;
		mRot2.m[2][2] = fCos;

		rot1 = DirectX::XMLoadFloat3x3(&mRot1);
		rot2 = DirectX::XMLoadFloat3x3(&mRot2);

		rot1 = rot1*rot2;



		std::wstring rotationw = L"";
		rotationw.append(std::to_wstring(rot3.x) + L" ");
		rotationw.append(std::to_wstring(rot3.y)+ L" ");
		rotationw.append(std::to_wstring(rot3.z));

		/*ArkLog::Get(LogType::Renderer).Output(rotationw);*/

		/*rot1 = DirectX::XMMatrixTranspose( rot1 );*/


		m_pEntity->Transform.Rotation() = DirectX::XMMatrixRotationY( 45 * (3.14f / 180) );

		m_pEntity->Transform.Position() = m_vTranslation;
	}

}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::SetRotation(const XMVECTOR& xyz)
{
	m_vRotation = xyz;
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::SetTranslation(const XMVECTOR& translation)
{
	m_vTranslation = translation;
}
//--------------------------------------------------------------------------------
template <typename T>
XMVECTOR& ArkSpatialController<T>::GetRotation()
{
	return m_vRotation;
}
//--------------------------------------------------------------------------------
template <typename T>
XMVECTOR& ArkSpatialController<T>::GetTranslation()
{
	return m_vTranslation;
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::RotateBy(const XMVECTOR& xyz)
{
	m_vRotation += xyz;
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::RotateXBy(const float x)
{
	m_vRotation.x += x;
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::RotateYBy(const float y)
{
	m_vRotation.y += y;
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::RotateZBy(const float z)
{
	m_vRotation.z += z;
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::TranslateBy(XMVECTOR& xyz)
{
	m_vTranslation += xyz;
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::TranslateXBy(const float x)
{
	m_vTranslation.x += x;
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::TranslateYBy(const float y)
{
	m_vTranslation.y += y;
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::TranslateZBy(const float z)
{
	m_vTranslation.z += z;
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::MoveForward(const float distance)
{
	if(m_pEntity)
	{
		XMMATRIX m = (m_pEntity->Transform.Rotation());
		XMVECTOR row = m.r[2];
		m_vTranslation += row * distance;
	}
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::MoveBackward(const float distance)
{
	if(m_pEntity)
	{
		XMMATRIX m = (m_pEntity->Transform.Rotation());
		XMVECTOR row = m.r[2];
		m_vTranslation -= row * distance;
	}
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::MoveRight(const float distance)
{
	if(m_pEntity)
	{
		XMMATRIX m = (m_pEntity->Transform.Rotation());
		XMVECTOR row = m.r[0];
		m_vTranslation += row * distance;
	}
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::MoveLeft(const float distance)
{
	if(m_pEntity)
	{
		XMMATRIX m = (m_pEntity->Transform.Rotation());
		XMVECTOR row = m.r[0];
		m_vTranslation -= row*distance;
	}
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::MoveUp(const float distance)
{
	if(m_pEntity)
	{
		XMMATRIX m = (m_pEntity->Transform.Rotation());
		XMVECTOR row = m.r[1];
		m_vTranslation += row * distance;
	}
}
//--------------------------------------------------------------------------------
template <typename T>
void ArkSpatialController<T>::MoveDown(const float distance)
{
	if(m_pEntity)
	{
		XMMATRIX m = (m_pEntity->Transform.Rotation());
		XMVECTOR row = m.r[1];
		m_vTranslation -= row * distance;
	}
}
//--------------------------------------------------------------------------------