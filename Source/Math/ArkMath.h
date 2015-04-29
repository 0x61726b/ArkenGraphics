//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkMath.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkSphere3.h"
#include "ArkPlane.h"
#include "ArkFrustum.h"
using namespace DirectX;
//--------------------------------------------------------------------------------
#ifndef __ArkMath_h__
#define __ArkMath_h__
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkMath
	{
	public:
		static ArkSphere3 ComputeBoundingSphereFromPoints(const XMFLOAT3* points, UINT numPoints, UINT stride);
		static void ComputeFrustum( const XMMATRIX& viewProj, ArkFrustum& frustum );
		static UINT TestFrustumSphere( const ArkFrustum& frustum, const ArkSphere3& sphere );

	};

};
//--------------------------------------------------------------------------------
#endif