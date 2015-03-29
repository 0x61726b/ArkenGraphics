//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkConstantBufferParameterWriter11.h
//--------------------------------------------------------------------------------
#ifndef __ArkVectorParameterWriter11_h__
#define __ArkVectorParameterWriter11_h__
//--------------------------------------------------------------------------------
#include "ArkParameterWriter.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkVectorParameterWriter11 : public ArkParameterWriter
	{
	public:
		ArkVectorParameterWriter11();
		virtual ~ArkVectorParameterWriter11();

		void SetRenderParameterRef( std::shared_ptr<ArkVectorParameter11> pParam );
		void SetValue( const DirectX::XMVECTOR& Value );
		DirectX::XMVECTOR GetValue();

		virtual void WriteParameter( IParameterManager* pParamMgr );
		virtual void InitializeParameter( );

		virtual std::shared_ptr<ArkRenderParameter11> GetRenderParameterRef();

	protected:
		std::shared_ptr<ArkVectorParameter11>			m_pParameter;
		DirectX::XMVECTOR								m_Value;
	};
};
//--------------------------------------------------------------------------------
#endif