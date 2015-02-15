//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//TaskCore.h
//--------------------------------------------------------------------------------
#ifndef __TaskCore_h__
#define __TaskCore_h__
//--------------------------------------------------------------------------------
#include "ArkRenderer11.h"
#include "PipelineManager.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class TaskCore
	{
	public:
		TaskCore();
		virtual ~TaskCore();

		virtual void Update( float dt ) = 0;

		virtual void QueuePreTasks( ArkRenderer11* pRenderer ) = 0;

		virtual void ExecuteTask( PipelineManager* pPipelineManager ) = 0;

		virtual std::wstring GetName() = 0;
	};
};
//--------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------