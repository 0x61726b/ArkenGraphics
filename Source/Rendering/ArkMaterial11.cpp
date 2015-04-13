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
#include "ArkLog.h"
#include "ArkMaterial11.h"
#include "IParameterManager.h"
#include "PipelineManager.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkMaterial11::ArkMaterial11()
{
	for ( int i = 0; i < VT_NUM_VIEW_TYPES; i++ )
	{
		Params[i].bRender = false;
		Params[i].pEffect = nullptr;
	}
	
	//m_pEntity = nullptr;
}
//--------------------------------------------------------------------------------
ArkMaterial11::~ArkMaterial11()
{

}
//--------------------------------------------------------------------------------
void ArkMaterial11::Update( float time )
{
	for ( int i = 0; i < VT_NUM_VIEW_TYPES; i++ )
	{
		if ( Params[i].bRender ) 
		{
			for ( auto pView : Params[i].Tasks )
				pView->Update( time );
		}
	}
}
//--------------------------------------------------------------------------------
void ArkMaterial11::PreRender( ArkRenderer11* pRenderer, VIEWTYPE type )
{
	// Update the render views that are needed before a view of 'type' can be
	// processed.  We will always try to update the view, but it may limit if it
	// is updated based on the maximum recurrence allowed. 

	for ( auto pView : Params[type].Tasks ) 
		pView->QueuePreTasks( pRenderer );
}
//--------------------------------------------------------------------------------
void ArkMaterial11::SetRenderParams( IParameterManager* pParamManager, VIEWTYPE type )
{
	// Set any special render view parameters that may be needed for the effect
	// to use the results of the render view in its rendering pass.

	for ( auto pView : Params[type].Tasks )
		pView->SetUsageParams( pParamManager);

	// Set the additional render parameters that have been added to the material.

	Parameters.SetRenderParams( pParamManager );
}
//--------------------------------------------------------------------------------
void ArkMaterial11::GetAllVertexShaderIDs( std::vector<int>& idlist )
{
	for ( unsigned int i = 0; i < VT_NUM_VIEW_TYPES; i++ )
	{
		if ( Params[i].pEffect != nullptr )
		{
			int ID = Params[i].pEffect->GetVertexShader();
		
			if ( ID != -1 ) 
				idlist.push_back( ID );
		}
	}
}
//--------------------------------------------------------------------------------