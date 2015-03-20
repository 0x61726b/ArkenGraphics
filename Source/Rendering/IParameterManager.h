//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//IParameterManager.h
//--------------------------------------------------------------------------------
#ifndef __IParameterManager_h__
#define __IParameterManager_h__
//--------------------------------------------------------------------------------
#include "ArkRenderer11.h"
#include "ArkVectorParameter11.h"
#include "ArkMatrixParameter11.h"
#include "ArkMatrixArrayParameter11.h"
#include "ArkConstantBufferParameter11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class IParameterManager
	{
	public:
		IParameterManager() { };
		virtual ~IParameterManager() { };

		virtual unsigned int GetID() = 0;

		virtual void SetVectorParameter( const std::wstring& name,DirectX::XMVECTOR* pV ) = 0;
		virtual void SetMatrixParameter( const std::wstring& name,DirectX::XMMATRIX* pM ) = 0;
		virtual void SetMatrixArrayParameter( const std::wstring& name,DirectX::XMFLOAT4X4* pmA ) = 0;
		virtual void SetConstantBufferParameter( const std::wstring& name, ResourcePtr resource ) = 0;

		virtual void SetVectorParameter( ArkRenderParameter11* pP,DirectX::XMVECTOR* pV ) = 0;
		virtual void SetMatrixParameter( ArkRenderParameter11* pP,DirectX::XMMATRIX* pM ) = 0;
		virtual void SetMatrixArrayParameter( ArkRenderParameter11* pP,DirectX::XMFLOAT4X4* pV ) = 0;
		virtual void SetConstantBufferParameter( ArkRenderParameter11* pParameter, ResourcePtr resource ) = 0;

		virtual ArkRenderParameter11* GetParameterRef( const std::wstring& name ) = 0;
		virtual ArkVectorParameter11* GetVectorParameterRef( const std::wstring& name ) = 0;
		virtual ArkMatrixParameter11* GetMatrixParameterRef( const std::wstring& name ) = 0 ;
		virtual ArkMatrixArrayParameter11* GetMatrixArrayParameterRef( const std::wstring& name ) = 0;
		virtual ArkConstantBufferParameter11* GetConstantBufferParameterRef( const std::wstring& name ) = 0;

		virtual DirectX::XMVECTOR GetVectorParameter( ArkRenderParameter11* pP ) = 0;
		virtual DirectX::XMMATRIX GetMatrixParameter( ArkRenderParameter11* pP ) = 0;
		virtual DirectX::XMFLOAT4X4 GetMatrixArrayParameter( ArkRenderParameter11* pP ) = 0;
		virtual int GetConstantBufferParameter( ArkRenderParameter11* pParameter ) = 0;

		virtual DirectX::XMVECTOR GetVectorParameter( const std::wstring& name ) = 0;
		virtual DirectX::XMMATRIX GetMatrixParameter( const std::wstring& name ) = 0;
		virtual DirectX::XMFLOAT4X4 GetMatrixArrayParameter( const std::wstring& name ) = 0;
		virtual int GetConstantBufferParameter( const std::wstring& name ) = 0;


		virtual void SetWorldMatrix( DirectX::XMMATRIX* w ) = 0;
		virtual void SetViewMatrix( DirectX::XMMATRIX* v ) = 0;	
		virtual void SetProjectionMatrix( DirectX::XMMATRIX* p ) = 0;

	};
};
#endif
//--------------------------------------------------------------------------------