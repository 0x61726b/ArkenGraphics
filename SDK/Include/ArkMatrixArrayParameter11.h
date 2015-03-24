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
		ArkMatrixArrayParameter11();
		ArkMatrixArrayParameter11(ArkMatrixArrayParameter11& copy);
		virtual ~ArkMatrixArrayParameter11();

		virtual void SetParameterData(void* pData,unsigned int thread = 0);

		virtual const ArkParamType GetParameterType();
		
		DirectX::XMFLOAT4X4 GetMatrixArray();
		void SetMatrixArray( DirectX::XMFLOAT4X4& v );

	protected:
		DirectX::XMFLOAT4X4 m_Matrix;
	};
};
//--------------------------------------------------------------------------------
#endif