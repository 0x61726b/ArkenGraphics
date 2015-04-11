//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Dx11UnorderedAccessViewConfig.h
//--------------------------------------------------------------------------------
#ifndef __Dx11UnorderedAccessViewConfig_h__
#define __Dx11UnorderedAccessViewConfig_h__
//--------------------------------------------------------------------------------
#include "ArkRenderer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
    class Dx11UnorderedAccessViewConfig
    {
    public:
        Dx11UnorderedAccessViewConfig();
        virtual ~Dx11UnorderedAccessViewConfig();

        void SetDefaults();

        void SetFormat( DXGI_FORMAT state );
        void SetViewDimensions( D3D11_UAV_DIMENSION state );        

        void SetBuffer( D3D11_BUFFER_UAV state );
        void SetTexture1D( D3D11_TEX1D_UAV state );
        void SetTexture1DArray( D3D11_TEX1D_ARRAY_UAV state );
        void SetTexture2D( D3D11_TEX2D_UAV state );
        void SetTexture2DArray( D3D11_TEX2D_ARRAY_UAV state );
        void SetTexture3D( D3D11_TEX3D_UAV state);

        D3D11_UNORDERED_ACCESS_VIEW_DESC& GetUAVDesc();

    protected:
        D3D11_UNORDERED_ACCESS_VIEW_DESC    m_State;

        friend ArkRenderer11;
    };
};
//--------------------------------------------------------------------------------
#endif // UnorderedAccessViewConfigDX11_h
//--------------------------------------------------------------------------------

