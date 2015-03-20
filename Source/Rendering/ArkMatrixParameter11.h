//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkMatrixParameter11.h
//--------------------------------------------------------------------------------
#ifndef __ArkMatrixParameter11_h__
#define __ArkMatrixParameter11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkRenderParameter11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkMatrixParameter11 : public ArkRenderParameter11
	{
	public:
		ArkMatrixParameter11();
		ArkMatrixParameter11(ArkMatrixParameter11& copy);
		virtual ~ArkMatrixParameter11();

		virtual void SetParameterData(void* pData,unsigned int thread = 0);

		virtual const ArkParamType GetParameterType();

		DirectX::XMMATRIX GetMatrix();
		void SetMatrix(DirectX::XMMATRIX& v);

	public:
		//for XM classes containing SSE data
		void* operator new(size_t i);
		void operator delete(void* p);
	protected:
		DirectX::XMMATRIX m_Matrix;
	};
};
//--------------------------------------------------------------------------------
#endif