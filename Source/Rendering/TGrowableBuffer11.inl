//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//TGrowableBuffer11.inl
//--------------------------------------------------------------------------------
template <class T>
TGrowableBuffer11<T>::TGrowableBuffer11() :
	m_uiMaxElementCount( 0 ),
	m_uiElementCount( 0 ),
	m_bUploadNeeded( false ),
	m_pDataArray( nullptr )
{
	// Initialize our buffer to a reasonable size
//	SetMaxElementCount( 128 );
}
//--------------------------------------------------------------------------------
template <class T>
TGrowableBuffer11<T>::~TGrowableBuffer11()
{
	SAFE_DELETE_ARRAY( m_pDataArray );
}
//--------------------------------------------------------------------------------
template <class T>
void TGrowableBuffer11<T>::SetMaxElementCount( unsigned int max )
{
	// if this is different than the current size, then release the existing
	// local array, plus resize the resource.

	if ( max != m_uiMaxElementCount ) {

		// Create the new array, temporarily in a local variable.
		T* pNewArray = new T[max];

		// Truncate the vertex count if more than the new max are already loaded 
		// which could happen if the array is being shrunken.
		if ( m_uiElementCount > max ) {
			m_uiElementCount = max;
		}

		// Copy the existing vertex data over, if any has been added.
		if ( m_uiElementCount > 0 ) {
			memcpy( pNewArray, m_pDataArray, m_uiElementCount * sizeof( T ) );
		}

		// Remember the maximum number of vertices to allow, and the 
		// current count of vertices is left as it is.
		m_uiMaxElementCount = max;

		// Release system memory for the old array so that we can set a new one
		SAFE_DELETE_ARRAY( m_pDataArray );
		m_pDataArray = pNewArray;

		// Delete the existing resource if it already existed
		DeleteResource( );
//		if ( nullptr != m_Buffer ) {
//			RendererDX11::Get()->DeleteResource( m_Buffer );
//			m_Buffer = nullptr;
//		}

		// Create the new vertex buffer, with the dynamic flag set to true
		CreateResource( m_uiMaxElementCount );
//		BufferConfigDX11 vbuffer;
//		vbuffer.SetDefaultVertexBuffer( m_uiMaxElementCount * sizeof( T ), true );
//		m_Buffer = RendererDX11::Get()->CreateVertexBuffer( &vbuffer, nullptr );

		m_bUploadNeeded = true;
	}
}
//--------------------------------------------------------------------------------
template <class T>
unsigned int TGrowableBuffer11<T>::GetMaxElementCount()
{
	return( m_uiMaxElementCount );
}
//--------------------------------------------------------------------------------
template <class T>
unsigned int TGrowableBuffer11<T>::GetElementCount()
{
	return( m_uiElementCount );
}
//--------------------------------------------------------------------------------
template <class T>
void TGrowableBuffer11<T>::AddElement( const T& element )
{
	EnsureCapacity( );

	m_pDataArray[m_uiElementCount] = element;
	m_uiElementCount++;

	m_bUploadNeeded = true;
}
//--------------------------------------------------------------------------------
//template <class T>
//void TGrowableBufferDX11<T>::UploadData( PipelineManagerDX11* pPipeline )
//{
//	if ( m_uiElementCount > 0 && m_bUploadNeeded == true ) {
//
//		m_bUploadNeeded = false;
//
//		// Map the vertex buffer for writing
//
//		D3D11_MAPPED_SUBRESOURCE resource = 
//			pPipeline->MapResource( m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0 );
//
//		// Only copy as much of the data as you actually have filled up
//	
//		memcpy( resource.pData, m_pDataArray, m_uiElementCount * sizeof( T ) );
//
//		// Unmap the vertex buffer
//
//		pPipeline->UnMapResource( m_Buffer, 0 );
//	}
//}
//--------------------------------------------------------------------------------
template <class T>
void TGrowableBuffer11<T>::ResetData()
{
	// Reset the vertex count here to prepare for the next drawing pass.
	m_uiElementCount = 0;

	m_bUploadNeeded = false;
}
//--------------------------------------------------------------------------------
template <class T>
void TGrowableBuffer11<T>::EnsureCapacity( )
{
	// If the next vertex would put us over the limit, then resize the array.
	if ( m_uiElementCount + 1 >= m_uiMaxElementCount ) {
		SetMaxElementCount( m_uiMaxElementCount + 1024 );
	}
}
//--------------------------------------------------------------------------------
//template <class T>
//ResourcePtr TGrowableBufferDX11<T>::GetBuffer()
//{
//	return( m_Buffer );
//}
//--------------------------------------------------------------------------------
