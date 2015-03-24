//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//PipelineManager.h
//--------------------------------------------------------------------------------
#ifndef __PipelineManager_h__
#define __PipelineManager_h__
//--------------------------------------------------------------------------------
#include "ArkVertexShaderStage11.h"
#include "ArkPixelShaderStage11.h"
#include "Dx11ResourceProxy.h"
#include "Dx11Resource.h"
#include "DxOutputMergerStage.h"
#include "DxRasterizerStage.h"
#include "DxIAStage.h"
#include "ArkRenderEffect11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class PipelineManager
	{
	public:
		PipelineManager();
		~PipelineManager();

		void SetDeviceContext(DeviceContextComPtr Context,D3D_FEATURE_LEVEL FeatureLevel);

		void BindConstantBufferParameter( ShaderType type,std::shared_ptr<ArkRenderParameter11> pParam, UINT slot,IParameterManager* pParamManager );

		void BindShader( ShaderType type,int ID, IParameterManager* pParamManager );

		void ClearRenderTargets();
		void ApplyRenderTargets();

		void ApplyPipelineResources();
		void ClearPipelineResources();

		void ApplyInputResources();
		void ClearInputResources();

		void ClearPipelineState();

		void ClearBuffers( float color[],float depth );


		void Draw( ArkRenderEffect11& effect, ResourcePtr vb, ResourcePtr ib,
					int inputLayout, D3D11_PRIMITIVE_TOPOLOGY primType,
					UINT vertexStride, UINT numIndices, IParameterManager* pParamManager);

		D3D11_MAPPED_SUBRESOURCE MapResource( Dx11Resource* pArkResource, UINT subresource, D3D11_MAP actions, UINT flags );
		void UnMapResource(Dx11Resource* pArkResource,UINT subresource);

		D3D_FEATURE_LEVEL						m_FeatureLevel;

		DeviceContextComPtr			            m_pContext;

		ArkVertexShaderStage11					VertexShaderStage;
		ArkPixelShaderStage11					PixelShaderStage;

		ArkShaderStage11*						ShaderStages[2];

		DxOutputMergerStage						OutputMergerStage;
		DxRasterizerStage						RasterizerStage;
		DxIAStage								InputAssemblerStage;
	};
};
//--------------------------------------------------------------------------------
#endif