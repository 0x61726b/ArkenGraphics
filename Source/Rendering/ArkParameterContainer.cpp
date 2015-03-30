//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkParameterContainer.h
//--------------------------------------------------------------------------------
#include "PCH.h"
#include "ArkParameterContainer.h"
#include "ArkLog.h"
#include "ArkRenderParameter11.h"
#include "IParameterManager.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkParameterContainer::ArkParameterContainer()
{
}
//--------------------------------------------------------------------------------
ArkParameterContainer::~ArkParameterContainer()
{
	
}
//--------------------------------------------------------------------------------
void ArkParameterContainer::AddRenderParameter( std::shared_ptr<ArkParameterWriter> pWriter )
{
	// Add render parameter will take the pointer passed in and add it to the
	// entity's internal list.  Therefore, this must not be from the stack!

	if ( pWriter )
	{
		// Search the list to see if this parameter is already there
		std::shared_ptr<ArkParameterWriter> pCurr = 0;

		for ( unsigned int i = 0; i < m_RenderParameters.size(); i++ )
		{
			if ( pWriter->GetRenderParameterRef()->GetName() == m_RenderParameters[i]->GetRenderParameterRef()->GetName() )
			{
				pCurr = m_RenderParameters[i];
				break;
			}
		}

		if ( !pCurr )
		{
			m_RenderParameters.push_back( pWriter );
		}
		else
		{
			ArkLog::Get(LogType::Renderer).Write( L"Tried to add a parameter to an entity that was already there!" );
		}
	}
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkParameterWriter> ArkParameterContainer::GetRenderParameter( const std::wstring& name )
{
	std::shared_ptr<ArkParameterWriter> pResult = nullptr;

	for ( auto pParameterWriter : m_RenderParameters )
	{
		std::shared_ptr<ArkRenderParameter11> pParameter = pParameterWriter->GetRenderParameterRef();
		if ( pParameter != nullptr ) {
			if ( name.compare( pParameter->GetName() ) == 0 ) {
				pResult = pParameterWriter;
				break;
			}
		}
	}

	return( pResult );
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkConstantBufferParameterWriter11> ArkParameterContainer::GetConstantBufferParameterWriter( const std::wstring& name )
{
	std::shared_ptr<ArkParameterWriter> pWriter = nullptr;
	std::shared_ptr<ArkConstantBufferParameterWriter11> pConstantBufferWriter = nullptr;

	// Check if the parameter already exists in this container.
	pWriter = GetRenderParameter( name );

	if ( nullptr != pWriter )
	{
		// The parameter is there, so now check its parameter type...
		std::shared_ptr<ArkRenderParameter11> pParameter = pWriter->GetRenderParameterRef();

		if ( nullptr != pParameter )
		{
			// If the type is correct, then set the value.
			if ( pParameter->GetParameterType() == CBUFFER ) {
				pConstantBufferWriter = std::dynamic_pointer_cast<ArkConstantBufferParameterWriter11>( pWriter );
			} else {
				ArkLog::Get(LogType::Renderer).Write( L"ERROR: Trying to access a constant buffer in a non-CB parameter writer!!!" );
			}
		} else {
			// Parameter was there, but didn't have a reference set so you can't tell what type it is.
			// This shouldn't happen, so ArkLog an error if it does...
			ArkLog::Get(LogType::Renderer).Write( L"ERROR: Trying to access a parameter writer without any reference set!!!" );
		}
	}

	return( pConstantBufferWriter );
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkMatrixArrayParameterWriter11> ArkParameterContainer::GetMatrixArrayParameterWriter( const std::wstring& name )
{
	std::shared_ptr<ArkParameterWriter> pWriter = nullptr;
	std::shared_ptr<ArkMatrixArrayParameterWriter11> pMatrixArrayWriter = nullptr;

	// Check if the parameter already exists in this container.
	pWriter = GetRenderParameter( name );

	if ( nullptr != pWriter )
	{
		// The parameter is there, so now check its parameter type...
		std::shared_ptr<ArkRenderParameter11> pParameter = pWriter->GetRenderParameterRef();

		if ( nullptr != pParameter )
		{
			// If the type is correct, then set the value.
			if ( pParameter->GetParameterType() == MATRIX_ARRAY ) {
				pMatrixArrayWriter = std::dynamic_pointer_cast<ArkMatrixArrayParameterWriter11>( pWriter );
			} else {
				ArkLog::Get(LogType::Renderer).Write( L"ERROR: Trying to access a matrix array in a non-matrix array parameter writer!!!" );
			}
		} else {
			// Parameter was there, but didn't have a reference set so you can't tell what type it is.
			// This shouldn't happen, so ArkArkLog an error if it does...
			ArkLog::Get(LogType::Renderer).Write( L"ERROR: Trying to access a parameter writer without any reference set!!!" );
		}
	}

	return( pMatrixArrayWriter );
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkMatrixParameterWriter11> ArkParameterContainer::GetMatrixParameterWriter( const std::wstring& name )
{
	std::shared_ptr<ArkParameterWriter> pWriter = nullptr;
	std::shared_ptr<ArkMatrixParameterWriter11> pMatrixWriter = nullptr;

	// Check if the parameter already exists in this container.
	pWriter = GetRenderParameter( name );

	if ( nullptr != pWriter )
	{
		// The parameter is there, so now check its parameter type...
		std::shared_ptr<ArkRenderParameter11> pParameter = pWriter->GetRenderParameterRef();

		if ( nullptr != pParameter )
		{
			// If the type is correct, then set the value.
			if ( pParameter->GetParameterType() == MATRIX ) {
				pMatrixWriter = std::dynamic_pointer_cast<ArkMatrixParameterWriter11>( pWriter );
			} else {
				ArkLog::Get(LogType::Renderer).Write( L"ERROR: Trying to access a matrix in a non-matrix parameter writer!!!" );
			}
		} else {
			// Parameter was there, but didn't have a reference set so you can't tell what type it is.
			// This shouldn't happen, so ArkArkLog an error if it does...
			ArkLog::Get(LogType::Renderer).Write( L"ERROR: Trying to access a parameter writer without any reference set!!!" );
		}
	}

	return( pMatrixWriter );
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkVectorParameterWriter11> ArkParameterContainer::GetVectorParameterWriter( const std::wstring& name )
{
	std::shared_ptr<ArkParameterWriter> pWriter = nullptr;
	std::shared_ptr<ArkVectorParameterWriter11> pVectorWriter = nullptr;

	// Check if the parameter already exists in this container.
	pWriter = GetRenderParameter( name );

	if ( nullptr != pWriter )
	{
		// The parameter is there, so now check its parameter type...
		std::shared_ptr<ArkRenderParameter11> pParameter = pWriter->GetRenderParameterRef();

		if ( nullptr != pParameter )
		{
			// If the type is correct, then set the value.
			if ( pParameter->GetParameterType() == VECTOR) {
				pVectorWriter = std::dynamic_pointer_cast<ArkVectorParameterWriter11>( pWriter );
			} else {
				ArkLog::Get(LogType::Renderer).Write( L"ERROR: Trying to access a vector in a non-vector parameter writer!!!" );
			}
		} else {
			// Parameter was there, but didn't have a reference set so you can't tell what type it is.
			// This shouldn't happen, so ArkArkLog an error if it does...
			ArkLog::Get(LogType::Renderer).Write( L"ERROR: Trying to access a parameter writer without any reference set!!!" );
		}
	}

	return( pVectorWriter );
}
//--------------------------------------------------------------------------------
void ArkParameterContainer::SetRenderParams( IParameterManager* pParamManager )
{
	// Scroll through each parameter and set it in the provided parameter manager.
	for ( auto pParamWriter : m_RenderParameters )
		pParamWriter->WriteParameter( pParamManager );
}
//--------------------------------------------------------------------------------
void ArkParameterContainer::InitRenderParams( )
{
	// Scroll through each parameter and initialize its value.
	for ( auto pParamWriter : m_RenderParameters )
		pParamWriter->InitializeParameter( );
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkConstantBufferParameterWriter11> ArkParameterContainer::SetConstantBufferParameter( const std::wstring& name, const ResourcePtr& value )
{
	// Check if the parameter already exists in this container
	std::shared_ptr<ArkConstantBufferParameterWriter11> pConstantBufferWriter = GetConstantBufferParameterWriter( name );

	// If not, then create one...
	if ( nullptr == pConstantBufferWriter ) {
		pConstantBufferWriter = std::make_shared<ArkConstantBufferParameterWriter11>();
		pConstantBufferWriter->SetRenderParameterRef( ArkRenderer11::Get()->m_pParamMgr->GetConstantBufferParameterRef( name ) );
		AddRenderParameter( pConstantBufferWriter );
	}

	// ... and set the value.
	pConstantBufferWriter->SetValue( value );

	return( pConstantBufferWriter );
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkMatrixArrayParameterWriter11> ArkParameterContainer::SetMatrixArrayParameter( const std::wstring& name, DirectX::XMMATRIX* values, int count )
{
	// Check if the parameter already exists in this container
	std::shared_ptr<ArkMatrixArrayParameterWriter11> pMatrixArrayWriter = GetMatrixArrayParameterWriter( name );

	// If not, then create one...
	if ( nullptr == pMatrixArrayWriter ) {
		pMatrixArrayWriter = std::make_shared<ArkMatrixArrayParameterWriter11>();
		pMatrixArrayWriter->SetRenderParameterRef( ArkRenderer11::Get()->m_pParamMgr->GetMatrixArrayParameterRef( name, count ) );
		AddRenderParameter( pMatrixArrayWriter );
	}

	// ... and set the value.
	pMatrixArrayWriter->SetValue( values );

	return( pMatrixArrayWriter );
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkMatrixParameterWriter11> ArkParameterContainer::SetMatrixParameter( const std::wstring& name, const DirectX::XMMATRIX& value )
{
	// Check if the parameter already exists in this container
	std::shared_ptr<ArkMatrixParameterWriter11> pMatrixWriter = GetMatrixParameterWriter( name );

	// If not, then create one...
	if ( nullptr == pMatrixWriter ) {
		pMatrixWriter = std::make_shared<ArkMatrixParameterWriter11>();
		pMatrixWriter->SetRenderParameterRef( ArkRenderer11::Get()->m_pParamMgr->GetMatrixParameterRef( name ) );
		AddRenderParameter( pMatrixWriter );
	}

	// ... and set the value.
	pMatrixWriter->SetValue( value );

	return( pMatrixWriter );
}
//--------------------------------------------------------------------------------
std::shared_ptr<ArkVectorParameterWriter11> ArkParameterContainer::SetVectorParameter( const std::wstring& name, const DirectX::XMVECTOR& value )
{
	// Check if the parameter already exists in this container
	std::shared_ptr<ArkVectorParameterWriter11> pMatrixWriter = GetVectorParameterWriter( name );

	// If not, then create one...
	if ( nullptr == pMatrixWriter ) {
		pMatrixWriter = std::make_shared<ArkVectorParameterWriter11>();
		pMatrixWriter->SetRenderParameterRef( ArkRenderer11::Get()->m_pParamMgr->GetVectorParameterRef( name ) );
		AddRenderParameter( pMatrixWriter );
	}

	// ... and set the value.
	pMatrixWriter->SetValue( value );

	return( pMatrixWriter );
}

