//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//PipelineExecutor11.h
//--------------------------------------------------------------------------------
#ifndef __TGrowableIndexBuffer11_h__
#define __TGrowableIndexBuffer11_h__
//--------------------------------------------------------------------------------
#include "PipelineManager.h"
#include "TGrowableBuffer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	template <class T>
	class TGrowableIndexBuffer11 : public TGrowableBuffer11<T>
	{
	public:
		TGrowableIndexBuffer11();
		virtual ~TGrowableIndexBuffer11();

		virtual void UploadData( PipelineManager* pPipeline );
		virtual ResourcePtr GetBuffer();

	protected:
        virtual void CreateResource( unsigned int elements );
        virtual void DeleteResource( );

	private:
    	ResourcePtr m_IB;
	};

#include "TGrowableIndexBuffer11.inl"
};
//--------------------------------------------------------------------------------
#endif // TGrowableIndexBufferDX11_h
//--------------------------------------------------------------------------------