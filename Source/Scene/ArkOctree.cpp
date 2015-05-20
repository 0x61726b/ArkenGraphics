//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkOctree.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkOctree.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
using namespace DirectX;
//--------------------------------------------------------------------------------
ArkOctree::ArkOctree()
	:m_bActiveNodes(0),
	m_iMaxLifeSpan(8),
	m_iCurLife(-1),
	m_bTreeBuilt(false),
	m_bTreeReady(false)
{

}
//--------------------------------------------------------------------------------
ArkOctree::ArkOctree(ArkBox* Region)
	:m_bActiveNodes(0),
	m_iMaxLifeSpan(8),
	m_iCurLife(-1),
	m_bTreeBuilt(false),
	m_bTreeReady(false)
{
	m_Region = Region;
}
//--------------------------------------------------------------------------------
ArkOctree::ArkOctree(ArkBox* BB,std::vector<ArkActor*> ObjList)
	:m_bActiveNodes(0),
	m_iMaxLifeSpan(8),
	m_iCurLife(-1),
	m_bTreeBuilt(false),
	m_bTreeReady(false)
{
	m_Region = BB;
	m_vObjects = ObjList;
}
//--------------------------------------------------------------------------------
ArkOctree::~ArkOctree()
{
}
//--------------------------------------------------------------------------------
void ArkOctree::Update()
{
	if(!m_bTreeBuilt)
	{
		while(m_vPendingObjects.size() != 0)
		{
			m_vObjects.push_back(m_vPendingObjects[m_vPendingObjects.size()-1]);
			Build();
		}
	}
	else
	{
		while(m_vPendingObjects.size() != 0)
		{
			m_vObjects.push_back(m_vPendingObjects[m_vPendingObjects.size()-1]);
			Insert();
		}
	}
}
//--------------------------------------------------------------------------------
void ArkOctree::Build()
{
	if(m_vObjects.size() <= 0)
		return;
	XMVECTOR Dimensions = m_Region->Max() - m_Region->Min();
	XMFLOAT3 gMin,gMax,gDim;
	XMStoreFloat3(&gMin,m_Region->Min());
	XMStoreFloat3(&gMax,m_Region->Max());
	XMStoreFloat3(&gDim,Dimensions);

	if(XMVector3Equal(Dimensions,XMVectorZero()))
	{
		FindEnclosingCube();
		Dimensions = m_Region->Max() - m_Region->Min();
	}
	if(gDim.x <= MIN_SIZE && gDim.y <= MIN_SIZE && gDim.z <= MIN_SIZE)
	{
		return;
	}
	XMVECTOR half = Dimensions * 0.5f;
	XMVECTOR Center = m_Region->Min() + half;
	XMFLOAT3 center3;
	XMStoreFloat3( &center3,Center );

	std::vector<ArkBox*> Octant;
	Octant[0] = new ArkBox(m_Region->Min(),Center);
	Octant[1] = new ArkBox(XMVectorSet(center3.x,gMin.y,gMin.z,0.0f),XMVectorSet(gMax.x,center3.y,center3.z,0.0f));
	Octant[2] = new ArkBox(XMVectorSet(center3.x,gMin.y,center3.z,0.0f),XMVectorSet(gMax.x,center3.y,gMax.z,0.0f));
	Octant[3] = new ArkBox(XMVectorSet(gMin.x,gMin.y,center3.z,0.0f),XMVectorSet(center3.x,center3.y,gMax.z,0.0f));
	Octant[4] = new ArkBox(XMVectorSet(gMin.x,center3.y,gMin.z,0.0f),XMVectorSet(center3.x,gMax.y,center3.z,0.0f));
	Octant[5] = new ArkBox(XMVectorSet(center3.x,center3.y,gMin.z,0.0f),XMVectorSet(gMax.x,gMax.y,center3.z,0.0f));
	Octant[6] = new ArkBox(Center,m_Region->Max());
	Octant[7] = new ArkBox(XMVectorSet(gMin.x,center3.y,center3.z,0.0f),XMVectorSet(center3.x,gMax.y,gMax.z,0.0f));

	std::vector<std::vector<ArkActor*>> OctantList;
	OctantList.resize(8);

	std::vector<ArkActor*> deList;

	for( int i=0; i < m_vObjects.size();i++ )
	{
		ArkBox* objBb = m_vObjects[i]->BoundingBox;

		if( !XMVector3Equal( objBb->Max(),objBb->Min() ) )
		{
			for( int k=0; k < 8; k++ )
			{
				if( Octant[k] == objBb )
				{
					OctantList[k].push_back( m_vObjects[i] );
					deList.push_back(m_vObjects[i] );
					break;
				}
			}
		}
	}
	for( int i=0; i < deList.size(); i++ )
	{
		int toDelete = -1;
		for( int k=0; k < m_vObjects.size(); k++)
		{
			if( deList[i] == m_vObjects[k] )
			{
				toDelete = k;
			}
		}
		m_vObjects.erase(m_vObjects.begin() + toDelete);
	}

	for( int i=0; i < 8; i++ )
	{
		if( OctantList[i].size() != 0 )
		{
			m_vChilds[i] = (CreateNode(Octant[i],OctantList[i]));
			m_bActiveNodes |= (byte)(1 << i);
			m_vChilds[i]->Build();
		}
	}
	m_bTreeBuilt = true;
	m_bTreeReady = true;
}
//--------------------------------------------------------------------------------
ArkOctree* ArkOctree::CreateNode( ArkBox* bB , std::vector<ArkActor*> List )
{
	if( List.size() == 0 )
		return NULL;
	ArkOctree* Octree = new ArkOctree(bB,List);
}
void ArkOctree::Insert()
{
}
//--------------------------------------------------------------------------------
void ArkOctree::FindEnclosingBox()
{
	XMFLOAT3 gMin,gMax;
	XMStoreFloat3(&gMin,m_Region->Min());
	XMStoreFloat3(&gMax,m_Region->Max());

	for(int i=0; i < m_vObjects.size(); ++i)
	{
		XMFLOAT3 lMin,lMax;
		lMin = XMFLOAT3(0,0,0);
		lMax = XMFLOAT3(0,0,0);

		ArkActor* pActor = m_vObjects[i];
		ArkBox* pBb = pActor->BoundingBox;

		if(pActor->BoundingBox != NULL && !XMVector3Equal(pBb->Max(),pBb->Min()))
		{
			XMStoreFloat3(&lMin,pBb->Min());
			XMStoreFloat3(&lMax,pBb->Max());
		}
		if(lMin.x < gMin.x) gMin.x = lMin.x;
		if(lMin.y < gMin.y) gMin.y= lMin.y;
		if(lMin.z < gMin.z) gMin.z = lMin.z;

		if(lMax.x > gMax.x) gMax.x = lMax.x;
		if(lMax.y > gMax.y) gMax.y = lMax.y;
		if(lMax.z > gMax.z) gMax.z = lMax.z;
	}
	m_Region->Min() = XMLoadFloat3(&gMin);
	m_Region->Max() = XMLoadFloat3(&gMax);
}
//--------------------------------------------------------------------------------
void ArkOctree::FindEnclosingCube()
{
	FindEnclosingBox();

	XMVECTOR offset = m_Region->Min();
	m_Region->Min() += offset;
	m_Region->Max() += offset;

	XMFLOAT3 gMax,gMin;
	XMStoreFloat3(&gMin,m_Region->Min());
	XMStoreFloat3(&gMax,m_Region->Max());

	int highX = (int)floor(max(max(gMax.x,gMax.y),gMax.z));

	for(int bit = 0; bit < 32; bit++)
	{
		if(highX == 1 << bit)
		{
			m_Region->Max() = XMVectorSet(highX,highX,highX,0.0f);

			m_Region->Min() -= offset;
			m_Region->Max() -= offset;
			return;
		}
	}
	int a = highX;
	int count;
	std::frexp(a,&count);
	int x = count - 1;

	m_Region->Max() = XMVectorSet(x,x,x,0.0f);

	m_Region->Min() -= offset;
	m_Region->Max() -= offset;
}
//--------------------------------------------------------------------------------