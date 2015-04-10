//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkDrawExecutor11.h
//--------------------------------------------------------------------------------
template <class T>
TGrowableVertexBuffer11<T>::TGrowableVertexBuffer11()
	: m_VB( nullptr )
{
	// Initialize our buffer to a reasonable size
	SetMaxElementCount( 128 );
}
//--------------------------------------------------------------------------------
template <class T>
TGrowableVertexBuffer11<T>::~TGrowableVertexBuffer11()
{
}
//--------------------------------------------------------------------------------
template <class T>
void TGrowableVertexBuffer11<T>::UploadData( PipelineManager* pPipeline )
{
	if ( m_uiElementCount > 0 && m_bUploadNeeded == true ) {

		m_bUploadNeeded = false;

		// Map the vertex buffer for writing

		D3D11_MAPPED_SUBRESOURCE resource = 
			pPipeline->MapResource( m_VB, 0, D3D11_MAP_WRITE_DISCARD, 0 );

		// Only copy as much of the data as you actually have filled up
	
		memcpy( resource.pData, m_pDataArray, m_uiElementCount * sizeof( T ) );

		// Unmap the vertex buffer

		pPipeline->UnMapResource( m_VB, 0 );
	}
}
//--------------------------------------------------------------------------------
template <class T>
ResourcePtr TGrowableVertexBuffer11<T>::GetBuffer()
{
    return( m_VB );
}
//--------------------------------------------------------------------------------
template <class T>
void TGrowableVertexBuffer11<T>::CreateResource( unsigned int elements )
{
	// Create the new vertex buffer, with the dynamic flag set to true

	ArkBuffer11Config vbuffer;
	vbuffer.SetDefaultVertexBuffer( elements * sizeof( T ), true );
	m_VB = ArkRenderer11::Get()->CreateVertexBuffer( &vbuffer, nullptr );
}
//--------------------------------------------------------------------------------
template <class T>
void TGrowableVertexBuffer11<T>::DeleteResource( )
{
	// Delete the existing resource if it already existed
	
	if ( nullptr != m_VB ) {
		ArkRenderer11::Get()->DeleteResource( m_VB );
		m_VB = nullptr;
	}
}
//--------------------------------------------------------------------------------
