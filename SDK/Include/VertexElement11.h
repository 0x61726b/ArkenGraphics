//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//VertexElement11.h
//--------------------------------------------------------------------------------
#ifndef __VertexElement11_h__
#define __VertexElement11_h__
//--------------------------------------------------------------------------------
#include "PCH.h"
using namespace DirectX;;
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class VertexElement11
	{

    public:

        // Standard semantic names
        static std::string PositionSemantic;
        static std::string NormalSemantic;
        static std::string TexCoordSemantic;
  //      static std::string BoneIDSemantic;
		//static std::string BoneWeightSemantic;
        static std::string TangentSemantic;
		static std::string BinormalSemantic;

	public:
		VertexElement11( int tuple, int elementCount );
		~VertexElement11( );
		
		int				SizeInBytes();
		int				Count();
		int				Tuple();

		void*			GetPtr( int i );

		float*			Get1f( int i );
		XMFLOAT2*		Get2f( int i );
		XMFLOAT3*		Get3f( int i );
		XMFLOAT4*		Get4f( int i );

		int*			Get1i( int i );

		unsigned int*	Get1ui( int i );

		float*					operator[]( int i );
		const float*			operator[]( int i ) const;

		std::string						m_SemanticName;
		UINT							m_uiSemanticIndex;
		DXGI_FORMAT						m_Format;
		UINT							m_uiInputSlot;
		UINT							m_uiAlignedByteOffset;
		D3D11_INPUT_CLASSIFICATION		m_InputSlotClass;
		UINT							m_uiInstanceDataStepRate;

	protected:
		VertexElement11();

		float*							m_pfData;
		int								m_iTuple;
		int								m_iCount;
	};
};
#endif // VertexElementDX11_h
