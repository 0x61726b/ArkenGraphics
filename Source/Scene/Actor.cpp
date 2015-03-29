//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Actor.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Actor.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
Actor::Actor()
{
	m_pRoot = new ArkNode3D();
	m_pBody = new ArkEntity3D();
	m_pRoot->AttachChild(m_pBody);

	// Add the root and body to the element list for cleanup later on.
	AddElement(m_pRoot);
	AddElement(m_pBody);
}
//--------------------------------------------------------------------------------
Actor::~Actor()
{
	for(auto pEntity : m_EntityElements)
		Safe_Delete(pEntity);

	for(auto pNode : m_NodeElements)
		Safe_Delete(pNode);
}
//--------------------------------------------------------------------------------
ArkNode3D* Actor::GetNode() const
{
	return( m_pRoot );
}
//--------------------------------------------------------------------------------
ArkEntity3D* Actor::GetBody() const
{
	return( m_pBody );
}
//--------------------------------------------------------------------------------
ArkNode3D* Actor::GetNode()
{
	return( m_pRoot );
}
//--------------------------------------------------------------------------------
ArkEntity3D* Actor::GetBody()
{
	return( m_pBody );
}
//--------------------------------------------------------------------------------
void Actor::AddElement( ArkEntity3D* pElement )
{
	// Add the entity to the element list, so that it's lifetime will be 
	// controlled by the actor.  This does not do anything with the connection
	// to the scene graph, so beware!

	m_EntityElements.push_back( pElement );
}
//--------------------------------------------------------------------------------
void Actor::RemoveElement( ArkEntity3D* pElement )
{
	// Here we iterate through the list of actors and remove any instances of 
	// this actor. 

	auto it = m_EntityElements.begin();

	while ( it != m_EntityElements.end() ) {
		if ( *it == pElement ) 
			it = m_EntityElements.erase( it );
		else
			it++;
	}

	// Removing an element from this list does not automatically remove any sub-
	// tree objects.  For example, if a node is directly removed using this 
	// method, its children are not automatically removed.  If this is what you 
	// want, then you should acquire a list of the children, and iterate over it
	// to remove each entity.  This functionality is provided in the 'sub-tree'
	// methods for convenience.
}
//--------------------------------------------------------------------------------
void Actor::AddElement( ArkNode3D* pElement )
{
	// Add the entity to the element list, so that it's lifetime will be 
	// controlled by the actor.  This does not do anything with the connection
	// to the scene graph, so beware!

	m_NodeElements.push_back( pElement );
}
//--------------------------------------------------------------------------------
void Actor::RemoveElement( ArkNode3D* pElement )
{
	// Here we iterate through the list of actors and remove any instances of 
	// this actor. 

	auto it = m_NodeElements.begin();

	while ( it != m_NodeElements.end() ) {
		if ( *it == pElement ) 
			it = m_NodeElements.erase( it );
		else
			it++;
	}

	// Removing an element from this list does not automatically remove any sub-
	// tree objects.  For example, if a node is directly removed using this 
	// method, its children are not automatically removed.  If this is what you 
	// want, then you should acquire a list of the children, and iterate over it
	// to remove each entity.  This functionality is provided in the 'sub-tree'
	// methods for convenience.
}
//--------------------------------------------------------------------------------
//void Actor::AddSubtree( Node3D* pNode )
//{
//	// Acquire a list of all the entities within this subtree.
//
//	std::vector<Entity3D*> entities;
//	pNode->GetEntities( entities );
//
//	// Add each of these entities to the 'elements' list to manage their lifetimes.
//
//	for ( auto pEntity : entities ) {
//		AddElement( pEntity );
//	}
//
//	// NOTE: Any changes to the sub-tree will not be reflected in the element list
//	//       after this call!  If you change a subtree, it is up to you to put
//	//       the resulting changes in the element list!
//}
////--------------------------------------------------------------------------------
//void Actor::RemoveSubtree( Node3D* pNode )
//{
//	// Acquire a list of all the entities within this subtree.
//
//	std::vector<Entity3D*> entities;
//	pNode->GetEntities( entities );
//
//	// Remove each of these entities from the 'elements' list to no longer manage 
//	// their lifetimes.
//
//	for ( auto pEntity : entities ) {
//		RemoveElement( pEntity );
//	}
//
//	// NOTE: Any changes to the sub-tree will not be reflected in the element list
//	//       after this call!  If you change a subtree, it is up to you to put
//	//       the resulting changes in the element list!
//}
//--------------------------------------------------------------------------------
