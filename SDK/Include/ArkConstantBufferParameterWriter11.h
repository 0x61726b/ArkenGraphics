//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkConstantBufferParameterWriter11.h
//--------------------------------------------------------------------------------
#ifndef __ArkConstantBufferParameterWriter11_h__
#define __ArkConstantBufferParameterWriter11_h__
//--------------------------------------------------------------------------------
#include "ArkParameterWriter.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkConstantBufferParameterWriter11 : public ArkParameterWriter
	{
	public:
		ArkConstantBufferParameterWriter11();
		virtual ~ArkConstantBufferParameterWriter11();

		void SetRenderParameterRef( std::shared_ptr<ArkConstantBufferParameter11> pParam );
		void SetValue( ResourcePtr Value );

		virtual void WriteParameter( IParameterManager* pParamMgr );
		virtual void InitializeParameter( );

		virtual std::shared_ptr<ArkRenderParameter11> GetRenderParameterRef();

	protected:
		std::shared_ptr<ArkConstantBufferParameter11>	m_pParameter;
		ResourcePtr										m_Value;
	};
};
//--------------------------------------------------------------------------------
#endif