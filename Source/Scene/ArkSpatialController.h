//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkSpatialController.h
//--------------------------------------------------------------------------------
#ifndef __ArkSpatialController_h__
#define __ArkSpatialController_h__
//--------------------------------------------------------------------------------
#include "IController.h"
#include "ArkEntity3D.h"
#include "ArkLog.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	template <typename T>
	class ArkSpatialController : public IController<T>
	{
	public:
		ArkSpatialController( );
		ArkSpatialController( const DirectX::XMVECTOR& translation, const DirectX::XMVECTOR& rotation );
		virtual ~ArkSpatialController( );

		virtual void Update( float fTime );

		void SetRotation( const XMVECTOR& xyz );
		void SetTranslation( const XMVECTOR& translation );

		DirectX::XMVECTOR& GetRotation();
		DirectX::XMVECTOR& GetTranslation();

		void RotateBy( const DirectX::XMVECTOR& xyz );
		void RotateXBy( const float x );
		void RotateYBy( const float y );
		void RotateZBy( const float z );

		void TranslateBy( DirectX::XMVECTOR& xyz );
		void TranslateXBy( const float x );
		void TranslateYBy( const float y );
		void TranslateZBy( const float z );

		void MoveForward( const float distance );
		void MoveBackward( const float distance );
		void MoveRight( const float distance );
		void MoveLeft( const float distance );
		void MoveUp( const float distance );
		void MoveDown( const float distance );

	protected:
		XMVECTOR		m_vRotation;
		XMVECTOR		m_vTranslation;
	};

	#include "ArkSpatialController.inl"
};
//--------------------------------------------------------------------------------
#endif // __ArkSpatialController_h__
