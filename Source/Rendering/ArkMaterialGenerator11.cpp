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
#include "ArkMaterialGenerator11.h"
#include "Dx11RasterizerStateConfig.h"
#include "Dx11DepthStencilStateConfig.h"
#include "Dx11BlendStateConfig.h"
#include "ArkShaderResourceParameterWriter11.h"
#include "ArkSamplerParameterWriter11.h"
#include "Dx11SamplerStateConfig.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkMaterialGenerator11::ArkMaterialGenerator11()
{
}
//--------------------------------------------------------------------------------
MaterialPtr ArkMaterialGenerator11::GenerateCascadedShadowMaps(ArkRenderer11& Renderer)
{
	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());
	std::shared_ptr<ArkRenderEffect11> pDepthEffect = std::make_shared<ArkRenderEffect11>();
	pDepthEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"DepthVS.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_5_0")
		));
	pDepthEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"DepthVS.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_5_0")
		));


	std::shared_ptr<ArkRenderEffect11> pCascadedEffect = std::make_shared<ArkRenderEffect11>();
	pCascadedEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"CascadedShadows.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_5_0")
		));
	pCascadedEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"CascadedShadows.hlsl"),
		std::wstring(L"PSMain"),
		std::wstring(L"ps_5_0")
		));

	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pCascadedEffect;


	pMaterial->Params[VT_LINEAR_DEPTH_NORMAL].bRender = true;
	pMaterial->Params[VT_LINEAR_DEPTH_NORMAL].pEffect = pDepthEffect;

	return pMaterial;
}
//--------------------------------------------------------------------------------
MaterialPtr ArkMaterialGenerator11::GenerateWireFrame(ArkRenderer11& Renderer)
{
	// Create the material that will be returned
	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());

	// Create and fill the effect that will be used for this view type
	auto pEffect = std::make_shared<ArkRenderEffect11>();;

	pEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"BasicTessellation.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_5_0")));

	pEffect->SetHullShader(Renderer.LoadShader(HULL_SHADER,
		std::wstring(L"BasicTessellation.hlsl"),
		std::wstring(L"HSMAIN"),
		std::wstring(L"hs_5_0")));

	pEffect->SetDomainShader(Renderer.LoadShader(DOMAIN_SHADER,
		std::wstring(L"BasicTessellation.hlsl"),
		std::wstring(L"DSMAIN"),
		std::wstring(L"ds_5_0")));

	pEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"BasicTessellation.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_5_0")));

	Dx11RasterizerStateConfig RS;
	RS.FillMode = D3D11_FILL_WIREFRAME;

	pEffect->m_iRasterizerState =
		Renderer.CreateRasterizerState(&RS);

	// Enable the material to render the given view type, and set its effect.
	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;

	return(pMaterial);
}
//--------------------------------------------------------------------------------
MaterialPtr ArkMaterialGenerator11::GenerateStaticTextured(ArkRenderer11& Renderer)
{
	// Create the material that will be returned
	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());

	// Create and fill the effect that will be used for this view type
	auto pEffect = std::make_shared<ArkRenderEffect11>();;

	pEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"MeshStaticTextured.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_5_0")));

	pEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"MeshStaticTextured.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_5_0")));

	Dx11RasterizerStateConfig RS;
	//RS.FillMode = D3D11_FILL_WIREFRAME;
	RS.CullMode = D3D11_CULL_NONE;

	pEffect->m_iRasterizerState =
		Renderer.CreateRasterizerState(&RS);

	// Enable the material to render the given view type, and set its effect.
	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;

	return(pMaterial);
}
//--------------------------------------------------------------------------------
MaterialPtr ArkMaterialGenerator11::GenerateSkinnedTextured(ArkRenderer11& Renderer)
{
	// Create the material that will be returned
	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());

	// Create and fill the effect that will be used for this view type
	auto pEffect = std::make_shared<ArkRenderEffect11>();;

	pEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"MeshSkinnedTextured.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_5_0")));

	pEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"MeshSkinnedTextured.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_5_0")));

	Dx11RasterizerStateConfig RS;
	//RS.FillMode = D3D11_FILL_WIREFRAME;
	RS.CullMode = D3D11_CULL_NONE;

	pEffect->m_iRasterizerState =
		Renderer.CreateRasterizerState(&RS);

	// Enable the material to render the given view type, and set its effect.
	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;

	return(pMaterial);
}
//--------------------------------------------------------------------------------
MaterialPtr ArkMaterialGenerator11::GenerateSkinnedSolid(ArkRenderer11& Renderer)
{
	// Create the material that will be returned
	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());

	// Create and fill the effect that will be used for this view type
	auto pEffect = std::make_shared<ArkRenderEffect11>();;

	pEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"MeshSkinnedTessellatedTextured.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_5_0")));

	pEffect->SetHullShader(Renderer.LoadShader(HULL_SHADER,
		std::wstring(L"MeshSkinnedTessellatedTextured.hlsl"),
		std::wstring(L"HSMAIN"),
		std::wstring(L"hs_5_0")));

	pEffect->SetDomainShader(Renderer.LoadShader(DOMAIN_SHADER,
		std::wstring(L"MeshSkinnedTessellatedTextured.hlsl"),
		std::wstring(L"DSMAIN"),
		std::wstring(L"ds_5_0")));

	pEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"MeshSkinnedTessellatedTextured.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_5_0")));

	Dx11RasterizerStateConfig RS;
	//RS.FillMode = D3D11_FILL_WIREFRAME;
	RS.CullMode = D3D11_CULL_NONE;

	pEffect->m_iRasterizerState =
		Renderer.CreateRasterizerState(&RS);

	// Enable the material to render the given view type, and set its effect.
	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;

	return(pMaterial);
}
//--------------------------------------------------------------------------------
MaterialPtr ArkMaterialGenerator11::GeneratePhong(ArkRenderer11& Renderer)
{
	// Create the material that will be returned
	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());

	// Create and fill the effect that will be used for this view type
	auto pEffect = std::make_shared<ArkRenderEffect11>();;

	pEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"PhongShading.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_5_0")));

	pEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"PhongShading.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_5_0")));

	Dx11RasterizerStateConfig RS;
	//RS.FillMode = D3D11_FILL_WIREFRAME;
	//RS.CullMode = D3D11_CULL_NONE;

	//pEffect->m_iRasterizerState = 
	//	Renderer.CreateRasterizerState( &RS );

	// Enable the material to render the given view type, and set its effect.
	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;

	return(pMaterial);
}
//--------------------------------------------------------------------------------
MaterialPtr ArkMaterialGenerator11::GenerateSolidColor(ArkRenderer11& Renderer)
{
	// Create the material that will be returned
	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());

	// Create and fill the effect that will be used for this view type
	auto pEffect = std::make_shared<ArkRenderEffect11>();;

	pEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"VertexColor.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_4_0")));

	pEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"VertexColor.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_4_0")));

	Dx11DepthStencilStateConfig ds;
	ds.DepthEnable = false;
	pEffect->m_iDepthStencilState = Renderer.CreateDepthStencilState(&ds);

	// Enable the material to render the given view type, and set its effect.
	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;

	return(pMaterial);
}
//--------------------------------------------------------------------------------
MaterialPtr ArkMaterialGenerator11::GenerateFromFile(ArkRenderer11& Renderer,std::wstring& file,unsigned int shaders)
{
	// Create the material that will be returned
	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());

	// Create and fill the effect that will be used for this view type
	auto pEffect = std::make_shared<ArkRenderEffect11>();;

	// Each type of shader is attempted to be loaded.  Any type not found in the 
	// file results in a log entry currently.  This should be addressed in the 
	// future by selectively logging the faults...

	if(shaders & VERTEX_SHADER_MSK)
		pEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		file,
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_5_0"),
		false));

	if(shaders & HULL_SHADER_MSK)
		pEffect->SetHullShader(Renderer.LoadShader(HULL_SHADER,
		file,
		std::wstring(L"HSMAIN"),
		std::wstring(L"hs_5_0")));

	if(shaders & DOMAIN_SHADER_MSK)
		pEffect->SetDomainShader(
		Renderer.LoadShader(DOMAIN_SHADER,
		file,
		std::wstring(L"DSMAIN"),
		std::wstring(L"ds_5_0")));

	if(shaders & GEOMETRY_SHADER_MSK)
		pEffect->SetGeometryShader(Renderer.LoadShader(GEOMETRY_SHADER,
		file,
		std::wstring(L"GSMAIN"),
		std::wstring(L"gs_5_0")));

	if(shaders & PIXEL_SHADER_MSK)
		pEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		file,
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_5_0")));

	if(shaders & COMPUTE_SHADER_MSK)
		pEffect->SetComputeShader(Renderer.LoadShader(COMPUTE_SHADER,
		file,
		std::wstring(L"CSMAIN"),
		std::wstring(L"cs_5_0")));

	// Enable the material to render the given view type, and set its effect.
	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;

	return(pMaterial);

}
//--------------------------------------------------------------------------------
MaterialPtr ArkMaterialGenerator11::GenerateKinectReconstructionMaterial(ArkRenderer11& Renderer)
{
	// Use the passed in material to render the visualization.  This allows for 
	// a user to create customized visualizations for special format textures.
	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());

	// Create and fill the effect that will be used for this view type
	auto pEffect = std::make_shared<ArkRenderEffect11>();;

	pEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"KinectBuffersVisualization.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_5_0")));

	pEffect->SetGeometryShader(Renderer.LoadShader(GEOMETRY_SHADER,
		std::wstring(L"KinectBuffersVisualization.hlsl"),
		std::wstring(L"GSMAIN"),
		std::wstring(L"gs_5_0")));

	pEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"KinectBuffersVisualization.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_5_0")));

	Dx11RasterizerStateConfig RS;
	RS.FillMode = D3D11_FILL_WIREFRAME;
	//RS.CullMode = D3D11_CULL_NONE;

	//pEffect->m_iRasterizerState = 
	//	Renderer.CreateRasterizerState( &RS );

	Dx11DepthStencilStateConfig ds;
	ds.DepthEnable = false;
	pEffect->m_iDepthStencilState = Renderer.CreateDepthStencilState(&ds);


	// Enable the material to render the given view type, and set its effect.
	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;

	return(pMaterial);
}
//--------------------------------------------------------------------------------
MaterialPtr ArkMaterialGenerator11::GenerateKinectDepthBufferMaterial(ArkRenderer11& Renderer)
{
	// Create the material that will be returned
	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());

	// Create and fill the effect that will be used for this view type
	auto pEffect = std::make_shared<ArkRenderEffect11>();;

	pEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"ObjectTexturedVS.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_5_0")));

	pEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"ObjectTexturedIntPS.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_5_0")));


	//RasterizerStateConfigDX11 RS;
	//RS.FillMode = D3D11_FILL_WIREFRAME;
	//RS.CullMode = D3D11_CULL_NONE;

	//pEffect->m_iRasterizerState = 
	//	Renderer.CreateRasterizerState( &RS );

	// Enable the material to render the given view type, and set its effect.
	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;

	return(pMaterial);
}
//--------------------------------------------------------------------------------
MaterialPtr ArkMaterialGenerator11::GenerateImmediateGeometrySolidMaterial(ArkRenderer11& Renderer)
{
	// Create the material that will be returned
	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());

	// Create and fill the effect that will be used for this view type
	auto pEffect = std::make_shared<ArkRenderEffect11>();;

	pEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"ImmediateGeometrySolid.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_4_0")));

	pEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"ImmediateGeometrySolid.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_4_0")));

	Dx11RasterizerStateConfig RS;
	RS.FillMode = D3D11_FILL_WIREFRAME;
	RS.CullMode = D3D11_CULL_NONE;

	//pEffect->m_iRasterizerState = 
	//	Renderer.CreateRasterizerState( &RS );


	// Enable the material to render the given view type, and set its effect.
	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;

	return(pMaterial);
}
//--------------------------------------------------------------------------------
MaterialPtr ArkMaterialGenerator11::GenerateImmediateGeometryTexturedMaterial(ArkRenderer11& Renderer)
{
	// Create the material that will be returned
	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());

	// Create and fill the effect that will be used for this view type
	auto pEffect = std::make_shared<ArkRenderEffect11>();;

	pEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"ImmediateGeometryTextured.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_4_0")));

	pEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"ImmediateGeometryTextured.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_4_0")));


	// Enable the material to render the given view type, and set its effect.
	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;


	// Load a texture to initialize with.  Then add a parameter writer to the 
	// material which will be used to set the texture for usage with this material.

	////////////ResourcePtr ColorTexture = ArkRenderer11::Get()->LoadTexture( L"EyeOfHorus_128.png" );
	////////////pMaterial->Parameters.SetShaderResourceParameter( L"ColorTexture", ColorTexture );

	// Create a sampler for use by this material.  Then add a parameter writer to the 
	// material which will be used to set the sampler when this material is used.

	Dx11SamplerStateConfig SamplerConfig;
	SamplerConfig.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerConfig.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerConfig.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerConfig.MaxAnisotropy = 0;

	int LinearSampler = ArkRenderer11::Get()->CreateSamplerState(&SamplerConfig);
	pMaterial->Parameters.SetSamplerParameter(L"LinearSampler",LinearSampler);

	return(pMaterial);
}
//--------------------------------------------------------------------------------
MaterialPtr ArkMaterialGenerator11::GenerateTextMaterial(ArkRenderer11& Renderer)
{
	// Create the material that will be returned
	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());

	// Create and fill the effect that will be used for this view type
	auto pEffect = std::make_shared<ArkRenderEffect11>();;

	pEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"AlphaTestTextured.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_4_0")));

	pEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"AlphaTestTextured.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_4_0")));


	// Create a special blend state to allow blending based on the alpha channel of the
	// source texture.  This allows the text to rendered without a background.  We set this
	// directly in the material provided by the material generator.

	Dx11BlendStateConfig blendConfig;
	blendConfig.AlphaToCoverageEnable = false;
	blendConfig.IndependentBlendEnable = false;
	for(int i = 0; i < 8; ++i)
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

	pEffect->m_iBlendState = Renderer.CreateBlendState(&blendConfig);

	//DepthStencilStateConfigDX11 ds;
	////ds.DepthEnable = false;
	////ds.DepthFunc = D3D11_COMPARISON_ALWAYS;
	////ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	//pEffect->m_iDepthStencilState = Renderer.CreateDepthStencilState( &ds );


	// Enable the material to render the given view type, and set its effect.
	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;


	// Load a texture to initialize with.  Then add a parameter writer to the 
	// material which will be used to set the texture for usage with this material.

	//////ResourcePtr ColorTexture = ArkRenderer11::Get()->LoadTexture( L"EyeOfHorus_128.png" );
	//////pMaterial->Parameters.SetShaderResourceParameter( L"ColorTexture", ColorTexture );

	// Create a sampler for use by this material.  Then add a parameter writer to the 
	// material which will be used to set the sampler when this material is used.

	Dx11SamplerStateConfig SamplerConfig;
	SamplerConfig.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerConfig.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerConfig.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerConfig.MaxAnisotropy = 0;

	int LinearSampler = ArkRenderer11::Get()->CreateSamplerState(&SamplerConfig);
	pMaterial->Parameters.SetSamplerParameter(L"LinearSampler",LinearSampler);

	return(pMaterial);
}
//--------------------------------------------------------------------------------
MaterialPtr ArkMaterialGenerator11::GenerateVolumeGeometryMaterial(ArkRenderer11& Renderer)
{
	// Create the material that will be returned
	MaterialPtr pMaterial = MaterialPtr(new ArkMaterial11());

	// Create and fill the effect that will be used for this view type
	auto pEffect = std::make_shared<ArkRenderEffect11>();;

	pEffect->SetVertexShader(Renderer.LoadShader(VERTEX_SHADER,
		std::wstring(L"VolumeGeometry.hlsl"),
		std::wstring(L"VSMAIN"),
		std::wstring(L"vs_5_0")));

	pEffect->SetPixelShader(Renderer.LoadShader(PIXEL_SHADER,
		std::wstring(L"VolumeGeometry.hlsl"),
		std::wstring(L"PSMAIN"),
		std::wstring(L"ps_5_0")));


	// Enable the material to render the given view type, and set its effect.
	pMaterial->Params[VT_PERSPECTIVE].bRender = true;
	pMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;


	// Load a texture to initialize with.  Then add a parameter writer to the 
	// material which will be used to set the texture for usage with this material.

	//ResourcePtr ColorTexture = ArkRenderer11::Get()->LoadTexture( L"EyeOfHorus_128.png" );
	//pMaterial->Parameters.SetShaderResourceParameter( L"ColorTexture", ColorTexture );

	// Create a sampler for use by this material.  Then add a parameter writer to the 
	// material which will be used to set the sampler when this material is used.

	Dx11SamplerStateConfig SamplerConfig;
	SamplerConfig.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	SamplerConfig.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	SamplerConfig.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	SamplerConfig.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerConfig.BorderColor[0] = 0.0f;
	SamplerConfig.BorderColor[1] = 0.0f;
	SamplerConfig.BorderColor[2] = 0.0f;
	SamplerConfig.BorderColor[3] = 0.0f;
	SamplerConfig.MaxAnisotropy = 0;

	int LinearSampler = ArkRenderer11::Get()->CreateSamplerState(&SamplerConfig);
	pMaterial->Parameters.SetSamplerParameter(L"LinearSampler",LinearSampler);

	return(pMaterial);
}
//--------------------------------------------------------------------------------