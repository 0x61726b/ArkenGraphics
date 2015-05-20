//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkOctree.h
//--------------------------------------------------------------------------------
#ifndef __ArkOctree_h__
#define __ArkOctree_h__
//--------------------------------------------------------------------------------
#include "ArkBox.h"
#include "ArkActor.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkOctree
	{
	public:
		ArkOctree();
		~ArkOctree();
		ArkOctree(ArkBox* BB);
		ArkOctree(ArkBox* BB,std::vector<ArkActor*> ObjList);

		void Update();
		void Build();
		void Insert();

		void FindEnclosingBox();
		void FindEnclosingCube();

		ArkOctree* CreateNode(ArkBox* bb,std::vector<ArkActor*> list);
		const int MIN_SIZE = 1;
	protected:
		ArkBox*							m_Region;
		std::vector<ArkActor*>			m_vObjects;
		std::vector<ArkOctree*>			m_vChilds;
		std::vector<ArkActor*>			m_vPendingObjects;
		byte							m_bActiveNodes;
		int								m_iMaxLifeSpan;
		int								m_iCurLife;

		ArkOctree*						m_Parent;
		bool							m_bTreeReady;
		bool							m_bTreeBuilt;

	};
}
//--------------------------------------------------------------------------------
#endif

