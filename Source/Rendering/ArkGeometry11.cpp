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
#include "PCH.h"
#include "ArkGeometry11.h"
#include "ArkVertexBuffer11.h"
#include "ArkIndexBuffer11.h"
#include "ArkBuffer11Config.h"
#include "ArkLog.h"
//#include "GlyphString.h"
#include "PipelineManager.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkGeometry11::ArkGeometry11()
{
	m_iVertexSize = 0;
	m_iVertexCount = 0;

	// Default to triangle lists
	m_ePrimType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}
//--------------------------------------------------------------------------------
ArkGeometry11::~ArkGeometry11()
{
	for(auto pElement : m_vElements)
	{
		if(pElement != nullptr)
			delete pElement;
	}
}
//--------------------------------------------------------------------------------
void ArkGeometry11::Execute(PipelineManager* pPipeline,IParameterManager* pParamManager)
{
	pPipeline->InputAssemblerStage.ClearCurrentState();

	// Set the Input Assembler state, then perform the draw call.
	int layout = GetInputLayout(pPipeline->ShaderStages[VERTEX_SHADER]->CurrentState.ShaderProgram.GetState());
	pPipeline->InputAssemblerStage.CurrentState.InputLayout.SetState(layout);
	pPipeline->InputAssemblerStage.CurrentState.PrimitiveTopology.SetState(m_ePrimType);

	pPipeline->InputAssemblerStage.CurrentState.VertexBuffers.SetState(0,m_VB->m_iResource);
	pPipeline->InputAssemblerStage.CurrentState.VertexBufferStrides.SetState(0,m_iVertexSize);
	pPipeline->InputAssemblerStage.CurrentState.VertexBufferOffsets.SetState(0,0);

	pPipeline->InputAssemblerStage.CurrentState.IndexBuffer.SetState(m_IB->m_iResource);
	pPipeline->InputAssemblerStage.CurrentState.IndexBufferFormat.SetState(DXGI_FORMAT_R32_UINT);

	pPipeline->ApplyInputResources();

	pPipeline->DrawIndexed(GetIndexCount(),0,0);
}
//--------------------------------------------------------------------------------
void ArkGeometry11::AddElement(VertexElement11* element)
{
	int index = -1;
	for(unsigned int i = 0; i < m_vElements.size(); i++)
	{
		if(m_vElements[i]->m_SemanticName == element->m_SemanticName)
			index = i;
	}

	if(index == -1)
	{
		m_vElements.push_back(element);		// the element wasn't found, so add it
	}
	else
	{
		delete m_vElements[index];		// delete the old element
		m_vElements[index] = element;	// set the new element
	}
}
////--------------------------------------------------------------------------------
void ArkGeometry11::AddFace(TriangleIndices& face)
{
	m_vIndices.push_back(face.P1());
	m_vIndices.push_back(face.P2());
	m_vIndices.push_back(face.P3());
}
////--------------------------------------------------------------------------------
void ArkGeometry11::AddLine(LineIndices& line)
{
	m_vIndices.push_back(line.P1());
	m_vIndices.push_back(line.P2());
}
//--------------------------------------------------------------------------------
void ArkGeometry11::AddPoint(PointIndices& point)
{
	m_vIndices.push_back(point.P1());
}
//--------------------------------------------------------------------------------
void ArkGeometry11::AddIndex(UINT index)
{
	m_vIndices.push_back(index);
}
//--------------------------------------------------------------------------------
VertexElement11* ArkGeometry11::GetElement(std::string name)
{
	VertexElement11* pElement = NULL;
	for(unsigned int i = 0; i < m_vElements.size(); i++)
	{
		if(m_vElements[i]->m_SemanticName == name)
			pElement = m_vElements[i];
	}

	return pElement;
}
//--------------------------------------------------------------------------------
VertexElement11* ArkGeometry11::GetElement(std::wstring name)
{
	std::string asciiName = std::wstring_convert<std::codecvt_utf8<wchar_t>,wchar_t>().to_bytes(name);
	return GetElement(asciiName);
}
//--------------------------------------------------------------------------------
VertexElement11* ArkGeometry11::GetElement(int index)
{
	return(m_vElements[index]);
}
//--------------------------------------------------------------------------------
UINT ArkGeometry11::GetIndex(unsigned int index)
{
	if(index < m_vIndices.size())
		return(m_vIndices[index]);

	ArkLog::Get(LogType::Renderer).Output(L"Tried to get an out of bounds index!");

	return(0);
}
//--------------------------------------------------------------------------------
D3D11_PRIMITIVE_TOPOLOGY ArkGeometry11::GetPrimitiveType()
{
	return(m_ePrimType);
}
//--------------------------------------------------------------------------------
int ArkGeometry11::GetPrimitiveCount()
{
	UINT count = 0;
	UINT indices = m_vIndices.size();

	switch(m_ePrimType)
	{
	case D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED:
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_POINTLIST:
		count = indices; break;
	case D3D11_PRIMITIVE_TOPOLOGY_LINELIST:
		count = indices / 2; break;
	case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP:
		count = indices - 1; break;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		count = indices / 3; break;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		count = indices - 2; break;
	case D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ:
		count = indices / 4; break;
	case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ:
		count = indices - 3; break;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ:
		count = indices / 6; break;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ:
		count = (indices - 3) / 2; break;
	case D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST:
		count = indices; break;
	case D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST:
		count = indices / 2; break;
	case D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST:
		count = indices / 3; break;
	case D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST:
		count = indices / 4; break;
	case D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST:
		count = indices / 5; break;
	case D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST:
		count = indices / 6; break;
	case D3D11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST:
		count = indices / 7; break;
	case D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST:
		count = indices / 8; break;
	case D3D11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST:
		count = indices / 9; break;
	case D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST:
		count = indices / 10; break;
	case D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST:
		count = indices / 11; break;
	case D3D11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST:
		count = indices / 12; break;
	case D3D11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST:
		count = indices / 13; break;
	case D3D11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST:
		count = indices / 14; break;
	case D3D11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST:
		count = indices / 15; break;
	case D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST:
		count = indices / 16; break;
	case D3D11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST:
		count = indices / 17; break;
	case D3D11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST:
		count = indices / 18; break;
	case D3D11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST:
		count = indices / 19; break;
	case D3D11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST:
		count = indices / 20; break;
	case D3D11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST:
		count = indices / 21; break;
	case D3D11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST:
		count = indices / 22; break;
	case D3D11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST:
		count = indices / 23; break;
	case D3D11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST:
		count = indices / 24; break;
	case D3D11_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST:
		count = indices / 25; break;
	case D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST:
		count = indices / 26; break;
	case D3D11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST:
		count = indices / 27; break;
	case D3D11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST:
		count = indices / 28; break;
	case D3D11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST:
		count = indices / 29; break;
	case D3D11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST:
		count = indices / 30; break;
	case D3D11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST:
		count = indices / 31; break;
	case D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST:
		count = indices / 32; break;
	}

	return(count);
}
//--------------------------------------------------------------------------------
void ArkGeometry11::SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY type)
{
	m_ePrimType = type;
}
//--------------------------------------------------------------------------------
int ArkGeometry11::GetVertexCount()
{
	return(m_iVertexCount);
}
//--------------------------------------------------------------------------------
int ArkGeometry11::GetElementCount()
{
	return(m_vElements.size());
}
//--------------------------------------------------------------------------------
int ArkGeometry11::GetVertexSize()
{
	return(m_iVertexSize);
}
//--------------------------------------------------------------------------------
int ArkGeometry11::CalculateVertexSize()
{
	// Reset the current vertex size 
	m_iVertexSize = 0;

	// Loop through the elements and add their per-vertex size
	for(auto pElement : m_vElements)
		m_iVertexSize += pElement->SizeInBytes();

	return(m_iVertexSize);
}
//--------------------------------------------------------------------------------
int ArkGeometry11::CalculateVertexCount()
{
	// Record the number of vertices as the number of vertices in the 
	// first element.  This could select the minimum number from all 
	// elements, but the user should have all the same size elements...
	if(m_vElements.size() > 0)
		m_iVertexCount = m_vElements[0]->Count();
	else
		m_iVertexCount = 0;

	return(m_iVertexCount);
}
//--------------------------------------------------------------------------------
void ArkGeometry11::GenerateInputLayout(int ShaderID)
{
	int iElems = m_vElements.size();

	if(iElems == 0)
	{
		// If no vertex elements exist in the geometry, then we can assume the 
		// vertex data will be generated in the shader itself.  In this case, the
		// number of vertices will be determined by the number of primitives.

		m_iVertexCount = GetPrimitiveCount();

		// Allocate the necessary number of element descriptions
		std::vector<D3D11_INPUT_ELEMENT_DESC> elements;

		// Create the input layout for the given shader index
		ArkRenderer11* pRenderer = ArkRenderer11::Get();
		if(m_InputLayouts[ShaderID] == 0)
		{
			InputLayoutKey* pKey = new InputLayoutKey();
			pKey->shader = ShaderID;
			pKey->layout = pRenderer->CreateInputLayout(elements,ShaderID);
			m_InputLayouts[ShaderID] = pKey;
		}
	}
	else
	{
		// Check the number of vertices to be created
		CalculateVertexCount();

		// Allocate the necessary number of element descriptions
		std::vector<D3D11_INPUT_ELEMENT_DESC> elements;

		// Fill in the vertex element descriptions based on each element
		for(unsigned int i = 0; i < m_vElements.size(); i++)
		{
			D3D11_INPUT_ELEMENT_DESC e;
			e.SemanticName = m_vElements[i]->m_SemanticName.c_str();
			e.SemanticIndex = m_vElements[i]->m_uiSemanticIndex;
			e.Format = m_vElements[i]->m_Format;
			e.InputSlot = m_vElements[i]->m_uiInputSlot;
			e.AlignedByteOffset = m_vElements[i]->m_uiAlignedByteOffset;
			e.InputSlotClass = m_vElements[i]->m_InputSlotClass;
			e.InstanceDataStepRate = m_vElements[i]->m_uiInstanceDataStepRate;

			elements.push_back(e);
		}

		// Create the input layout for the given shader index

		ArkRenderer11* pRenderer = ArkRenderer11::Get();
		if(m_InputLayouts[ShaderID] == 0)
		{
			InputLayoutKey* pKey = new InputLayoutKey();
			pKey->shader = ShaderID;
			pKey->layout = pRenderer->CreateInputLayout(elements,ShaderID);
			m_InputLayouts[ShaderID] = pKey;
		}
	}
}
//--------------------------------------------------------------------------------
void ArkGeometry11::LoadToBuffers()
{
	// Check the number of vertices to be created
	CalculateVertexCount();

	// Check the size of the assembled vertices
	CalculateVertexSize();

	if(GetVertexSize() > 0)
	{
		// Load the vertex buffer first by calculating the required size
		unsigned int vertices_length = GetVertexSize() * GetVertexCount();

		char* pBytes = new char[vertices_length];

		for(int j = 0; j < m_iVertexCount; j++)
		{
			int iElemOffset = 0;
			for(unsigned int i = 0; i < m_vElements.size(); i++)
			{
				memcpy(pBytes + j * m_iVertexSize + iElemOffset,m_vElements[i]->GetPtr(j),m_vElements[i]->SizeInBytes());
				iElemOffset += m_vElements[i]->SizeInBytes();
			}
		}

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = reinterpret_cast<void*>(pBytes);
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		ArkBuffer11Config vbuffer;
		vbuffer.SetDefaultVertexBuffer(vertices_length,false);
		m_VB = ArkRenderer11::Get()->CreateVertexBuffer(&vbuffer,&data);

		delete[] pBytes;
	}

	// Load the index buffer by calculating the required size
	// based on the number of indices.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = reinterpret_cast<void*>(&m_vIndices[0]);
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	ArkBuffer11Config ibuffer;
	ibuffer.SetDefaultIndexBuffer(sizeof(UINT) * GetIndexCount(),false);
	m_IB = ArkRenderer11::Get()->CreateIndexBuffer(&ibuffer,&data);
}
//--------------------------------------------------------------------------------
UINT ArkGeometry11::GetIndexCount()
{
	return(m_vIndices.size());
}
//--------------------------------------------------------------------------------
bool ArkGeometry11::ComputeTangentFrame(std::string positionSemantic,
	std::string normalSemantic,
	std::string texCoordSemantic,
	std::string tangentSemantic)
{
	// Only works for triangle lists    
	if(m_ePrimType != D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		ArkLog::Get(LogType::Renderer).Output(L"Tangent frame computation failed, geometry wasn't a triangle list");
		return false;
	}

	// Needs to be indexed
	if(m_vIndices.size() == 0)
	{
		ArkLog::Get(LogType::Renderer).Output(L"Tangent frame computation failed, geometry wasn't indexed");
		return false;
	}

	// Look for our vertex elements
	VertexElement11* pPositionElement = GetElement(positionSemantic);
	if(pPositionElement == NULL)
	{
		ArkLog::Get(LogType::Renderer).Output(L"Tangent frame computation failed, unable to find position vertex element");
		return false;
	}

	VertexElement11* pNormalElement = GetElement(normalSemantic);
	if(pNormalElement == NULL)
	{
		ArkLog::Get(LogType::Renderer).Output(L"Tangent frame computation failed, unable to find normal vertex element");
		return false;
	}

	VertexElement11* pTexCoordElement = GetElement(texCoordSemantic);
	if(pTexCoordElement == NULL)
	{
		ArkLog::Get(LogType::Renderer).Output(L"Tangent frame computation failed, unable to find texture coordinate vertex element");
		return false;
	}

	// Add the new element for the tangent
	VertexElement11* pTangentElement = new VertexElement11(4,CalculateVertexCount());
	pTangentElement->m_SemanticName = VertexElement11::TangentSemantic;
	pTangentElement->m_uiSemanticIndex = 0;
	pTangentElement->m_Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	pTangentElement->m_uiInputSlot = 0;
	pTangentElement->m_uiAlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pTangentElement->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pTangentElement->m_uiInstanceDataStepRate = 0;

	// Compute the tangent frame for each vertex. The following code is based on 
	// "Computing Tangent Space Basis Vectors for an Arbitrary Mesh", by Eric Lengyel
	// http://www.terathon.com/code/tangent.html

	// Make temporary arrays for the tangent and the bitangent
	XMFLOAT3* tangents= new XMFLOAT3[m_iVertexCount];

	XMFLOAT3* bitangents = new XMFLOAT3[m_iVertexCount];

	ZeroMemory(tangents,m_iVertexCount * sizeof(XMFLOAT3));
	ZeroMemory(bitangents,m_iVertexCount * sizeof(XMFLOAT3));

	int counter = 0;
	// Loop through each triangle    
	for(UINT i = 0; i < m_vIndices.size(); i+=3)
	{
		UINT i1 = m_vIndices[i + 0];
		UINT i2 = m_vIndices[i + 1];
		UINT i3 = m_vIndices[i + 2];

		const XMFLOAT3& v1 = *pPositionElement->Get3f(i1);
		const XMFLOAT3& v2 = *pPositionElement->Get3f(i2);
		const XMFLOAT3& v3 = *pPositionElement->Get3f(i3);


		const XMFLOAT2& w1 = *pTexCoordElement->Get2f(i1);
		const XMFLOAT2& w2 = *pTexCoordElement->Get2f(i2);
		const XMFLOAT2& w3 = *pTexCoordElement->Get2f(i3);

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;


		float r = 1.0f / (s1 * t2 - s2 * t1) ;


		XMFLOAT3 sDir((t2 * x1 - t1 * x2) * r,(t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r);
		XMFLOAT3 tDir((s1 * x2 - s2 * x1) * r,(s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r);


		tangents[i1] = XMFLOAT3(tangents[i1].x + sDir.x,tangents[i1].y + sDir.y,tangents[i1].z + sDir.z);
		tangents[i2] = XMFLOAT3(tangents[i2].x + sDir.x,tangents[i2].y + sDir.y,tangents[i2].z + sDir.z);
		tangents[i3] = XMFLOAT3(tangents[i3].x + sDir.x,tangents[i3].y + sDir.y,tangents[i3].z + sDir.z);;


		bitangents[i1] = XMFLOAT3(bitangents[i1].x + tDir.x,bitangents[i1].y + tDir.y,bitangents[i1].z + tDir.z);
		bitangents[i2] = XMFLOAT3(bitangents[i2].x + tDir.x,bitangents[i2].y + tDir.y,bitangents[i2].z + tDir.z);
		bitangents[i3] = XMFLOAT3(bitangents[i3].x + tDir.x,bitangents[i3].y + tDir.y,bitangents[i3].z + tDir.z);
	}

	for(int i = 0; i < m_iVertexCount; ++i)
	{
		XMFLOAT3& n = *pNormalElement->Get3f(i);
		XMFLOAT3& t = tangents[i];

		XMVECTOR tv,nv;
		nv = XMLoadFloat3(&n);
		tv = XMLoadFloat3(&t);

		// Gram-Schmidt orthogonalize
		XMVECTOR dotnt = XMVector3Dot(nv,tv);
		XMVECTOR crossnt = XMVector3Cross(nv,tv);

		XMVECTOR tangent = XMVector3Normalize((tv - nv * dotnt));
		XMFLOAT3 tan3;
		XMStoreFloat3(&tan3,tangent);

		XMVECTOR bitangentV;
		bitangentV = XMLoadFloat3(&bitangents[i]);

		XMFLOAT3 sign3;
		XMVECTOR r = XMVector3Dot(crossnt,bitangentV) ;
		XMStoreFloat3(&sign3,r);
		// Calculate handedness
		float sign = (sign3.x < 0.0f) ? -1.0f : 1.0f;

		XMFLOAT4& finalTangent = *pTangentElement->Get4f(i);
		finalTangent = XMFLOAT4(tan3.x,tan3.y,tan3.z,sign);


	}

	// Clean up our temporary arrays
	delete[] tangents;
	delete[] bitangents;

	AddElement(pTangentElement);

	return true;
}
//--------------------------------------------------------------------------------
