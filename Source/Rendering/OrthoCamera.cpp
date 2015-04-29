//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Camera.h
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "OrthoCamera.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
OrthoCamera::OrthoCamera(float minX,float minY,float maxX,
	float maxY,float nearClip,float farClip):
	xMin(minX),
	yMin(minY),
	xMax(maxX),
	yMax(maxY)
{
	m_fNear = nearClip;
	m_fFar = farClip;

	_ASSERT(xMax > xMin && yMax > yMin);

	CreateProjection();

}
//--------------------------------------------------------------------------------
OrthoCamera::~OrthoCamera()
{
}
//--------------------------------------------------------------------------------
void OrthoCamera::CreateProjection()
{
	m_ProjMatrix = XMMatrixOrthographicOffCenterLH(xMin, xMax, yMin, yMax, m_fNear, m_fFar);

}
//--------------------------------------------------------------------------------