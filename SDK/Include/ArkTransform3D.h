//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//X.h
//--------------------------------------------------------------------------------
#ifndef __ArkTransform3D_h__
#define __ArkTransform3D_h__
//--------------------------------------------------------------------------------
#include "PCH.h"
//--------------------------------------------------------------------------------
using namespace DirectX;
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkTransform3D
	{
	public:
		ArkTransform3D( );
		~ArkTransform3D( );

		XMVECTOR& Position( );
		XMMATRIX& Rotation( );
		XMVECTOR& Scale( );

		void UpdateLocal( );
		void UpdateWorld( const XMMATRIX& parent );
		void UpdateWorld( );

		const XMMATRIX& LocalMatrix( ) const;
		const XMMATRIX& WorldMatrix( ) const;
		XMMATRIX& LocalMatrix( );
		XMMATRIX& WorldMatrix( );

		void SetPosition(const XMVECTOR& V);
		XMMATRIX GetView( ) const;

		XMVECTOR LocalToWorldSpace( const XMVECTOR& input );
		XMVECTOR WorldToLocalSpace( const XMVECTOR& input );
		XMVECTOR LocalVectorToWorldSpace( const XMVECTOR& input );
		XMVECTOR LocalPointToWorldSpace( const XMVECTOR& input );
		XMVECTOR WorldVectorToLocalSpace( const XMVECTOR& input );
		XMVECTOR WorldPointToLocalSpace( const XMVECTOR& input );

	protected:

		XMVECTOR m_vTranslation;	// The translation and rotation varaibles are updated
		XMMATRIX m_mRotation;		// during the update phase and used to generate the
		XMVECTOR m_vScale;			// local matrix.  Then, the world matrix is updated
		XMMATRIX m_mWorld;			// with the new local matrix and the entity's parent
		XMMATRIX m_mLocal;			// world matrix.

	};
};
//--------------------------------------------------------------------------------
#endif // Transform3D_h
//--------------------------------------------------------------------------------
