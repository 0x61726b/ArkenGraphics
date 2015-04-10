//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// TGrowableVertexBufferDX11
//
//--------------------------------------------------------------------------------
#ifndef TGrowableVertexBufferDX11_h
#define TGrowableVertexBufferDX11_h
//--------------------------------------------------------------------------------
#include "PipelineManager.h"
#include "TGrowableBuffer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	template <class T>
	class TGrowableVertexBuffer11 : public TGrowableBuffer11<T>
	{
	public:
		TGrowableVertexBuffer11();
		virtual ~TGrowableVertexBuffer11();

		virtual void UploadData( PipelineManager* pPipeline );
		virtual ResourcePtr GetBuffer();

	protected:
        virtual void CreateResource( unsigned int elements );
        virtual void DeleteResource( );

	private:
		ResourcePtr m_VB;
	};

#include "TGrowableVertexBuffer11.inl"
};
//--------------------------------------------------------------------------------
#endif // TGrowableVertexBufferDX11_h
//--------------------------------------------------------------------------------