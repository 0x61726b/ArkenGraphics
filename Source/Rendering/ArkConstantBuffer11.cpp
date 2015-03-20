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