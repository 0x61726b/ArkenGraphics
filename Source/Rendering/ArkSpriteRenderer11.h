//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkSpriteRenderer11.h
//--------------------------------------------------------------------------------
#ifndef __ArkSpriteRenderer11_h__
#define __ArkSpriteRenderer11_h__
//--------------------------------------------------------------------------------
#include "ArkRenderer11.h"
#include "ArkRenderEffect11.h"
#include "ArkDrawIndexedInstancedExecutor11.h"
#include "SpriteVertex11.h"
#include "ArkSpriteFont11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class PipelineManager;

	typedef std::shared_ptr<ArkDrawIndexedInstancedExecutor11<SpriteVertex11::VertexData,SpriteVertex11::InstanceData>> SpriteGeometryPtr;

	class ArkSpriteRenderer11
	{

	public:

		enum FilterMode
		{
			DontSet = 0,
			Linear = 1,
			Point = 2
		};

		static const UINT MaxBatchSize = 1000;

		ArkSpriteRenderer11();
		~ArkSpriteRenderer11();

		bool Initialize();

		void Render(	PipelineManager* pipeline,
						IParameterManager* parameters,
						ResourcePtr texture,
						const SpriteVertex11::InstanceData* drawData,
						UINT numSprites,
						FilterMode filterMode = Linear );

		void Render(	PipelineManager* pipeline,
						IParameterManager* parameters,
						ResourcePtr texture,
						const DirectX::XMFLOAT4X4& transform,
						const DirectX::XMFLOAT4& Color = DirectX::XMFLOAT4( 1, 1, 1, 1 ),
						FilterMode filterMode = Linear,
						const SpriteVertex11::SpriteDrawRect* drawRect = NULL );

		void RenderText(	PipelineManager* pipeline,
							IParameterManager* parameters,
							SpriteFontPtr pFont,
							const wchar_t* text,
							const DirectX::XMFLOAT4X4& transform,
							const DirectX::XMFLOAT4& color = DirectX::XMFLOAT4( 1, 1, 1, 1 ) );

	protected:

		void RenderCommon( ResourcePtr texture );

		ArkRenderEffect11 m_effect;

		SpriteGeometryPtr m_pGeometry;

		int m_iLinearSamplerState;
		int m_iPointSamplerState;

		bool m_bInitialized;

	};
}
#endif