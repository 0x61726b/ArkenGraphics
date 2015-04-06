//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkLight.h
//--------------------------------------------------------------------------------
#ifndef __ArkLight_h__
#define __ArkLight_h__
//--------------------------------------------------------------------------------
#include "Actor.h"
#include "ArkParameterContainer.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkLight : public Actor
	{
	public:
		ArkLight();
		virtual ~ArkLight();

		void SetDiffuse( const DirectX::XMVECTOR& color );
		void SetAmbient( const DirectX::XMVECTOR& color );
		void SetSpecular( const DirectX::XMVECTOR& color );

		DirectX::XMVECTOR GetDiffuse( ) const;
		DirectX::XMVECTOR GetAmbient( ) const;
		DirectX::XMVECTOR GetSpecular( ) const;

		ArkParameterContainer Parameters;

	protected:
		std::shared_ptr<ArkVectorParameterWriter11> m_DiffuseWriter;
		std::shared_ptr<ArkVectorParameterWriter11> m_AmbientWriter;
		std::shared_ptr<ArkVectorParameterWriter11> m_SpecularWriter;
		std::shared_ptr<ArkVectorParameterWriter11> m_PositionWriter;
	};
};

//--------------------------------------------------------------------------------
#endif