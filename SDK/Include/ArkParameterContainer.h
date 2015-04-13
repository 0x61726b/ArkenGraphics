//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkParameterContainer.h
//--------------------------------------------------------------------------------
#ifndef __ArkParameterContainer_h__
#define __ArkParameterContainer_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkConstantBufferParameterWriter11.h"
#include "ArkMatrixParameterWriter11.h"
#include "ArkMatrixArrayParameterWriter11.h"
#include "ArkVectorParameterWriter11.h"
#include "ArkSamplerParameterWriter11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkParameterContainer
	{
	public:
		ArkParameterContainer();
		virtual ~ArkParameterContainer();

		// Adding a generic parameter writer to this container.
		void AddRenderParameter( std::shared_ptr<ArkParameterWriter> pWriter);

		std::shared_ptr<ArkParameterWriter> GetRenderParameter(const std::wstring& name);
		std::shared_ptr<ArkConstantBufferParameterWriter11> GetConstantBufferParameterWriter(const std::wstring& name);
		std::shared_ptr<ArkMatrixArrayParameterWriter11> GetMatrixArrayParameterWriter(const std::wstring& name);
		std::shared_ptr<ArkMatrixParameterWriter11> GetMatrixParameterWriter(const std::wstring& name);
		std::shared_ptr<ArkVectorParameterWriter11> GetVectorParameterWriter(const std::wstring& name);
		std::shared_ptr<ArkSamplerParameterWriter11>  GetSamplerParameterWriter( const std::wstring& name );

		std::shared_ptr<ArkVectorParameterWriter11> SetVectorParameter( const std::wstring& name,const DirectX::XMVECTOR& vec);
		std::shared_ptr<ArkConstantBufferParameterWriter11> SetConstantBufferParameter(const std::wstring& name,const ResourcePtr& value);
		std::shared_ptr<ArkMatrixArrayParameterWriter11> SetMatrixArrayParameter(const std::wstring& name,DirectX::XMMATRIX* values,int count);
		std::shared_ptr<ArkMatrixParameterWriter11> SetMatrixParameter(const std::wstring& name,const DirectX::XMMATRIX& value);
		std::shared_ptr<ArkSamplerParameterWriter11> SetSamplerParameter( const std::wstring& name, int value );

		void SetRenderParams(IParameterManager* pParamManager);
		void InitRenderParams();

	protected:

		std::vector< std::shared_ptr<ArkParameterWriter> > m_RenderParameters;
	};
};
//--------------------------------------------------------------------------------
#endif