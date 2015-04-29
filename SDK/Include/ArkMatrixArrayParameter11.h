//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkMatrixArrayParameter11.h
//--------------------------------------------------------------------------------
#ifndef __ArkMatrixArrayParameter11_h__
#define __ArkMatrixArrayParameter11_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkRenderParameter11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkMatrixArrayParameter11 : public ArkRenderParameter11
	{
	public:
		ArkMatrixArrayParameter11(int count);
		ArkMatrixArrayParameter11(ArkMatrixArrayParameter11& copy);
		ArkMatrixArrayParameter11& operator=(ArkMatrixArrayParameter11& parameter);
		virtual ~ArkMatrixArrayParameter11();

		virtual void SetParameterData(void* pData,unsigned int thread = 0);

		virtual const ArkParamType GetParameterType();

		int GetMatrixCount();


		DirectX::XMMATRIX* GetMatrices(unsigned int threadID = 0);
		DirectX::XMFLOAT4X4* GetMatrices4x4(unsigned int threadID = 0);
	protected:
		int						m_iMatrixCount;
		Float4Align DirectX::XMMATRIX*		m_pMatrices[NUM_THREADS+1];
		DirectX::XMFLOAT4X4*					m_pTestMatrices[NUM_THREADS+1];
	};
};
//--------------------------------------------------------------------------------
#endif