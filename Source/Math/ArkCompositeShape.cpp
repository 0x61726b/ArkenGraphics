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
#include "ArkCompositeShape.h"
#include "ArkSphere3.h"
//#include "Box3f.h"
#include "ArkFrustum.h"
#include "ArkPlane.h"
#include "ArkRay3.h"
//#include "Triangle3f.h"
//#include "IntrRay3fSphere3f.h"
//#include "IntrRay3fBox3f.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkCompositeShape::ArkCompositeShape( )
{
}
//--------------------------------------------------------------------------------
ArkCompositeShape::~ArkCompositeShape()
{
	for ( auto pShape : m_Shapes )
		delete pShape;
}
//--------------------------------------------------------------------------------
void ArkCompositeShape::AddShape( ArkShape3D* pShape )
{
	if ( pShape )
		m_Shapes.push_back( pShape );
}
//--------------------------------------------------------------------------------
ArkCompositeShape* ArkCompositeShape::DeepCopy( )
{
	ArkCompositeShape* pCopy = new ArkCompositeShape();

	for ( auto pShape : m_Shapes )
	{
		// Get the shape type
		eEShape type = pShape->GetShapeType();

		// Create and copy the shape, then add it to the copied composite
		switch( type )
		{
		case SPHERE:
			{
				ArkSphere3* pSphere = new ArkSphere3();
				*pSphere = *(ArkSphere3*)pShape;
				pCopy->AddShape( pSphere );
				
				break;
			}

		case BOX:
			{
			/*	Box3f* pBox = new Box3f();
				*pBox = *(Box3f*)pShape;
				pCopy->AddShape( pBox );*/
				
				break;
			}

		case FRUSTUM:
			{
				ArkFrustum* pFrustum = new ArkFrustum();
				*pFrustum = *(ArkFrustum*)pShape;
				pCopy->AddShape( pFrustum );
				
				break;
			}

		case PLANE:
			{
				ArkPlane* pPlane = new ArkPlane();
				*pPlane = *(ArkPlane*)pShape;
				pCopy->AddShape( pPlane );
				
				break;
			}

		case RAY:
			{
				ArkRay3* pRay = new ArkRay3();
				*pRay = *(ArkRay3*)pShape;
				pCopy->AddShape( pRay );
				
				break;
			}

		case TRIANGLE:
			{
				/*Triangle3f* pTriangle = new Triangle3f();
				*pTriangle = *(Triangle3f*)pShape;
				pCopy->AddShape( pTriangle );
				*/
				break;
			}
		}
	}

	return( pCopy );
}
//--------------------------------------------------------------------------------
bool ArkCompositeShape::RayIntersection( const ArkRay3& ray, float* fDist )
{
	float fMin = 10000000000.0f;
	bool bHit = false;

	for ( auto pShape : m_Shapes )
	{
		eEShape type = pShape->GetShapeType();

		switch( type )
		{
		case SPHERE:
			{
				ArkSphere3* pSphere = (ArkSphere3*)pShape;
				/*IntrRay3fSphere3f Intr( ray, *pSphere );
				if ( Intr.Test() )
				{
					bHit = true;

					Intr.Find();
					for ( int j = 0; j < Intr.m_iQuantity; j++ )
					{
						if ( Intr.m_afRayT[j] < *fDist )
							*fDist = Intr.m_afRayT[j];
					}
				}*/

				break;
			}

		case BOX:
			{/*
				Box3f* pBox = (Box3f*)pShape;
				IntrRay3fBox3f Intr( ray, *pBox );
				if ( Intr.Test() )
				{
					bHit = true;

					Intr.Find();
					for ( int j = 0; j < Intr.m_iQuantity; j++ )
					{
						if ( Intr.m_afRayT[j] < *fDist )
							*fDist = Intr.m_afRayT[j];
					}
				}
				*/
				break;
			}

		case FRUSTUM:
			{
				
				break;
			}

		case PLANE:
			{
				
				break;
			}

		case RAY:
			{
				
				break;
			}

		case TRIANGLE:
			{
				
				break;
			}
		}
	}

	return( bHit );

}
//--------------------------------------------------------------------------------
int ArkCompositeShape::GetNumberOfShapes() const
{
	return( m_Shapes.size() );
}
//--------------------------------------------------------------------------------