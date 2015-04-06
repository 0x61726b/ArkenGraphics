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
		void SetMatrixArrayParameter(const std::wstring& name,int count,DirectX::XMMATRIX* pmA);
		void SetConstantBufferParameter( const std::wstring& name,ResourcePtr resource );


		virtual void SetVectorParameter( std::shared_ptr<ArkRenderParameter11> pP,DirectX::XMVECTOR* pV);
		virtual void SetMatrixParameter(std::shared_ptr<ArkRenderParameter11> pP,DirectX::XMMATRIX* pM) ;
		virtual void SetMatrixArrayParameter(std::shared_ptr<ArkRenderParameter11> pP,int count,DirectX::XMMATRIX* pV);
		virtual void SetConstantBufferParameter( std::shared_ptr<ArkRenderParameter11> pParameter, ResourcePtr resource );

		virtual DirectX::XMVECTOR GetVectorParameter(const std::wstring& name);
		virtual DirectX::XMMATRIX GetMatrixParameter(const std::wstring& name);
		virtual DirectX::XMMATRIX GetMatrixArrayParameter(const std::wstring& name,int count);
		virtual std::shared_ptr<ArkConstantBufferParameter11> GetConstantBufferParameterRef( const std::wstring& name );

		virtual DirectX::XMVECTOR GetVectorParameter(std::shared_ptr<ArkRenderParameter11> pP);
		virtual DirectX::XMMATRIX GetMatrixParameter(std::shared_ptr<ArkRenderParameter11> pP);
		virtual DirectX::XMMATRIX* GetMatrixArrayParameter(std::shared_ptr<ArkRenderParameter11> pP) ;
		virtual int GetConstantBufferParameter( std::shared_ptr<ArkRenderParameter11> pParameter ) ;


		virtual std::shared_ptr<ArkRenderParameter11> GetParameterRef(const std::wstring& name) ;
		virtual std::shared_ptr<ArkVectorParameter11> GetVectorParameterRef(const std::wstring& name) ;
		virtual std::shared_ptr<ArkMatrixParameter11> GetMatrixParameterRef(const std::wstring& name)  ;
		virtual std::shared_ptr<ArkMatrixArrayParameter11> GetMatrixArrayParameterRef(const std::wstring& name,int count) ;
		virtual int GetConstantBufferParameter( const std::wstring& name );

		virtual void SetWorldMatrix(DirectX::XMMATRIX* w);
		virtual void SetViewMatrix(DirectX::XMMATRIX* v);
		virtual void SetProjectionMatrix(DirectX::XMMATRIX* p);

		static std::map<std::wstring,std::shared_ptr<ArkRenderParameter11>> m_Parameters;

		void AttachParent( IParameterManager* pParent );
		void DetachParent();

		unsigned int GetID();
	protected:
		std::shared_ptr<ArkMatrixParameter11>	m_pWorldMatrix;
		std::shared_ptr<ArkMatrixParameter11>	m_pViewMatrix;
		std::shared_ptr<ArkMatrixParameter11>	m_pProjMatrix;
		std::shared_ptr<ArkMatrixParameter11>	m_pWorldViewMatrix;
		std::shared_ptr<ArkMatrixParameter11>	m_pWorldProjMatrix;
		std::shared_ptr<ArkMatrixParameter11>	m_pViewProjMatrix;
		std::shared_ptr<ArkMatrixParameter11>	m_pWorldViewProjMatrix;


		unsigned int m_ID;
		IParameterManager*						m_pParent;

	};
};
//--------------------------------------------------------------------------------
#endif