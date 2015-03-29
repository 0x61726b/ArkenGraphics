//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkMatrixArrayParameterWriter11.h
//--------------------------------------------------------------------------------
#ifndef __ArkMatrixArrayParameterWriter11_h__
#define __ArkMatrixArrayParameterWriter11_h__
//--------------------------------------------------------------------------------
#include "ArkParameterWriter.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkMatrixArrayParameterWriter11 : public ArkParameterWriter
	{
	public:
		ArkMatrixArrayParameterWriter11();
		virtual ~ArkMatrixArrayParameterWriter11();

		void SetRenderParameterRef( std::shared_ptr<ArkMatrixArrayParameter11> pParam );
		void SetValue( DirectX::XMMATRIX* Value );
		void SetCount( int count );

		virtual void WriteParameter( IParameterManager* pParamMgr );
		virtual void InitializeParameter( );

		virtual std::shared_ptr<ArkRenderParameter11> GetRenderParameterRef();

	protected:
		std::shared_ptr<ArkMatrixArrayParameter11>		m_pParameter;
		DirectX::XMMATRIX*								m_pValue;
		int								m_iCount;
	};
}
//--------------------------------------------------------------------------------
#endif