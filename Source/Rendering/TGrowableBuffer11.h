//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//TGrowableBuffer11.h
//--------------------------------------------------------------------------------
#ifndef __TGrowableBuffer11_h__
#define __TGrowableBuffer11_h__
//--------------------------------------------------------------------------------
#include "PipelineManager.h"
#include "ArkBuffer11Config.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	template <class T>
	class TGrowableBuffer11
	{
	public:
		TGrowableBuffer11();
		virtual ~TGrowableBuffer11();

		// Setting the size of the buffer will create a new array, and 
		// copy as much of the existing array as possible.

		void SetMaxElementCount( unsigned int max );
		
		unsigned int GetMaxElementCount();
		unsigned int GetElementCount();

		
		// Elements are added one at a time, with a template method.

		void AddElement( const T& element );
		

		// These methods allow the user to either upload the data to
		// the buffer resource, or alternatively they can 'reset' the 
		// data, which essentially just resets the element counters
		// (i.e. - no data is actually cleared from the system memory
		// array).  Each subclass is responsible for properly implementing
        // the upload method.

		virtual void UploadData( PipelineManager* pPipeline ) = 0;
		void ResetData();

		virtual ResourcePtr GetBuffer() = 0;

        // These methods are used internally by the growable buffer to allocate 
		// and deallocate the resources to be used.  This lets each subclass
		// determine the type and number of resources to use, giving very good 
		// future flexibility.  Because we leave this flexible for the subclasses
        // to implement, we also include the GetBuffer method into the required
        // methods to implement.

	protected:
        virtual void CreateResource( unsigned int elements ) = 0;
        virtual void DeleteResource( ) = 0;

		void EnsureCapacity( );

		// The sizes
		unsigned int m_uiMaxElementCount;
		unsigned int m_uiElementCount;

		bool m_bUploadNeeded;

		// The pointer to our array of vertex data
		T* m_pDataArray;
	};

#include "TGrowableBuffer11.inl"
};
//--------------------------------------------------------------------------------
#endif // TGrowableBufferDX11_h
//--------------------------------------------------------------------------------