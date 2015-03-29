//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------
#ifndef __ArkCompositeShape_h__
#define __ArkCompositeShape_h__
//--------------------------------------------------------------------------------
#include "ArkShape3D.h"
#include "ArkRay3.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkCompositeShape
	{
	public:
		ArkCompositeShape( );
		virtual ~ArkCompositeShape( );

		ArkCompositeShape* DeepCopy( );

		void AddShape( ArkShape3D* pShape );
		bool RayIntersection( const ArkRay3& ray, float* fDist );

		int GetNumberOfShapes() const;

		std::vector< ArkShape3D* > m_Shapes;
	};
};
//--------------------------------------------------------------------------------
#endif // CompositeShape_h
