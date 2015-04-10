//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//PipelineExecutor11.h
//--------------------------------------------------------------------------------
template <class T>
TGrowableIndexBuffer11<T>::TGrowableIndexBuffer11() :
    m_IB( nullptr )
{
	// Initialize our buffer to a reasonable size
	SetMaxElementCount( 128 );
}
//--------------------------------------------------------------------------------
template <class T>
TGrowableIndexBuffer11<T>::~TGrowableIndexBuffer11()
{
}
//--------------------------------------------------------------------------------
template <class T>
void TGrowableIndexBuffer11<T>::UploadData( PipelineManager* pPipeline )
{
	if ( m_uiElementCount > 0 && m_bUploadNeeded == true ) {

		m_bUploadNeeded = false;

		// Map the index buffer for writing

		D3D11_MAPPED_SUBRESOURCE resource = 
			pPipeline->MapResource( m_IB, 0, D3D11_MAP_WRITE_DISCARD, 0 );

		// Only copy as much of the data as you actually have filled up
	
		memcpy( resource.pData, m_pDataArray, m_uiElementCount * sizeof( T ) );

		// Unmap the index buffer

		pPipeline->UnMapResource( m_IB, 0 );
	}
}
//--------------------------------------------------------------------------------
template <class T>
ResourcePtr TGrowableIndexBuffer11<T>::GetBuffer()
{
    return( m_IB );
}
//--------------------------------------------------------------------------------
template <class T>
void TGrowableIndexBuffer11<T>::CreateResource( unsigned int elements )
{
	// Create the new index buffer, with the dynamic flag set to true

	ArkBuffer11Config ibuffer;
	ibuffer.SetDefaultIndexBuffer( elements * sizeof( T ), true );
	m_IB = ArkRenderer11::Get()->CreateIndexBuffer( &ibuffer, nullptr );
}
//--------------------------------------------------------------------------------
template <class T>
void TGrowableIndexBuffer11<T>::DeleteResource( )
{
	// Delete the existing resource if it already existed

	if ( nullptr != m_IB ) {
		ArkRenderer11::Get()->DeleteResource( m_IB );
		m_IB = nullptr;
	}
}
//--------------------------------------------------------------------------------
