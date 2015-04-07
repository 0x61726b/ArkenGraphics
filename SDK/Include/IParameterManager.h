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
#include "ArkShaderResourceParameter11.h"
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
		virtual void SetMatrixArrayParameter( const std::wstring& name,int count,DirectX::XMMATRIX* pmA ) = 0;
		virtual void SetConstantBufferParameter( const std::wstring& name, ResourcePtr resource ) = 0;
		virtual void SetShaderResourceParameter( const std::wstring& name, ResourcePtr resource ) = 0;

		virtual void SetVectorParameter( std::shared_ptr<ArkRenderParameter11> pP,DirectX::XMVECTOR* pV ) = 0;
		virtual void SetMatrixParameter( std::shared_ptr<ArkRenderParameter11> pP,DirectX::XMMATRIX* pM ) = 0;
		virtual void SetMatrixArrayParameter( std::shared_ptr<ArkRenderParameter11> pP,int count,DirectX::XMMATRIX* pV ) = 0;
		virtual void SetConstantBufferParameter( std::shared_ptr<ArkRenderParameter11> pParameter, ResourcePtr resource ) = 0;
		virtual void SetShaderResourceParameter( std::shared_ptr<ArkRenderParameter11> pParameter, ResourcePtr resource ) = 0;

		virtual std::shared_ptr<ArkRenderParameter11> GetParameterRef( const std::wstring& name ) = 0;
		virtual std::shared_ptr<ArkVectorParameter11> GetVectorParameterRef( const std::wstring& name ) = 0;
		virtual std::shared_ptr<ArkMatrixParameter11> GetMatrixParameterRef( const std::wstring& name ) = 0 ;
		virtual std::shared_ptr<ArkMatrixArrayParameter11> GetMatrixArrayParameterRef( const std::wstring& name,int count ) = 0;
		virtual std::shared_ptr<ArkConstantBufferParameter11> GetConstantBufferParameterRef( const std::wstring& name ) = 0;
		virtual std::shared_ptr<ArkShaderResourceParameter11> GetShaderResourceParameterRef( const std::wstring& name ) = 0;

		virtual DirectX::XMVECTOR GetVectorParameter( std::shared_ptr<ArkRenderParameter11> pP ) = 0;
		virtual DirectX::XMMATRIX GetMatrixParameter( std::shared_ptr<ArkRenderParameter11> pP ) = 0;
		virtual DirectX::XMMATRIX* GetMatrixArrayParameter( std::shared_ptr<ArkRenderParameter11> pP) = 0;
		virtual int GetConstantBufferParameter( std::shared_ptr<ArkRenderParameter11> pParameter ) = 0;
		virtual int GetShaderResourceParameter( std::shared_ptr<ArkRenderParameter11> pParameter ) = 0;

		virtual DirectX::XMVECTOR GetVectorParameter( const std::wstring& name ) = 0;
		virtual DirectX::XMMATRIX GetMatrixParameter( const std::wstring& name ) = 0;
		virtual DirectX::XMMATRIX GetMatrixArrayParameter( const std::wstring& name,int count ) = 0;
		virtual int GetConstantBufferParameter( const std::wstring& name ) = 0;
		virtual int GetShaderResourceParameter( const std::wstring& name ) = 0;

		virtual void SetWorldMatrix( DirectX::XMMATRIX* w ) = 0;
		virtual void SetViewMatrix( DirectX::XMMATRIX* v ) = 0;	
		virtual void SetProjectionMatrix( DirectX::XMMATRIX* p ) = 0;

		virtual void AttachParent( IParameterManager* pParent ) = 0;
		virtual void DetachParent( ) = 0;

	};
};
#endif
//--------------------------------------------------------------------------------