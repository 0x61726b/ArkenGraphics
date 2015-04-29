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
#include "PCH.h"
#include "ArkRenderable.h"
#include "ArkLog.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkRenderable::ArkRenderable() :
	iPass( Geometry ),
	Executor( nullptr ),
	Material( nullptr ) 
{
}
//--------------------------------------------------------------------------------
ArkRenderable::~ArkRenderable()
{
	Executor = nullptr;
	Material = nullptr;
}
//--------------------------------------------------------------------------------
void ArkRenderable::SetMaterial( MaterialPtr pMaterial )
{
	Material = pMaterial;

	// Query all of the vertex shader IDs that will be used so that the pipeline
	// executor class can pre-create them.

	if ( Material != nullptr && Executor != nullptr )
	{
		std::vector<int> idlist;
		Material->GetAllVertexShaderIDs( idlist );

		for ( auto ID : idlist ) {
			Executor->GenerateInputLayout( ID );
		}
	}
}
//--------------------------------------------------------------------------------
MaterialPtr ArkRenderable::GetMaterial( )
{
	return( Material );
}
//--------------------------------------------------------------------------------
void ArkRenderable::SetGeometry( ExecutorPtr pExecutor )
{
	Executor = pExecutor;

	// Query all of the vertex shader IDs that will be used so that the pipeline
	// executor class can pre-create them.

	if ( Material != nullptr && Executor != nullptr )
	{
		std::vector<int> idlist;
		Material->GetAllVertexShaderIDs( idlist );

		for ( auto ID : idlist ) {
			Executor->GenerateInputLayout( ID );
		}
	}
}
//--------------------------------------------------------------------------------
ExecutorPtr ArkRenderable::GetGeometry( )
{
	return( Executor );
}
//--------------------------------------------------------------------------------
