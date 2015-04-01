//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//BasicVertex11.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkMultiExecutor11.h"
#include "ArkRenderer11.h"
#include "ArkLog.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkMultiExecutor11::ArkMultiExecutor11( )
{
}
//--------------------------------------------------------------------------------
ArkMultiExecutor11::~ArkMultiExecutor11()
{
}
//--------------------------------------------------------------------------------
void ArkMultiExecutor11::Execute( PipelineManager* pPipeline, IParameterManager* pParamManager )
{
	// Propagate the execute call to all sub-executors.

	for ( auto pExecutor : m_Executors )
		pExecutor->Execute( pPipeline, pParamManager );
}
//--------------------------------------------------------------------------------
int ArkMultiExecutor11::GetInputLayout( int ShaderID )
{
	// Since this executor doesn't actually ever get used, this method should
	// never be called!  If it is called, it means that someone sub-classed from
	// this class and is using it improperly.

	ArkLog::Get(LogType::Renderer).Write( L"ERROR: Trying to get the input layout of a MultiExecutorDX11!!!" );

	return( -1 );
}
//--------------------------------------------------------------------------------
void ArkMultiExecutor11::GenerateInputLayout( int ShaderID )
{
	// Propagate the call to all sub-executors.

	for ( auto pExecutors : m_Executors )
		pExecutors->GenerateInputLayout( ShaderID );
}
//--------------------------------------------------------------------------------
void ArkMultiExecutor11::SetLayoutElements( unsigned int count, D3D11_INPUT_ELEMENT_DESC* pElements )
{
	// Since this executor doesn't actually ever get used, this method should
	// never be called!  If it is called, it means that someone sub-classed from
	// this class and is using it improperly.

	ArkLog::Get(LogType::Renderer).Write( L"ERROR: Trying to set the layout elements of a MultiExecutorDX11!!!" );
}
//--------------------------------------------------------------------------------
void ArkMultiExecutor11::AddExecutor( ExecutorPtr pExecutor )
{
	m_Executors.push_back( pExecutor );
}
//--------------------------------------------------------------------------------