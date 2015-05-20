//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkBox.h
//--------------------------------------------------------------------------------
#ifndef __ArkBox_h__
#define __ArkBox_h__
//--------------------------------------------------------------------------------
#include "ArkShape3D.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkBox : public ArkShape3D
	{
	public:
		ArkBox();
		ArkBox(DirectX::XMVECTOR& c,DirectX::XMVECTOR& e,DirectX::XMVECTOR& m,DirectX::XMVECTOR& min);
		ArkBox(DirectX::XMVECTOR& Min,DirectX::XMVECTOR& Max);
		virtual ~ArkBox();


		DirectX::XMVECTOR Center() const;
		DirectX::XMVECTOR& Center();


		DirectX::XMVECTOR Extents() const;
		DirectX::XMVECTOR& Extents();

		DirectX::XMVECTOR Max() const;
		DirectX::XMVECTOR& Max();

		DirectX::XMVECTOR Min() const;
		DirectX::XMVECTOR& Min();

		virtual eEShape GetShapeType() const;

	public:
		DirectX::XMVECTOR m_vCenter;
		DirectX::XMVECTOR m_vExtents;
		DirectX::XMVECTOR m_vMax;
		DirectX::XMVECTOR m_vMin;

	};
};
//--------------------------------------------------------------------------------
#endif