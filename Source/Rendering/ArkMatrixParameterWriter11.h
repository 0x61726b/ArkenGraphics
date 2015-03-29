//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkMatrixParameterWriter11.h
//--------------------------------------------------------------------------------
#ifndef __ArkMatrixParameterWriter11_h__
#define __ArkMatrixParameterWriter11_h__
//--------------------------------------------------------------------------------
#include "ArkParameterWriter.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkMatrixParameterWriter11 : public ArkParameterWriter
	{
	public:
		ArkMatrixParameterWriter11();
		virtual ~ArkMatrixParameterWriter11();

		void SetRenderParameterRef( std::shared_ptr<ArkMatrixParameter11> pParam );
		void SetValue( const DirectX::XMMATRIX& Value );

		virtual void WriteParameter( IParameterManager* pParamMgr );
		virtual void InitializeParameter( );

		virtual std::shared_ptr<ArkRenderParameter11> GetRenderParameterRef();

	protected:
		std::shared_ptr<ArkMatrixParameter11>			m_pParameter;
		DirectX::XMMATRIX								m_Value;
	};
};
//--------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------
