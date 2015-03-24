//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkBuffer11.h
//--------------------------------------------------------------------------------
#ifndef __ArkBuffer11_h__
#define __ArkBuffer11_h__
//--------------------------------------------------------------------------------
#include "Dx11Resource.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkBuffer11 : public Dx11Resource
	{
	public:
		ArkBuffer11();
		virtual ~ArkBuffer11();

		D3D11_BUFFER_DESC			GetActualDescription();
		D3D11_BUFFER_DESC			GetDesiredDescription();
		void						SetDesiredDescription( D3D11_BUFFER_DESC desc );

		UINT						GetByteWidth();
		void						SetByteWidth( UINT width );

		D3D11_USAGE					GetUsage();
		void						SetUsage( D3D11_USAGE usage );

		UINT						GetBindFlags();
		void						SetBindFlags( UINT flags );

		UINT						GetCPUAccessFlags();
		void						SetCPUAccessFlags( UINT flags );

		UINT						GetMiscFlags();
		void						SetMiscFlags( UINT flags );

		UINT						GetStructureByteStride();
		void						SetStructureByteStride( UINT stride );

		void*						Map();
		void						UnMap();

		virtual ID3D11Resource*				GetResource();

		virtual UINT						GetEvictionPriority();
		virtual void						SetEvictionPriority( UINT EvictionPriority );

	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_pBuffer;
		D3D11_BUFFER_DESC							m_DesiredDesc;
		D3D11_BUFFER_DESC							m_ActualDesc;

		friend ArkRenderer11;
	};
};
//--------------------------------------------------------------------------------
#endif