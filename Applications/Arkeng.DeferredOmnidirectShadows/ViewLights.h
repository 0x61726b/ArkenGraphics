//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewLights.h
//--------------------------------------------------------------------------------
#ifndef __ViewLights_h__
#define __ViewLights_h__
//--------------------------------------------------------------------------------
#include "RenderTask.h"
#include "ArkGeometry11.h"
#include "ArkRenderEffect11.h"
#include "AppSettings.h"
#include "ArkMatrixParameter11.h"
#include "ArkVectorParameter11.h"
#include "ArkShaderResourceParameter11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
    // Light type
    enum LightType
    {
        Point = 0,
        Spot = 1,
        Directional = 2,

        NumLightTypes
    };

    // Light parameters
    struct LightParams
    {
        XMFLOAT3 Position;
        XMFLOAT3 Color;
        XMFLOAT3 Direction;
        float Range;
        float SpotInnerAngle;
        float SpotOuterAngle;
        LightType Type;
		XMMATRIX View;
		XMMATRIX Proj;

		XMFLOAT4X4	OmniViewMatrices[6];
        LightParams() : Position( 0.0f, 0.0f, 0.0f ), Color( 1.0f, 1.0f, 1.0f ), Direction( -1.0f, -1.0f, 1.0f ),
                Range( 2.0f ), SpotInnerAngle( 0 ), SpotOuterAngle ( 0 ), Type ( Point ), View( XMMatrixIdentity() ),Proj( XMMatrixIdentity() ) {}
    };

	class ViewLights : public RenderTask
    {
    public:
        ViewLights( ArkRenderer11& Renderer );
        virtual ~ViewLights();

        virtual void Update( float fTime );
		virtual void QueuePreTasks( ArkRenderer11* pRenderer );
		virtual void ExecuteTask( PipelineManager* pPipelineManager, IParameterManager* pParamManager );
		virtual void Resize( UINT width, UINT height );

        virtual void SetRenderParams( IParameterManager* pParamManager );
        virtual void SetUsageParams( IParameterManager* pParamManager );

        void AddLight( const LightParams& light );
        void SetTargets( std::vector<ResourcePtr>& GBufferTargets, ResourcePtr pRenderTarget,
                            ResourcePtr DepthTarget, int Viewport, int vpWidth, int vpHeight );
        void SetClipPlanes( float NearClip, float FarClip );

		virtual std::wstring GetName();

    protected:

        D3D11_RECT CalcScissorRect( const XMVECTOR& lightPos, float lightRange );

    protected:

        int								m_iDisabledDSState;
        int								m_iLessThanDSState;
        int								m_iGreaterThanDSState;
        int								m_iBlendState;
        int								m_iScissorRSState;
        int							    m_iBackFaceCullRSState;
        int								m_iFrontFaceCullRSState;

        UINT						    m_uVPWidth;
        UINT							m_uVPHeight;

        float							m_fNearClip;
        float							m_fFarClip;

        ResourcePtr						m_pRenderTarget;
        ResourcePtr						m_DepthTarget;
        ResourcePtr						m_ReadOnlyDepthTarget;
        std::vector<ResourcePtr>		m_GBufferTargets;

        GeometryPtr						m_QuadGeometry;
        GeometryPtr						m_SphereGeometry;
        GeometryPtr						m_ConeGeometry;

        ArkRenderEffect11				m_PointLightEffect[GBufferOptMode::NumSettings][LightOptMode::NumSettings][AAMode::NumSettings];
        ArkRenderEffect11				m_SpotLightEffect[GBufferOptMode::NumSettings][LightOptMode::NumSettings][AAMode::NumSettings];
        ArkRenderEffect11				m_DirectionalLightEffect[GBufferOptMode::NumSettings][LightOptMode::NumSettings][AAMode::NumSettings];

        std::vector<LightParams>		m_Lights;
        XMFLOAT4X4						m_WorldMatrix;

		std::shared_ptr<ArkMatrixParameter11>			m_pLightViewMatrix;
		std::shared_ptr<ArkMatrixParameter11>			m_pLightProjMatrix;
		std::shared_ptr<ArkMatrixParameter11>			m_pInvProjMatrix;
		std::shared_ptr<ArkMatrixParameter11>			m_pProjMatrix;
		std::shared_ptr<ArkVectorParameter11>			m_pCameraPos;
		std::shared_ptr<ArkVectorParameter11>			m_pLightPos;
		std::shared_ptr<ArkVectorParameter11>			m_pLightColor;
		std::shared_ptr<ArkVectorParameter11>			m_pLightDirection;
		std::shared_ptr<ArkVectorParameter11>			m_pLightRange;
		std::shared_ptr<ArkVectorParameter11>			m_pSpotlightAngles;

		std::shared_ptr<ArkShaderResourceParameter11>	m_pGBufferTargets[4];
		std::shared_ptr<ArkShaderResourceParameter11>	m_pDepthTexture;
    };
}
//--------------------------------------------------------------------------------
#endif // ViewLights_h