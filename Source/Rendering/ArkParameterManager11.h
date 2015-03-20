//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkParameterManager11.h
//--------------------------------------------------------------------------------
#ifndef __ArkParameterManager11_h__
#define __ArkParameterManager11_h__
//--------------------------------------------------------------------------------
#include "IParameterManager.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkParameterManager11 : public IParameterManager
	{
	public:
		ArkParameterManager11(unsigned int ID);
		virtual ~ArkParameterManager11();

		void SetVectorParameter(const std::wstring& name,DirectX::XMVECTOR* pV);
		void SetMatrixParameter(const std::wstring& name,DirectX::XMMATRIX* pM);
		void SetMatrixArrayParameter(const std::wstring& name,DirectX::XMFLOAT4X4* pmA);
		void SetConstantBufferParameter( const std::wstring& name, ResourcePtr resource );


		virtual void SetVectorParameter(ArkRenderParameter11* pP,DirectX::XMVECTOR* pV);
		virtual void SetMatrixParameter(ArkRenderParameter11* pP,DirectX::XMMATRIX* pM) ;
		virtual void SetMatrixArrayParameter(ArkRenderParameter11* pP,DirectX::XMFLOAT4X4* pV);
		virtual void SetConstantBufferParameter( ArkRenderParameter11* pParameter, ResourcePtr resource );

		virtual DirectX::XMVECTOR GetVectorParameter(const std::wstring& name);
		virtual DirectX::XMMATRIX GetMatrixParameter(const std::wstring& name);
		virtual DirectX::XMFLOAT4X4 GetMatrixArrayParameter(const std::wstring& name);
		virtual ArkConstantBufferParameter11* GetConstantBufferParameterRef( const std::wstring& name );

		virtual DirectX::XMVECTOR GetVectorParameter(ArkRenderParameter11* pP);
		virtual DirectX::XMMATRIX GetMatrixParameter(ArkRenderParameter11* pP);
		virtual DirectX::XMFLOAT4X4 GetMatrixArrayParameter(ArkRenderParameter11* pP) ;
		virtual int GetConstantBufferParameter( ArkRenderParameter11* pParameter ) ;


		virtual ArkRenderParameter11* GetParameterRef(const std::wstring& name) ;
		virtual ArkVectorParameter11* GetVectorParameterRef(const std::wstring& name) ;
		virtual ArkMatrixParameter11* GetMatrixParameterRef(const std::wstring& name)  ;
		virtual ArkMatrixArrayParameter11* GetMatrixArrayParameterRef(const std::wstring& name) ;
		virtual int GetConstantBufferParameter( const std::wstring& name );

		virtual void SetWorldMatrix(DirectX::XMMATRIX* w);
		virtual void SetViewMatrix(DirectX::XMMATRIX* v);
		virtual void SetProjectionMatrix(DirectX::XMMATRIX* p);

		static std::map<std::wstring,ArkRenderParameter11*> m_Parameters;

		unsigned int GetID();
	protected:
		ArkMatrixParameter11*	m_pWorldMatrix;
		ArkMatrixParameter11*	m_pViewMatrix;
		ArkMatrixParameter11*	m_pProjMatrix;
		ArkMatrixParameter11*	m_pWorldViewMatrix;
		ArkMatrixParameter11*	m_pWorldProjMatrix;
		ArkMatrixParameter11*	m_pViewProjMatrix;
		ArkMatrixParameter11*	m_pWorldViewProjMatrix;


		unsigned int m_ID;

	};
};
//--------------------------------------------------------------------------------
#endif