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
#include "Pch.h"
#include "PipelineExecutor11.h"
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
PipelineExecutor11::PipelineExecutor11( )
{
}
//--------------------------------------------------------------------------------
PipelineExecutor11::~PipelineExecutor11()
{
	std::map<int, InputLayoutKey*>::iterator it = m_InputLayouts.begin();
	for( ; it != m_InputLayouts.end(); it++ )
        Safe_Delete( (*it).second );
}
//--------------------------------------------------------------------------------
int PipelineExecutor11::GetInputLayout( int ShaderID )
{
	int layout = -1;

	// Automatically generate the layout if it doesn't already exist.

	if ( m_InputLayouts[ShaderID] == 0 )
		GenerateInputLayout( ShaderID );
	
	// The layout should be created now...
	layout = m_InputLayouts[ShaderID]->layout;

	return( layout );
}
//--------------------------------------------------------------------------------
void PipelineExecutor11::GenerateInputLayout( int ShaderID )
{
	// Create the input layout for the given shader index

	ArkRenderer11* pRenderer = ArkRenderer11::Get();
	if ( m_InputLayouts[ShaderID] == 0 )
	{
		InputLayoutKey* pKey = new InputLayoutKey();
		pKey->shader = ShaderID;
		pKey->layout = pRenderer->CreateInputLayout( m_elements, ShaderID );
		m_InputLayouts[ShaderID] = pKey;
	}
}
//--------------------------------------------------------------------------------
void PipelineExecutor11::SetLayoutElements( unsigned int count, D3D11_INPUT_ELEMENT_DESC* pElements )
{
	for ( unsigned int i = 0; i < count; i++ )
	{
		m_elements.push_back( pElements[i] );
	}
}
//--------------------------------------------------------------------------------
