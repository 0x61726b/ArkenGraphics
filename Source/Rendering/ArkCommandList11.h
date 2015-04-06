//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkCommandList11.h
//--------------------------------------------------------------------------------
#ifndef __ArkCommandList_h__
#define __ArkCommandList_h__
//--------------------------------------------------------------------------------
#include "ArkRenderer11.h"
#include "PipelineManager.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkCommandList11
	{
	public:
		ArkCommandList11();
		virtual ~ArkCommandList11();

		bool ListAvailable();
		void ReleaseList();

	protected:
		ID3D11CommandList*			m_pList;

		friend PipelineManager;
		friend ArkRenderer11;
	};
};

//--------------------------------------------------------------------------------
#endif