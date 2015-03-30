//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkPickRecord.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkNode3D.h"
#include "ArkEntity3D.h"
#include "ArkSceneGraph.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkNode3D::ArkNode3D() :
	m_pParent( nullptr ),
	Controllers( this )
{
}
//--------------------------------------------------------------------------------
ArkNode3D::~ArkNode3D()
{
}
//--------------------------------------------------------------------------------
void ArkNode3D::PreRender( ArkRenderer11* pRenderer, VIEWTYPE view )
{
	for ( auto pChild : m_Leafs ) {
		if ( pChild ) pChild->PreRender( pRenderer, view );
	}

	for ( auto node : m_Nodes ) {
		if ( node ) node->PreRender( pRenderer, view );
	}
}
//--------------------------------------------------------------------------------
void ArkNode3D::Render( PipelineManager* pPipelineManager, IParameterManager* pParamManager, VIEWTYPE view )
{
	for ( auto pChild : m_Leafs ) {
		if ( pChild ) pChild->Render( pPipelineManager, pParamManager, view );
	}

	for ( auto node : m_Nodes ) {
		if ( node ) node->Render( pPipelineManager, pParamManager, view );
	}

}
//--------------------------------------------------------------------------------
void ArkNode3D::Update( float time )
{
	UpdateLocal( time );
	UpdateWorld( );

	for ( auto pChild : m_Leafs ) {
		if ( pChild ) pChild->Update( time );
	}

	for ( auto node : m_Nodes ) {
		if ( node ) node->Update( time );
	}
}
//--------------------------------------------------------------------------------
void ArkNode3D::UpdateLocal( float fTime )
{
	// Update the controllers that are attached to this entity.

	Controllers.Update( fTime );

	// Load the local space matrix with the rotation and translation components.

	Transform.UpdateLocal();
}
//--------------------------------------------------------------------------------
void ArkNode3D::UpdateWorld( )
{
	// If the entity has a parent, then update its world matrix accordingly.

	if ( m_pParent )
		Transform.UpdateWorld( m_pParent->Transform.WorldMatrix() );
	else
		Transform.UpdateWorld( );
}
//--------------------------------------------------------------------------------
void ArkNode3D::AttachChild( ArkEntity3D* Child )
{
	// Check for open spots in the vector
	for ( auto& pChild : m_Leafs )
	{
		if ( pChild == nullptr )
		{
			pChild = Child;
			Child->AttachParent( this );
			return;
		}
	}

	// If no open spots then add a new one
	m_Leafs.push_back( Child );
	Child->AttachParent( this );
}
//--------------------------------------------------------------------------------
void ArkNode3D::AttachChild( ArkNode3D* Child )
{
	// Check for open spots in the vector
	for ( auto& pChild : m_Nodes )
	{
		if ( pChild == nullptr )
		{
			pChild = Child;
			Child->AttachParent( this );
			return;
		}
	}

	// If no open spots then add a new one
	m_Nodes.push_back( Child );
	Child->AttachParent( this );
}
//--------------------------------------------------------------------------------
void ArkNode3D::DetachChild( ArkEntity3D* Child )
{
	for ( auto& pChild : m_Leafs )
	{
		if ( pChild == Child )
		{
			pChild->DetachParent();
			pChild = nullptr;
		}
	}
}
//--------------------------------------------------------------------------------
void ArkNode3D::DetachChild( ArkNode3D* Child )
{
	for ( auto& pChild : m_Nodes )
	{
		if ( pChild == Child )
		{
			pChild->DetachParent();
			pChild = nullptr;
		}
	}
}
//--------------------------------------------------------------------------------
void ArkNode3D::AttachParent( ArkNode3D* Parent )
{
	m_pParent = Parent;
}
//--------------------------------------------------------------------------------
void ArkNode3D::DetachParent( )
{
	m_pParent = nullptr;
}
//--------------------------------------------------------------------------------
ArkNode3D* ArkNode3D::GetParent()
{
	return( m_pParent );
}
//--------------------------------------------------------------------------------
std::wstring ArkNode3D::toString( )
{
	std::wstringstream objString;

	for ( int i = 0; i < GraphDepth( this ); i++ )
		objString << L"  ";

	objString << L"Node3D ID: " << this << L", Name:" << GetName() << L"\n";

	for ( auto pChild : m_Leafs )
	{
		if ( pChild )
			objString << pChild->toString();
	}

	return( objString.str() );
	return L"";
}
//--------------------------------------------------------------------------------
void ArkNode3D::SetName( const std::wstring& name )
{
	m_Name = name;
}
//--------------------------------------------------------------------------------
std::wstring ArkNode3D::GetName() const
{
	return( m_Name );
}
//--------------------------------------------------------------------------------
//Entity3D* Node3D::GetChild( unsigned int index )
//{
//	if ( index >= m_Leafs.size() )
//		return( nullptr );
//	else
//		return( m_Leafs[index] );
//}
//--------------------------------------------------------------------------------
const std::vector<ArkEntity3D*>& ArkNode3D::Leafs()
{
	return m_Leafs;
}
//--------------------------------------------------------------------------------
const std::vector<ArkNode3D*>& ArkNode3D::Nodes()
{
	return m_Nodes;
}
//--------------------------------------------------------------------------------
