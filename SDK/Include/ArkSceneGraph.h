//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkSceneGraph.h
//--------------------------------------------------------------------------------
#ifndef __ArkSceneGraph_h__
#define __ArkSceneGraph_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkEntity3D.h"
#include "ArkNode3D.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	// Returns the next node up in the scene graph.
	template <typename T>
	ArkNode3D* GetParent( T* element ) {
		
		assert( element != nullptr );
		
		return element->GetParent();	
	};

	// Returns the last node in the graph which does not have a parent.
	template <typename T>
	ArkNode3D* GetRoot( T* element ) {

		assert( element != nullptr );

		Node3D* parent = GetParent( element );

		while ( parent != nullptr ) {
			element = parent;
			parent = GetParent( element );
		}

		return element;
	}

	template <typename T>
	int GraphDepth( T* element ) {

		assert( element != nullptr );

		ArkNode3D* parent = GetParent( element );

		if ( parent == nullptr )
			return( 0 );
		else
			return( GraphDepth( parent ) + 1 );
	}


	void GetAllEntities( ArkNode3D* node, std::vector< ArkEntity3D* >& set );

	// The pick record is the correct way to build a list of the entities that are 
	// intersecting the ray.  The other two methods are just as valid, but perform
	// a different type of query than the pick record.
	
	void BuildPickRecord( ArkNode3D* node, const ArkRay3& ray, std::vector<ArkPickRecord>& record );
	void GetIntersectingEntities( ArkNode3D* node, std::vector< ArkEntity3D* >& set, ArkSphere3& bounds );
	void GetIntersectingEntities( ArkNode3D* node, std::vector< ArkEntity3D* >& set, ArkFrustum& bounds );
};
//--------------------------------------------------------------------------------
#endif // SceneGraph_h
//--------------------------------------------------------------------------------
