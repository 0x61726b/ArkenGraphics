//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkGeometryGenerator11.h
//--------------------------------------------------------------------------------
#ifndef __ArkSamplerParameterWriter11_h__
#define __ArkSamplerParameterWriter11_h__
//--------------------------------------------------------------------------------
#include "ArkParameterWriter.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkSamplerParameterWriter11 : public ArkParameterWriter
	{
	public:
		ArkSamplerParameterWriter11();
		virtual ~ArkSamplerParameterWriter11();

		void SetRenderParameterRef( std::shared_ptr<ArkSamplerParameter11> pParam );
		void SetValue( int Value );

		virtual void WriteParameter( IParameterManager* pParamMgr );
		virtual void InitializeParameter( );

		virtual std::shared_ptr<ArkRenderParameter11> GetRenderParameterRef();

	protected:
		std::shared_ptr<ArkSamplerParameter11>	m_pParameter;
		int										m_Value;
	};
};
//--------------------------------------------------------------------------------
#endif // SamplerParameterWriterDX11_h
//--------------------------------------------------------------------------------

