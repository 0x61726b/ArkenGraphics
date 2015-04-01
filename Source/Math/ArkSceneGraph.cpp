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
#include "ArkSceneGraph.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
void Arkeng::GetAllEntities( ArkNode3D* node, std::vector< ArkEntity3D* >& set ) {

	set.reserve( set.size() + node->Leafs().size() );
	set.insert( set.end(), node->Leafs().begin(), node->Leafs().end() );

	// Get all of the leafs from this node, then decend to its children
	for ( auto n : node->Nodes() ) {
		GetAllEntities( n, set );
	}
}
//--------------------------------------------------------------------------------
void Arkeng::GetIntersectingEntities( ArkNode3D* node, std::vector< ArkEntity3D* >& set, ArkFrustum& bounds )
{
	// TODO: THis has to be fixed!
	//if ( bounds.Intersects( GetWorldBoundingSphere() ) )
	//	set.push_back( this );
}
//--------------------------------------------------------------------------------
void Arkeng::GetIntersectingEntities( ArkNode3D* node, std::vector< ArkEntity3D* >& set, ArkSphere3& bounds )
{
	// TODO: THis has to be fixed!
	//if ( bounds.Intersects( GetWorldBoundingSphere() ) )
	//	set.push_back( this );
}
//--------------------------------------------------------------------------------
void Arkeng::BuildPickRecord( ArkNode3D* node, const ArkRay3& ray, std::vector<ArkPickRecord>& record )
{
	// Get a flat list of all the entities in this node's subtree.
	std::vector<ArkEntity3D*> set;
	GetAllEntities( node, set );

	for ( auto& entity : set )
	{
		if ( entity->Shape.GetNumberOfShapes() > 0 )
		{
			XMVECTOR det;
			XMMATRIX InvWorld = XMMatrixInverse(&det,( entity->Transform.WorldMatrix() ));

			XMFLOAT3 or;
			XMStoreFloat3( &or,ray.Origin );

			XMFLOAT3 dir;
			XMStoreFloat3( &dir,ray.Direction);

			XMVECTOR position = XMVectorSet( or.x, or.y, or.z, 1.0f );
			XMVECTOR direction = XMVectorSet( dir.x, dir.y, dir.z, 0.0f );
		
			position = XMVector4Transform(position,InvWorld);
			direction = XMVector4Transform(direction,InvWorld );

			XMStoreFloat3(  &or,position );
			XMStoreFloat3( &dir,direction );

			XMFLOAT3 oOrigin;
			XMFLOAT3 oDir;

			ArkRay3 ObjectRay;
			oOrigin.x = or.x;
			oOrigin.y = or.y;
			oOrigin.z = or.z;
			oDir.x = dir.x;
			oDir.y = dir.y;
			oDir.z = dir.z;

			ObjectRay.Origin = XMLoadFloat3(&oOrigin);
			ObjectRay.Direction = XMLoadFloat3(&oDir);

			float fT = 10000000000.0f;
			if ( entity->Shape.RayIntersection( ObjectRay, &fT ) )
			{
				ArkPickRecord Record;
				Record.pEntity = entity;
				Record.fDistance = fT;
				record.push_back( Record );
			}
		}
		//else
		//{
		//	IntrRay3fSphere3f Intersector( ray, GetWorldBoundingSphere() );
		//	if ( Intersector.Find() )
		//	{
		//		PickRecord Record;

		//		Record.pEntity = this;
		//		Record.fDistance = min( Intersector.m_afRayT[0], Intersector.m_afRayT[1] ); 
		//		
		//		record.push_back( Record );
		//	}
		//}
	}
}
//--------------------------------------------------------------------------------
//void Node3D::GetIntersectingEntities( std::vector< Entity3D* >& set, Frustum3f& bounds )
//{
//	//Entity3D::GetIntersectingEntities( set, bounds );
//
//	for ( auto pChild : m_Leafs ) {
//		if ( pChild ) pChild->GetIntersectingEntities( set, bounds );
//	}
//
//	for ( auto node : m_Nodes ) {
//		if ( node ) node->GetIntersectingEntities( set, bounds );
//	}
//}
////--------------------------------------------------------------------------------
//void Node3D::GetIntersectingEntities( std::vector< Entity3D* >& set, Sphere3f& bounds )
//{
//	for ( auto pChild : m_Leafs ) {
//		if ( pChild ) pChild->GetIntersectingEntities( set, bounds );
//	}
//
//	for ( auto node : m_Nodes ) {
//		if ( node ) node->GetIntersectingEntities( set, bounds );
//	}
//}
//--------------------------------------------------------------------------------
//void Node3D::BuildPickRecord( Ray3f& ray, std::vector<PickRecord>& record )
//{
//	for ( auto pChild : m_Leafs )
//	{
//		if ( pChild )
//			pChild->BuildPickRecord( ray, record );
//	}
//}
