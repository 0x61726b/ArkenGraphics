//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Actor.h
//--------------------------------------------------------------------------------
#ifndef __Actor_h__
#define __Actor_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkNode3D.h"
#include "ArkEntity3D.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class Actor
	{
	public:
		Actor();
		virtual ~Actor();

		ArkNode3D* GetNode() const;
		ArkNode3D* GetNode();
		ArkEntity3D* GetBody() const;
		ArkEntity3D* GetBody();


		void AddElement( ArkEntity3D* pElement );
		void RemoveElement( ArkEntity3D* pElement );
		void AddElement( ArkNode3D* pElement );
		void RemoveElement( ArkNode3D* pElement );

		//void AddSubtree( Node3D* pNode );
		//void RemoveSubtree( Node3D* pNode );



	protected:
		ArkNode3D*					m_pRoot;
		ArkEntity3D*				m_pBody;

		std::vector<ArkEntity3D*>	m_EntityElements;
		std::vector<ArkNode3D*>	m_NodeElements;

	};
};

//--------------------------------------------------------------------------------
#endif
