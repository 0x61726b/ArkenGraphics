//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//App.h
//--------------------------------------------------------------------------------

#include "ArkRenderApplication11.h"
#include "Win32RenderWindow.h"
#include "ArkRenderer11.h"
using namespace Arkeng;

class App : public ArkApplication
{
public:
	App();
	~App();
	virtual void Initialize();
	virtual void Update();
	virtual void Shutdown();

	virtual bool ConfigureEngineComponents();
	virtual void ShutdownEngineComponents();

	virtual bool HandleEvent(EventPtr pEvent);
	virtual std::wstring GetName();

	ArkRenderEffect11		m_Effect;
	Win32RenderWindow*		m_pWindow;
	ArkRenderer11*			m_pRenderer;
	int						m_iSwapChain;
	ResourcePtr				m_RenderTarget;
	ResourcePtr				m_DepthTarget;

protected:
	ResourcePtr             m_pVertexBuffer;
	int                     m_VertexLayout;
	ResourcePtr             m_pIndexBuffer;

	
	DirectX::XMMATRIX m_WorldMatrix;
	DirectX::XMMATRIX m_ViewMatrix;
	DirectX::XMMATRIX m_ProjMatrix;
	
};