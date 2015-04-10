//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ViewTextOverlay.h
//--------------------------------------------------------------------------------
#ifndef __ViewTextOverlay_h__
#define __ViewTextOverlay_h__
//--------------------------------------------------------------------------------
#include "TaskCore.h"
#include "ArkFontLoader.h"
#include "ArkSpriteRenderer11.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkEntity3D;

	struct TextEntry
	{
		std::wstring text;
		XMFLOAT4X4 xform;
		XMFLOAT4 color;
		TextEntry() {};
		TextEntry( std::wstring& text, XMFLOAT4X4& xform, XMFLOAT4& color )
		{
			this->text = text;
			this->xform = xform;
			this->color = color;
		};
	};

	class ViewTextOverlay : public TaskCore
	{
	public:
		ViewTextOverlay( ArkRenderer11& Renderer, ResourcePtr RenderTarget );
		virtual ~ViewTextOverlay();

		virtual void Update( float fTime );
		virtual void QueuePreTasks( ArkRenderer11* pRenderer );
		virtual void ExecuteTask( PipelineManager* pPipelineManager, IParameterManager* pParamManager );
		virtual void Resize( UINT width, UINT height );

		virtual void SetRenderParams( IParameterManager* pParamManager );
		virtual void SetUsageParams( IParameterManager* pParamManager );

		void WriteText( std::wstring& text, XMFLOAT4X4& xform, XMFLOAT4& color );

		virtual std::wstring GetName();

	protected:
		XMFLOAT4				m_vColor;
		int						m_iViewport;

		ResourcePtr				m_RenderTarget;

		SpriteFontPtr			m_pSpriteFont;
		ArkSpriteRenderer11*		m_pSpriteRenderer;
		
		std::vector<TextEntry>	m_TextEntries;
	};
};
//--------------------------------------------------------------------------------
#endif // ViewTextOverlay_h
