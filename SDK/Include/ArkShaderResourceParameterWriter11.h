//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//__ArkShaderResourceParameterWriter11.h
//--------------------------------------------------------------------------------
#ifndef __ArkShaderResourceParameterWriter11_h__
#define __ArkShaderResourceParameterWriter11_h__
//--------------------------------------------------------------------------------
#include "ArkParameterWriter.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkShaderResourceParameterWriter11 : public ArkParameterWriter
	{
	public:
		ArkShaderResourceParameterWriter11();
		virtual ~ArkShaderResourceParameterWriter11();

		void SetRenderParameterRef( std::shared_ptr<ArkShaderResourceParameter11> pParam );
		void SetValue( ResourcePtr Value );

		virtual void WriteParameter( IParameterManager* pParamMgr );
		virtual void InitializeParameter( );

		virtual std::shared_ptr<ArkRenderParameter11> GetRenderParameterRef();

	protected:
		std::shared_ptr<ArkShaderResourceParameter11>	m_pParameter;
		ResourcePtr						m_Value;

	};
};
//--------------------------------------------------------------------------------
#endif