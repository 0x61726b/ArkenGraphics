//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkConstantBuffer11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkConstantBuffer11.h"
#include "PipelineManager.h"
#include "IParameterManager.h"
#include "ArkLog.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
ArkConstantBuffer11::ArkConstantBuffer11( Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer )
{
	m_pBuffer = pBuffer;
	m_bAutoUpdate = true;
}
//--------------------------------------------------------------------------------
ArkConstantBuffer11::~ArkConstantBuffer11()
{
	// Buffer is released in the BufferDX11 destructor
}
//--------------------------------------------------------------------------------
ResourceType ArkConstantBuffer11::GetType()
{
	return( RT_CONSTANTBUFFER );
}
//--------------------------------------------------------------------------------
void ArkConstantBuffer11::AddMapping( ConstantBufferMapping& mapping )
{
	m_Mappings.push_back( mapping );
}
//--------------------------------------------------------------------------------
void ArkConstantBuffer11::EmptyMappings( )
{
	m_Mappings.clear();
}
//--------------------------------------------------------------------------------
void ArkConstantBuffer11::EvaluateMappings(PipelineManager* pPipeline,IParameterManager* pParamManager)
{
	// Test the index to ensure that it is a constant buffer.  If the method above returns
	// a non-null result, then this is a constant buffer.
	if ( m_pBuffer ) 
	{
		if ( GetAutoUpdate() )
		{
			// Check the parameters that go into this constant buffer, and see if they have
			// new values that need to be loaded into the buffer for this frame.  If not,
			// then we can safely skip the update of this buffer.

			bool doUpdate = false;

			for ( unsigned int j = 0; j < m_Mappings.size(); j++ ) {
				if ( m_Mappings[j].pParameter->GetValueID( pParamManager->GetID() ) 
					!= m_Mappings[j].valueID ) {
					doUpdate = true;
					break;
				}
			}

			if ( doUpdate ) {

				// Map the constant buffer into system memory.  We map the buffer 
				// with the discard write flag since we don't care what was in the
				// buffer already.

				D3D11_MAPPED_SUBRESOURCE resource = 
					pPipeline->MapResource(this, 0, D3D11_MAP_WRITE_DISCARD, 0 );

				// Update each variable in the constant buffer.  These variables are identified
				// by their type, and are currently allowed to be Vector4f, Matrix4f, or Matrix4f
				// arrays.  Additional types will be added as they are needed...

				for ( unsigned int j = 0; j < m_Mappings.size(); j++ )
				{
					std::shared_ptr<ArkRenderParameter11> pParam		= m_Mappings[j].pParameter;
					unsigned int offset									= m_Mappings[j].offset;
					unsigned int size									= m_Mappings[j].size;
					unsigned int elements								= m_Mappings[j].elements;
					unsigned int valueID								= m_Mappings[j].valueID;
					unsigned int threadID								= pParamManager->GetID();

					if( pParam == nullptr )
						continue;

					m_Mappings[j].valueID = pParam->GetValueID( threadID );

					if ( m_Mappings[j].varclass == D3D_SVC_VECTOR )
					{
						Float4Align DirectX::XMVECTOR vector = pParamManager->GetVectorParameter( pParam );
						DirectX::XMVECTOR* pBuf = (DirectX::XMVECTOR*)((char*)resource.pData + offset);
						*pBuf = vector;
						int x = 0;
					}
					else if ( ( m_Mappings[j].varclass == D3D_SVC_MATRIX_ROWS ) ||
						( m_Mappings[j].varclass == D3D_SVC_MATRIX_COLUMNS ) )
					{
						// Check if it is an array of matrices first...
						if ( elements == 0 ) 
						{
							Float4Align DirectX::XMMATRIX matrix = pParamManager->GetMatrixParameter( pParam );
							DirectX::XMMATRIX* pBuf = (DirectX::XMMATRIX*)((char*)resource.pData + offset);
							*pBuf = matrix;
						}
						else 
						{
							// If a matrix array, then use the corresponding parameter type.
							if ( size == elements * sizeof( DirectX::XMFLOAT4X4 ) )
							{
								
								DirectX::XMFLOAT4X4* pM = pParamManager->GetMatrixArrayParameter( pParam );
								memcpy( ((char*)resource.pData + offset), (char*)pM, size );
								
							} else {
								ArkLog::Get(LogType::Renderer).Output( L"Mismatch in matrix array count, update will not be performed!!!" );
							}
						}
					} else {
						ArkLog::Get(LogType::Renderer).Output( L"Non vector or matrix parameter specified in a constant buffer!  This will not be updated!" );
					}
				}

				pPipeline->UnMapResource(this, 0 );
			}
		}
	} else {
		ArkLog::Get(LogType::Renderer).Output( L"Trying to update a constant buffer that isn't a constant buffer!" );
	}
}
//--------------------------------------------------------------------------------
bool ArkConstantBuffer11::ContainsMapping( int ID, const ConstantBufferMapping& mapping )
{
	bool result = false;

	// First retrieve the internal mapping

	unsigned int index = static_cast<unsigned int>( ID );

	if ( index < m_Mappings.size() ) {
		ConstantBufferMapping internalMapping = m_Mappings[index];

		if ( internalMapping.pParameter == mapping.pParameter 
			&& internalMapping.offset == mapping.offset 
			&& internalMapping.size == mapping.size
			&& internalMapping.varclass == mapping.varclass
			&& internalMapping.elements == mapping.elements ) {
			result = true;
		}
	}

	return( result );
}
//--------------------------------------------------------------------------------
void ArkConstantBuffer11::SetAutoUpdate( bool enable )
{
	m_bAutoUpdate = enable;
}
//--------------------------------------------------------------------------------
bool ArkConstantBuffer11::GetAutoUpdate()
{
	return( m_bAutoUpdate );
}
//--------------------------------------------------------------------------------