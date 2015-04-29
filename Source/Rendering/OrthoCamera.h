//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Camera.h
//--------------------------------------------------------------------------------
#include "Camera.h"
//--------------------------------------------------------------------------------
#ifndef __OrthoCamera_h__
#define __OrthoCamera_h__
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class OrthoCamera : public Camera
	{
	public:
		OrthoCamera(float minX,float minY,float maxX,
			float maxY,float nearClip,float farClip);
		~OrthoCamera();

		void CreateProjection();

		float xMin;
		float xMax;
		float yMin;
		float yMax;
	};
};
//--------------------------------------------------------------------------------
#endif