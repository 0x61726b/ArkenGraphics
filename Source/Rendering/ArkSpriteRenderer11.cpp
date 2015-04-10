//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkSpriteRenderer11.h"
#include "ArkRenderer11.h"
#include "ArkLog.h"
#include "Dx11BlendStateConfig.h"
#include "Dx11RasterizerStateConfig.h"
#include "Dx11DepthStencilStateConfig.h"
#include "ArkSpriteFont11.h"
#include "PipelineManager.h"
#include "ArkParameterManager11.h"
#include "ArkBuffer11Config.h"
#include "Dx11Texture2DConfig.h"
#include "Dx11ViewPort.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkSpriteRenderer11::ArkSpriteRenderer11() :
									m_iLinearSamplerState(-1),
									m_iPointSamplerState(-1),
									m_bInitialized(false)
{

}
//--------------------------------------------------------------------------------
ArkSpriteRenderer11::~ArkSpriteRenderer11()
{

}
//--------------------------------------------------------------------------------
bool ArkSpriteRenderer11::Initialize()
{
	// Get the renderer
	ArkRenderer11* renderer = ArkRenderer11::Get();

	// Load the shaders
	m_effect.SetVertexShader( renderer->LoadShader( VERTEX_SHADER,
		std::wstring( L"Sprite.hlsl" ),
		std::wstring( L"SpriteVS" ),
		std::wstring( L"vs_4_0" ) ) );

	if ( m_effect.GetVertexShader() == -1 )
	{
		ArkLog::Get(LogType::Renderer).Output( L"Failed to load sprite vertex shader" );
		return false;
	}

	m_effect.SetPixelShader( renderer->LoadShader( PIXEL_SHADER,
		std::wstring( L"Sprite.hlsl" ),
		std::wstring( L"SpritePS" ),
		std::wstring( L"ps_4_0" ) ) );

	if ( m_effect.GetPixelShader() == -1 )
	{
		ArkLog::Get(LogType::Renderer).Output( L"Failed to load sprite vertex shader" );
		return false;
	}


	// Create our states
	Dx11RasterizerStateConfig rsConfig;
	rsConfig.AntialiasedLineEnable = FALSE;
	rsConfig.CullMode = D3D11_CULL_NONE;
	rsConfig.DepthBias = 0;
	rsConfig.DepthBiasClamp = 1.0f;
	rsConfig.DepthClipEnable = false;
	rsConfig.FillMode = D3D11_FILL_SOLID;
	rsConfig.FrontCounterClockwise = false;
	rsConfig.MultisampleEnable = true;
	rsConfig.ScissorEnable = false;
	rsConfig.SlopeScaledDepthBias = 0;
	m_effect.m_iRasterizerState = renderer->CreateRasterizerState( &rsConfig );

	if ( m_effect.m_iRasterizerState == -1 )
	{
		ArkLog::Get(LogType::Renderer).Output( L"Failed to create sprite rasterizer state" );
		return false;
	}

	Dx11BlendStateConfig blendConfig;
	blendConfig.AlphaToCoverageEnable = false;
	blendConfig.IndependentBlendEnable = false;
	for ( int i = 0; i < 8; ++i )
	{
		blendConfig.RenderTarget[i].BlendEnable = true;
		blendConfig.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendConfig.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendConfig.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendConfig.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendConfig.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendConfig.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
		blendConfig.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	m_effect.m_iBlendState = renderer->CreateBlendState( &blendConfig );

	if ( m_effect.m_iBlendState == -1 )
	{
		ArkLog::Get(LogType::Renderer).Output( L"Failed to create sprite blend state" );
		return false;
	}

	Dx11DepthStencilStateConfig dsConfig;
	dsConfig.DepthEnable = FALSE;
	dsConfig.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsConfig.DepthFunc = D3D11_COMPARISON_LESS;
	dsConfig.StencilEnable = false;
	dsConfig.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsConfig.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsConfig.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsConfig.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsConfig.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dsConfig.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsConfig.BackFace = dsConfig.FrontFace;

	m_effect.m_iDepthStencilState = renderer->CreateDepthStencilState( &dsConfig );

	if ( m_effect.m_iDepthStencilState == -1 )
	{
		ArkLog::Get(LogType::Renderer).Output( L"Failed to create sprite depth stencil state" );
		return false;
	}

	// Linear filtering sampler state
	D3D11_SAMPLER_DESC samplerConfig;
	samplerConfig.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerConfig.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerConfig.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerConfig.BorderColor[0] = 0;
	samplerConfig.BorderColor[1] = 0;
	samplerConfig.BorderColor[2] = 0;
	samplerConfig.BorderColor[3] = 0;
	samplerConfig.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerConfig.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerConfig.MaxAnisotropy = 1;
	samplerConfig.MaxLOD = D3D11_FLOAT32_MAX;
	samplerConfig.MinLOD = 0;
	samplerConfig.MipLODBias = 0;

	m_iLinearSamplerState = renderer->CreateSamplerState( &samplerConfig );

	if ( m_iLinearSamplerState == -1 )
	{
		ArkLog::Get(LogType::Renderer).Output( L"Failed to create sprite sampler state" );
		return false;
	}

	// Point filtering sampler state
	samplerConfig.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

	m_iPointSamplerState = renderer->CreateSamplerState( &samplerConfig );

	if ( m_iPointSamplerState == -1 )
	{
		ArkLog::Get(LogType::Renderer).Output( L"Failed to create sprite sampler state" );
		return false;
	}

	m_pGeometry = SpriteGeometryPtr( new ArkDrawIndexedInstancedExecutor11<SpriteVertex11::VertexData, SpriteVertex11::InstanceData>() );
	m_pGeometry->SetLayoutElements( SpriteVertex11::GetElementCount(), SpriteVertex11::Elements );

	// Set up the vertex data here first.  This is essentially just the four
	// vertices that will make up a quad.
	
	SpriteVertex11::VertexData verts[] =
	{
		{ DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
		{ DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) }
	};

	m_pGeometry->AddVertex( verts[0] );
	m_pGeometry->AddVertex( verts[1] );
	m_pGeometry->AddVertex( verts[2] );
	m_pGeometry->AddVertex( verts[3] );

	// Next, set up the index data.  This will provide the indices for each 
	// instance that gets replicated during the draw call.  Since we are just
	// replicating a quad made of two triangles, we only need two triangles.

	m_pGeometry->AddIndices( 0, 1, 2 );
	m_pGeometry->AddIndices( 3, 0, 2 );

	// Ensure the compatibility of the pipeline executor and the render effect
	// that is going to be used with it.

	m_pGeometry->GenerateInputLayout( m_effect.GetVertexShader() );

	m_bInitialized = true;

	return true;
}
//--------------------------------------------------------------------------------
void ArkSpriteRenderer11::Render( PipelineManager* pipeline,
								 IParameterManager* parameters,
								 ResourcePtr texture,
								 const SpriteVertex11::InstanceData* drawData,
								 UINT numSprites, FilterMode filterMode )
{
	_ASSERT(m_bInitialized);

	pipeline->BeginEvent( std::wstring( L"SpriteRendererDX11 Render" ) );

	// Only reset the instances, since the vertex and index data remains the same!
	m_pGeometry->ResetInstances();

	// Make sure the draw rects are all valid
	D3D11_TEXTURE2D_DESC desc = texture->m_pTexture2dConfig->GetTextureDesc();
	for ( UINT i = 0; i < numSprites; ++i )
	{
		SpriteVertex11::SpriteDrawRect drawRect = drawData[i].DrawRect;
		_ASSERT( drawRect.X >= 0 && drawRect.X < desc.Width );
		_ASSERT( drawRect.Y >= 0 && drawRect.Y < desc.Height );
		_ASSERT( drawRect.Width > 0 && drawRect.X + drawRect.Width <= desc.Width );
		_ASSERT( drawRect.Height > 0 && drawRect.Y + drawRect.Height <= desc.Height );

		m_pGeometry->AddInstance( drawData[i] );
	}

	// Set the constants
	DirectX::XMFLOAT4 texAndViewportSize;
	texAndViewportSize.x = static_cast<float>( desc.Width );
	texAndViewportSize.y = static_cast<float>( desc.Height );

	int viewportID = pipeline->RasterizerStage.CurrentState.ViewPorts.GetState( 0 );
	Dx11ViewPort vp = ArkRenderer11::Get()->GetViewPort( viewportID );
	texAndViewportSize.z = static_cast<float>( vp.GetWidth() );
	texAndViewportSize.w = static_cast<float>( vp.GetHeight());

	parameters->SetVectorParameter( L"TexAndViewportSize", &XMLoadFloat4( &texAndViewportSize) );

	// Set the texture
	parameters->SetShaderResourceParameter( L"SpriteTexture", texture );

	// Set the sampler
	if ( filterMode == Linear )
		parameters->SetSamplerParameter( L"SpriteSampler", &m_iLinearSamplerState );
	else if ( filterMode == Point )
		parameters->SetSamplerParameter( L"SpriteSampler", &m_iPointSamplerState );

	pipeline->ClearPipelineResources();
	m_effect.ConfigurePipeline( pipeline, parameters );
	pipeline->ApplyPipelineResources();

	m_pGeometry->Execute( pipeline, parameters );

	pipeline->EndEvent();
}
//--------------------------------------------------------------------------------
void ArkSpriteRenderer11::Render( PipelineManager* pipeline, 
								 IParameterManager* parameters,
							     ResourcePtr texture, 
								 const DirectX::XMFLOAT4X4& transform,
								 const DirectX::XMFLOAT4& color, FilterMode filterMode,
								 const SpriteVertex11::SpriteDrawRect* drawRect )
{
	SpriteVertex11::InstanceData data;
	data.Color = color;
	data.Transform = transform;

	if ( drawRect )
		data.DrawRect = *drawRect;
	else
	{
		// Draw the full texture
		D3D11_TEXTURE2D_DESC textureDesc = texture->m_pTexture2dConfig->GetTextureDesc();
		data.DrawRect.X = 0;
		data.DrawRect.Y = 0;
		data.DrawRect.Width = static_cast<float>( textureDesc.Width );
		data.DrawRect.Height = static_cast<float>( textureDesc.Height );
	}

	Render( pipeline, parameters, texture, &data, 1, filterMode );
}
//--------------------------------------------------------------------------------
void ArkSpriteRenderer11::RenderText( PipelineManager* pipeline, 
									 IParameterManager* parameters,
									 SpriteFontPtr pFont, const wchar_t* text,
									 const  DirectX::XMFLOAT4X4& transform, const  DirectX::XMFLOAT4& color )
{
	pipeline->BeginEvent( std::wstring( L"SpriteRenderer RenderText" ) );

	SpriteVertex11::InstanceData m_TextDrawData [MaxBatchSize];

	size_t length = wcslen( text );

	XMFLOAT4X4 textTransform4f;
	DirectX::XMStoreFloat4x4( &textTransform4f,DirectX::XMMatrixIdentity() );

	size_t numCharsToDraw = min( length, MaxBatchSize );
	UINT currentDraw = 0;

	for (size_t i = 0; i < numCharsToDraw; ++i)
	{
		wchar_t character = text[i];
		if(character == ' ')
			textTransform4f._41 += pFont->SpaceWidth();
		else if(character == '\n')
		{
			textTransform4f._42  += pFont->CharHeight();
			textTransform4f._41  = 0;
		}
		else
		{
			ArkSpriteFont11::CharDesc desc = pFont->GetCharDescriptor(character);

			XMMATRIX t1 = XMLoadFloat4x4( &textTransform4f );
			XMMATRIX t2 = XMLoadFloat4x4( &transform );
			XMMATRIX m = t1*t2;
			XMStoreFloat4x4( &m_TextDrawData[currentDraw].Transform,m );

			m_TextDrawData[currentDraw].Color = color;
			m_TextDrawData[currentDraw].DrawRect.X = desc.X;
			m_TextDrawData[currentDraw].DrawRect.Y = desc.Y;
			m_TextDrawData[currentDraw].DrawRect.Width = desc.Width;
			m_TextDrawData[currentDraw].DrawRect.Height = desc.Height;
			currentDraw++;

			textTransform4f._41  += desc.Width + 1;
		}
	}

	

	// Submit a batch
	Render( pipeline, parameters, pFont->TextureResource(), m_TextDrawData, currentDraw, Point );

	pipeline->EndEvent();

	if( length > numCharsToDraw )
		RenderText( pipeline, parameters, pFont, text + numCharsToDraw, textTransform4f, color );
}
//--------------------------------------------------------------------------------