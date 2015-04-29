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
#include "VertexElement11.h"
#include "ArkLog.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;

std::string VertexElement11::PositionSemantic = "POSITION";
std::string VertexElement11::NormalSemantic = "NORMAL";
std::string VertexElement11::TexCoordSemantic = "TEXCOORDS";
//std::string VertexElement11::BoneIDSemantic = "BONEIDS";
//std::string VertexElementDX11::BoneWeightSemantic = "BONEWEIGHTS";
std::string VertexElement11::TangentSemantic = "TANGENT";
std::string VertexElement11::BinormalSemantic = "BINORMAL";

//--------------------------------------------------------------------------------
VertexElement11::VertexElement11( int float_tuple, int elementCount )
{
	m_iTuple = float_tuple;
	m_iCount = elementCount;
	m_pfData = new float[ m_iTuple * m_iCount ];
}
//--------------------------------------------------------------------------------
VertexElement11::~VertexElement11()
{
	delete[] m_pfData;
}
//--------------------------------------------------------------------------------
int	VertexElement11::SizeInBytes()
{
	return( m_iTuple * sizeof(float) );
}
//--------------------------------------------------------------------------------
int	VertexElement11::Tuple()
{
	return( m_iTuple );
}
//--------------------------------------------------------------------------------
int VertexElement11::Count()
{
	return( m_iCount );
}
//--------------------------------------------------------------------------------
float* VertexElement11::Get1f( int i )
{
	return( &m_pfData[ i ] );
}
//--------------------------------------------------------------------------------
XMFLOAT2* VertexElement11::Get2f( int i )
{
	return( (XMFLOAT2*)&m_pfData[ 2*i ] );
}
//--------------------------------------------------------------------------------
XMFLOAT3* VertexElement11::Get3f( int i )
{
	return( (XMFLOAT3*)&m_pfData[ 3*i ] );
}
//--------------------------------------------------------------------------------
XMFLOAT4* VertexElement11::Get4f( int i )
{
	return( (XMFLOAT4*)&m_pfData[ 4*i ] );
}
//--------------------------------------------------------------------------------
int* VertexElement11::Get1i( int i )
{
	return( (int*)&m_pfData[ i ] );
}
//--------------------------------------------------------------------------------
unsigned int* VertexElement11::Get1ui( int i )
{
	return( (unsigned int*)&m_pfData[ i ] );
}
//--------------------------------------------------------------------------------
float* VertexElement11::operator[]( int i )
{
	return( &m_pfData[ m_iTuple * i ] );
}
//--------------------------------------------------------------------------------
const float* VertexElement11::operator[]( int i ) const
{
	return( &m_pfData[ m_iTuple * i ] );
}
//--------------------------------------------------------------------------------
void* VertexElement11::GetPtr( int i )
{
	return( &m_pfData[ m_iTuple * i ] );
}
//--------------------------------------------------------------------------------