//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkParameterWriter.h
//--------------------------------------------------------------------------------
#include "ArkRenderParameter11.h"
#include "IParameterManager.h"
//--------------------------------------------------------------------------------
#ifndef __ArkParameterWriter_h__
#define __ArkParameterWriter_h__
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkParameterWriter
	{
	public:
		ArkParameterWriter();
		virtual ~ArkParameterWriter();

		virtual std::shared_ptr<ArkRenderParameter11> GetRenderParameterRef() = 0;

		virtual void WriteParameter( IParameterManager* pParamMgr ) = 0;

		virtual void InitializeParameter( ) = 0;
	};
};



//--------------------------------------------------------------------------------
#endif