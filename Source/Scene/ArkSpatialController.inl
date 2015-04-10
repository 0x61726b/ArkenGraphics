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
		/*XMFLOAT3 rot3 = XMFLOAT3(0,0,0);
		std::wstring rotationw = L"";
		rotationw.append(std::to_wstring(rot3.x) + L" ");
		rotationw.append(std::to_wstring(rot3.y)+ L" ");
		rotationw.append(std::to_wstring(rot3.z));

		ArkLog::Get(LogType::Renderer).Output(rotationw);*/

		/*rot1 = DirectX::XMMatrixTranspose( rot1 );*/

		
		XMMATRIX rotationx = DirectX::XMMatrixRotationRollPitchYawFromVector( m_vRotation );
		
		
		m_pEntity->Transform.Rotation()  = rotationx;

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