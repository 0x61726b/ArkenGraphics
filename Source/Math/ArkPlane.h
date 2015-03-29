//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkPlane.h
//--------------------------------------------------------------------------------
#ifndef __ArkPlane_h__
#define __ArkPlane_h__
//--------------------------------------------------------------------------------
#include "ArkShape3D.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkPlane : public ArkShape3D
	{
	public:
		ArkPlane( );
		ArkPlane( float a, float b, float c, float d );
		virtual ~ArkPlane( );

		// Operations
		void Normalize();
		float DistanceToPoint( const DirectX::XMVECTOR& pt ) const;

		// Member Access
		float a() const;
		float& a();
		float b() const;
		float& b();
		float c() const;
		float& c();
		float d() const;
		float& d();

		virtual eEShape GetShapeType( ) const;

	protected:
		union
		{
			struct 
			{
				float A;
				float B;
				float C;
				float D;
			};
            float m_fComponents[4];
		};

	};
};
//--------------------------------------------------------------------------------
#endif // Plane3f_h
