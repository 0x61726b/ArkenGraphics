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
#include "ArkGeometryLoader11.h"
#include "ArkGeometry11.h"
#include "VertexElement11.h"
#include "ms3dspec.h"
#include "ArkLog.h"
//#include "ArkMaterialGenerator11.h"
#include <sstream>
#include "ArkFileSystem.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
ArkGeometryLoader11::ArkGeometryLoader11()
{
}
//--------------------------------------------------------------------------------
GeometryPtr ArkGeometryLoader11::LoadFbxFile(std::wstring filename)
{
	static FbxManager* m_pFbxSdkManager = nullptr;

	m_pFbxSdkManager = FbxManager::Create();
	FbxIOSettings* pIOsettings = FbxIOSettings::Create(m_pFbxSdkManager,IOSROOT);

	m_pFbxSdkManager->SetIOSettings(pIOsettings);

	FbxImporter* pImporter = FbxImporter::Create(m_pFbxSdkManager,"");
	FbxScene* pFbxScene = FbxScene::Create(m_pFbxSdkManager,"");

	ArkFileSystem fs;
	filename = fs.GetModelsDirectory() + filename;
	std::string fileName;
	fileName.assign(filename.begin(),filename.end());
	bool bSuccess = pImporter->Initialize(fileName.c_str(),-1,m_pFbxSdkManager->GetIOSettings());

	if(!bSuccess)
		return NULL;

	bSuccess = pImporter->Import(pFbxScene);

	if(!bSuccess)
		return NULL;

	pImporter->Destroy();

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	UINT triangleCount = 0;
	std::vector<XMFLOAT3> pPos;
	std::vector<XMFLOAT3> pNrm;
	std::vector<XMFLOAT2> PTex;
	if(pFbxRootNode)
	{
		for(int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if(pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if(AttributeType != FbxNodeAttribute::eMesh)
				continue;

			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();

			FbxVector4* pVertices = pMesh->GetControlPoints();
			FbxArray<FbxVector4> pNormals;
			pMesh->GetPolygonVertexNormals(pNormals);

			for(int j = 0; j < pMesh->GetPolygonCount(); j++)
			{
				int iNumVertices = pMesh->GetPolygonSize(j);

				assert(iNumVertices == 3);
				triangleCount++;

				for(int k = 0; k < iNumVertices; k++)
				{
					int iControlPointIndex = pMesh->GetPolygonVertex(j,k);

					FbxVector4 normalFbx;
					bool s = pMesh->GetPolygonVertexNormal(j,k,normalFbx);

					FbxVector2 fbxTexCoord;
					FbxStringList UVSetNameList;

					// Get the name of each set of UV coords
					pMesh->GetUVSetNames(UVSetNameList);

					// Get the UV coords for this vertex in this poly which belong to the first UV set
					// Note: Using 0 as index into UV set list as this example supports only one UV set
					bool unmapped;
					s = pMesh->GetPolygonVertexUV(j,k,UVSetNameList.GetStringAt(0),fbxTexCoord,unmapped);


					XMFLOAT3 vertex;
					vertex.x = (float)pVertices[iControlPointIndex].mData[0];
					vertex.y = (float)pVertices[iControlPointIndex].mData[1];
					vertex.z = (float)pVertices[iControlPointIndex].mData[2];
					pPos.push_back(vertex);

					XMFLOAT3 norm;
					norm.x = (float)normalFbx.mData[0];
					norm.y = (float)normalFbx.mData[1];
					norm.z = -(float)normalFbx.mData[2];
					pNrm.push_back(norm);

					XMFLOAT2 tex;
					tex.x = (float)fbxTexCoord.mData[0];
					tex.y = (float)fbxTexCoord.mData[1];
					PTex.push_back(tex);

				}
			}
			pMesh->Destroy();

			pFbxChildNode->Destroy();

		}
	}
	pFbxRootNode->Destroy();
	pFbxScene->Destroy();
	m_pFbxSdkManager->Destroy();
	m_pFbxSdkManager = nullptr;


	VertexElement11* pPositions = new VertexElement11(3,triangleCount*3);
	pPositions->m_SemanticName = VertexElement11::PositionSemantic;
	pPositions->m_uiSemanticIndex = 0;
	pPositions->m_Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pPositions->m_uiInputSlot = 0;
	pPositions->m_uiAlignedByteOffset = 0;
	pPositions->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pPositions->m_uiInstanceDataStepRate = 0;

	VertexElement11* pTexcoords = new VertexElement11(2,triangleCount*3);
	pTexcoords->m_SemanticName = VertexElement11::TexCoordSemantic;
	pTexcoords->m_uiSemanticIndex = 0;
	pTexcoords->m_Format = DXGI_FORMAT_R32G32_FLOAT;
	pTexcoords->m_uiInputSlot = 0;
	pTexcoords->m_uiAlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pTexcoords->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pTexcoords->m_uiInstanceDataStepRate = 0;

	VertexElement11* pNormals = new VertexElement11(3,triangleCount*3);
	pNormals->m_SemanticName = VertexElement11::NormalSemantic;
	pNormals->m_uiSemanticIndex = 0;
	pNormals->m_Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pNormals->m_uiInputSlot = 0;
	pNormals->m_uiAlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pNormals->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pNormals->m_uiInstanceDataStepRate = 0;

	XMFLOAT3* pPos3 = pPositions->Get3f(0);
	XMFLOAT3* pNormal3 = pNormals->Get3f(0);
	XMFLOAT2* pTex = pTexcoords->Get2f(0);

	GeometryPtr MeshPtr = GeometryPtr(new ArkGeometry11());

	TriangleIndices face;


	for(int i=0; i < pPos.size(); ++i)
	{

		face.P1() = i*3 + 0;
		face.P2() = i*3 + 1;
		face.P3() = i*3 + 2;

		pPos3[i].x = pPos[i].x;
		pPos3[i].y = pPos[i].y;
		pPos3[i].z = pPos[i].z;

		pNormal3[i] = pNrm[i];
		pTex[i] = PTex[i];

		MeshPtr->AddFace(face);
	}

	MeshPtr->AddElement(pPositions);
	MeshPtr->AddElement(pNormals);
	MeshPtr->AddElement(pTexcoords);



	return MeshPtr;
}
GeometryPtr ArkGeometryLoader11::loadMS3DFile2(std::wstring filename)
{
	// Get the file path to the models
	ArkFileSystem fs;
	filename = fs.GetModelsDirectory() + filename;

	// Temporary Milkshape structures
	unsigned short usVertexCount = 0;
	unsigned short usTriangleCount = 0;
	unsigned short usGroupCount = 0;
	unsigned short usMaterialCount = 0;
	MS3DVertex* pMS3DVertices = NULL;
	MS3DTriangle* pMS3DTriangles = NULL;
	MS3DGroup* pMS3DGroups = NULL;
	MS3DMaterial* pMS3DMaterials = NULL;

	std::ifstream fin;
	MS3DHeader header;

	// Open the file and read the MS3D header data
	fin.open(filename.c_str(),std::ios::binary);
	fin.read((char*)(&(header.id)),sizeof(header.id));
	fin.read((char*)(&(header.version)),sizeof(header.version));
	if(header.version!=3 && header.version!=4)
		return NULL;

	// Load all the vertices
	fin.read((char*)(&usVertexCount),sizeof(unsigned short));
	pMS3DVertices = new MS3DVertex[usVertexCount];
	for(int i = 0; i < usVertexCount; i++)
	{
		fin.read((char*)&(pMS3DVertices[i].flags),sizeof(unsigned char));
		fin.read((char*)&(pMS3DVertices[i].vertex[0]),sizeof(float));
		fin.read((char*)&(pMS3DVertices[i].vertex[1]),sizeof(float));
		fin.read((char*)&(pMS3DVertices[i].vertex[2]),sizeof(float));
		fin.read((char*)&(pMS3DVertices[i].boneId),sizeof(char));
		fin.read((char*)&(pMS3DVertices[i].referenceCount),sizeof(unsigned char));
	}

	// Load all the triangle indices
	fin.read((char*)(&usTriangleCount),sizeof(unsigned short));
	pMS3DTriangles = new MS3DTriangle[usTriangleCount];
	for(int i = 0; i < usTriangleCount; i++)
	{
		fin.read((char*)&(pMS3DTriangles[i].flags),sizeof(unsigned short));
		fin.read((char*)&(pMS3DTriangles[i].vertexIndices[0]),sizeof(unsigned short)); //3*sizeof(unsigned short));
		fin.read((char*)&(pMS3DTriangles[i].vertexIndices[1]),sizeof(unsigned short)); //3*sizeof(unsigned short));
		fin.read((char*)&(pMS3DTriangles[i].vertexIndices[2]),sizeof(unsigned short)); //3*sizeof(unsigned short));
		fin.read((char*)&(pMS3DTriangles[i].vertexNormals[0]),3*sizeof(float));
		fin.read((char*)&(pMS3DTriangles[i].vertexNormals[1]),3*sizeof(float));
		fin.read((char*)&(pMS3DTriangles[i].vertexNormals[2]),3*sizeof(float));
		fin.read((char*)&(pMS3DTriangles[i].s),3*sizeof(float));
		fin.read((char*)&(pMS3DTriangles[i].t),3*sizeof(float));
		fin.read((char*)&(pMS3DTriangles[i].smoothingGroup),sizeof(unsigned char));
		fin.read((char*)&(pMS3DTriangles[i].groupIndex),sizeof(unsigned char));
	}

	// Load all the group information
	fin.read((char*)(&usGroupCount),sizeof(unsigned short));
	pMS3DGroups = new MS3DGroup[usGroupCount];
	for(int i = 0; i < usGroupCount; i++)
	{
		fin.read((char*)&(pMS3DGroups[i].flags),sizeof(unsigned char));
		fin.read((char*)&(pMS3DGroups[i].name),sizeof(char[32]));
		fin.read((char*)&(pMS3DGroups[i].numtriangles),sizeof(unsigned short));
		unsigned short triCount = pMS3DGroups[i].numtriangles;
		pMS3DGroups[i].triangleIndices = new unsigned short[triCount];
		fin.read((char*)(pMS3DGroups[i].triangleIndices),sizeof(unsigned short) * triCount);
		fin.read((char*)&(pMS3DGroups[i].materialIndex),sizeof(char));
	}

	// Load all the material information
	fin.read((char*)(&usMaterialCount),sizeof(unsigned short));
	pMS3DMaterials = new MS3DMaterial[usMaterialCount];
	for(int i = 0; i < usMaterialCount; i++)
	{
		fin.read((char*)&(pMS3DMaterials[i].name),sizeof(char[32]));
		fin.read((char*)&(pMS3DMaterials[i].ambient),4 * sizeof(float));
		fin.read((char*)&(pMS3DMaterials[i].diffuse),4 * sizeof(float));
		fin.read((char*)&(pMS3DMaterials[i].specular),4 * sizeof(float));
		fin.read((char*)&(pMS3DMaterials[i].emissive),4 * sizeof(float));
		fin.read((char*)&(pMS3DMaterials[i].shininess),sizeof(float));
		fin.read((char*)&(pMS3DMaterials[i].transparency),sizeof(float));
		fin.read((char*)&(pMS3DMaterials[i].mode),sizeof(char));
		fin.read((char*)&(pMS3DMaterials[i].texture),sizeof(char[128]));
		fin.read((char*)&(pMS3DMaterials[i].alphamap),sizeof(char[128]));
	}

	// Close the file (remaining file data unused)
	fin.close();


	// create the vertex element streams
	VertexElement11* pPositions = new VertexElement11(3,usTriangleCount*3);
	pPositions->m_SemanticName = VertexElement11::PositionSemantic;
	pPositions->m_uiSemanticIndex = 0;
	pPositions->m_Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pPositions->m_uiInputSlot = 0;
	pPositions->m_uiAlignedByteOffset = 0;
	pPositions->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pPositions->m_uiInstanceDataStepRate = 0;

	VertexElement11* pTexcoords = new VertexElement11(2,usTriangleCount*3);
	pTexcoords->m_SemanticName = VertexElement11::TexCoordSemantic;
	pTexcoords->m_uiSemanticIndex = 0;
	pTexcoords->m_Format = DXGI_FORMAT_R32G32_FLOAT;
	pTexcoords->m_uiInputSlot = 0;
	pTexcoords->m_uiAlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pTexcoords->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pTexcoords->m_uiInstanceDataStepRate = 0;

	VertexElement11* pNormals = new VertexElement11(3,usTriangleCount*3);
	pNormals->m_SemanticName = VertexElement11::NormalSemantic;
	pNormals->m_uiSemanticIndex = 0;
	pNormals->m_Format = DXGI_FORMAT_R32G32B32_FLOAT;
	pNormals->m_uiInputSlot = 0;
	pNormals->m_uiAlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	pNormals->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	pNormals->m_uiInstanceDataStepRate = 0;

	XMFLOAT3* pPos = (XMFLOAT3*)((*pPositions)[0]);
	XMFLOAT3* pNrm = (XMFLOAT3*)((*pNormals)[0]);
	XMFLOAT2* pTex = (XMFLOAT2*)((*pTexcoords)[0]);

	GeometryPtr MeshPtr = GeometryPtr(new ArkGeometry11());

	//for ( int i = 0; i < usVertexCount; i++ )
	//{
	//	pPos[i].x() = pMS3DVertices[i].vertex[0];
	//	pPos[i].y() = pMS3DVertices[i].vertex[1];
	//	pPos[i].z() = -pMS3DVertices[i].vertex[2];
	//	pNrm[i].MakeZero();
	//}

	TriangleIndices face;

	for(int i = 0; i < usTriangleCount; i++)
	{

		face.P1() = 3*i+0;
		face.P2() = 3*i+2;
		face.P3() = 3*i+1;

		pPos[3*i+0].x = pMS3DVertices[pMS3DTriangles[i].vertexIndices[0]].vertex[0];
		pPos[3*i+0].y = pMS3DVertices[pMS3DTriangles[i].vertexIndices[0]].vertex[1];
		pPos[3*i+0].z = -pMS3DVertices[pMS3DTriangles[i].vertexIndices[0]].vertex[2];
		pPos[3*i+1].x = pMS3DVertices[pMS3DTriangles[i].vertexIndices[1]].vertex[0];
		pPos[3*i+1].y = pMS3DVertices[pMS3DTriangles[i].vertexIndices[1]].vertex[1];
		pPos[3*i+1].z = -pMS3DVertices[pMS3DTriangles[i].vertexIndices[1]].vertex[2];
		pPos[3*i+2].x = pMS3DVertices[pMS3DTriangles[i].vertexIndices[2]].vertex[0];
		pPos[3*i+2].y = pMS3DVertices[pMS3DTriangles[i].vertexIndices[2]].vertex[1];
		pPos[3*i+2].z = -pMS3DVertices[pMS3DTriangles[i].vertexIndices[2]].vertex[2];

		pNrm[3*i+0].x = pMS3DTriangles[i].vertexNormals[0][0];
		pNrm[3*i+0].y = pMS3DTriangles[i].vertexNormals[0][1];
		pNrm[3*i+0].z = -pMS3DTriangles[i].vertexNormals[0][2];
		pNrm[3*i+1].x = pMS3DTriangles[i].vertexNormals[1][0];
		pNrm[3*i+1].y = pMS3DTriangles[i].vertexNormals[1][1];
		pNrm[3*i+1].z = -pMS3DTriangles[i].vertexNormals[1][2];
		pNrm[3*i+2].x = pMS3DTriangles[i].vertexNormals[2][0];
		pNrm[3*i+2].y = pMS3DTriangles[i].vertexNormals[2][1];
		pNrm[3*i+2].z = -pMS3DTriangles[i].vertexNormals[2][2];

		pTex[3*i+0].x = pMS3DTriangles[i].s[0];
		pTex[3*i+0].y = pMS3DTriangles[i].t[0];
		pTex[3*i+1].x = pMS3DTriangles[i].s[1];
		pTex[3*i+1].y = pMS3DTriangles[i].t[1];
		pTex[3*i+2].x = pMS3DTriangles[i].s[2];
		pTex[3*i+2].y = pMS3DTriangles[i].t[2];

		MeshPtr->AddFace(face);
	}

	for(int i = 0; i < usVertexCount; i++)
	{
		XMVECTOR v = XMLoadFloat3(&pNrm[i]);
		XMStoreFloat3(&pNrm[i],v);
		v = XMVector3Normalize(v);
		XMStoreFloat3(&pNrm[i],v);
	}

	MeshPtr->AddElement(pPositions);
	MeshPtr->AddElement(pTexcoords);
	MeshPtr->AddElement(pNormals);

	// Delete temporary materials
	if(pMS3DMaterials != NULL)
	{
		delete[] pMS3DMaterials;
		pMS3DMaterials = NULL;
	}

	// Delete temporary groups and their indices
	if(pMS3DGroups != NULL)
	{
		for(int i = 0; i < usGroupCount; i++)
		{
			if(pMS3DGroups[i].triangleIndices != NULL)
			{
				delete[] pMS3DGroups[i].triangleIndices;
				pMS3DGroups[i].triangleIndices = NULL;
			}
		}
		delete[] pMS3DGroups;
		pMS3DGroups = NULL;
	}

	// Delete temporary triangles
	if(pMS3DTriangles != NULL)
	{
		delete[] pMS3DTriangles;
		pMS3DTriangles = NULL;
	}

	// Delete temporary vertices
	if(pMS3DVertices != NULL)
	{
		delete[] pMS3DVertices;
		pMS3DVertices = NULL;
	}

	//MeshPtr->GenerateVertexDeclaration();
	//MeshPtr->LoadToBuffers();

	return(MeshPtr);
}
GeometryPtr ArkGeometryLoader11::loadStanfordPlyFile(std::wstring filename,bool withAdjacency)
{
	// Get the file path to the models
	ArkFileSystem fs;
	filename = fs.GetModelsDirectory() + filename;

	// Load the contents of the file
	std::ifstream fin;

	// Open the file and read the MS3D header data
	fin.open(filename.c_str(),std::ios::in);

	if(!fin.is_open())
	{
		// signal error - bad filename?
		throw new std::exception("Could not open file");
	}

	// Parse the input
	std::string txt;

	// Read in header
	std::getline(fin,txt);

	if(0 != txt.compare("ply"))
	{
		// signal error - not a PLY format file
		throw new std::exception("File does not contain the correct header - 'PLY' expected.");
	}

	std::getline(fin,txt);

	if(0 != txt.compare("format ascii 1.0"))
	{
		// signal error - not a format of PLY that this code supports
		throw new std::exception("File is not correct format - ASCII 1.0 expected.");
	}

	std::vector< PlyElementDesc > elements;

	// Read in the rest of the header
	while(fin.is_open() && !fin.eof())
	{
		// Grab the next line of the header
		std::getline(fin,txt);

		// If we're at the end then stop processing
		if(0 == txt.compare("end_header"))
		{
			break;
		}
		// If this line is a comment, skip to the next line
		else if(0 == txt.compare(0,7,"comment"))
		{
			continue;
		}
		// If this line is an element, process it
		else if(0 == txt.compare(0,7,"element"))
		{
			elements.push_back(ParsePLYElementHeader(txt,fin));
		}
		// Otherwise, wtf?
		else
		{
			throw new std::exception("File header contains unexpected line beginning");
		}
	}

	// Read all the raw data
	for(std::vector< PlyElementDesc >::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		(*it).data = ReadPLYElementData(fin,*it);
	}

	// Create a resource to contain the geometry
	GeometryPtr MeshPtr = GeometryPtr(new ArkGeometry11());

	// Convert data to D3D11 format
	int elemIdx = -1;

	// Pull out all the vertex data
	if(-1 < (elemIdx = FindPlyElementIndex(elements,"vertex")))
	{
		PlyElementDesc d = elements.at(elemIdx);

		// Has positions?
		int xIdx = FindPlyElementPropertyIndex(d.dataFormat,"x");
		int yIdx = FindPlyElementPropertyIndex(d.dataFormat,"y");
		int zIdx = FindPlyElementPropertyIndex(d.dataFormat,"z");

		if((-1 != xIdx) && (-1 != yIdx) && (-1 != zIdx))
		{
			VertexElement11 *pPositions = new VertexElement11(3,d.elementCount);
			pPositions->m_SemanticName = VertexElement11::PositionSemantic;
			pPositions->m_uiSemanticIndex = 0;
			pPositions->m_Format = DXGI_FORMAT_R32G32B32_FLOAT;
			pPositions->m_uiInputSlot = 0;
			pPositions->m_uiAlignedByteOffset = 0;
			pPositions->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			pPositions->m_uiInstanceDataStepRate = 0;

			XMFLOAT3* pRawPos = pPositions->Get3f(0);

			for(int v = 0; v < d.elementCount; ++v)
			{
				void** raw = d.data.at(v);

				float x = *reinterpret_cast<float*>(raw[xIdx]);
				float y = *reinterpret_cast<float*>(raw[yIdx]);
				float z = *reinterpret_cast<float*>(raw[zIdx]);

				pRawPos[v] = XMFLOAT3(x,y,z);
			}

			MeshPtr->AddElement(pPositions);
		}

		// Has normals?
		int nxIdx = FindPlyElementPropertyIndex(d.dataFormat,"nx");
		int nyIdx = FindPlyElementPropertyIndex(d.dataFormat,"ny");
		int nzIdx = FindPlyElementPropertyIndex(d.dataFormat,"nz");

		if((-1 != nxIdx) && (-1 != nyIdx) && (-1 != nzIdx))
		{
			VertexElement11 *pNormals = new VertexElement11(3,d.elementCount);
			pNormals->m_SemanticName = VertexElement11::NormalSemantic;
			pNormals->m_uiSemanticIndex = 0;
			pNormals->m_Format = DXGI_FORMAT_R32G32B32_FLOAT;
			pNormals->m_uiInputSlot = 0;
			pNormals->m_uiAlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			pNormals->m_InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			pNormals->m_uiInstanceDataStepRate = 0;

			XMFLOAT3* pRawNorms = pNormals->Get3f(0);

			for(int v = 0; v < d.elementCount; ++v)
			{
				void** raw = d.data.at(v);

				float x = *reinterpret_cast<float*>(raw[nxIdx]);
				float y = *reinterpret_cast<float*>(raw[nyIdx]);
				float z = *reinterpret_cast<float*>(raw[nzIdx]);

				pRawNorms[v] = XMFLOAT3(x,y,z);
			}

			MeshPtr->AddElement(pNormals);
		}
	}
	else
	{
		throw new std::exception("Expected a 'vertex' element, but not found");
	}

	// Pull out all the face index data
	if(-1 < (elemIdx = FindPlyElementIndex(elements,"face")))
	{
		PlyElementDesc d = elements.at(elemIdx);

		// Firstly, assert that the format is correct
		if((1 != d.dataFormat.size()) && d.dataFormat.at(0).isList && (0 == d.dataFormat.at(0).type.compare("uint")))
		{
			// Expect a single list of integers
			throw new std::exception("Expected 'face' to be a single list of integers per-face");
		}

		// Secondly, assert that each list is of the same dimension
		int faceSize = -1;
		for(int f = 0; f < d.elementCount; ++f)
		{
			void** raw = d.data.at(f);
			PlyDataArray<int>* idxs = reinterpret_cast<PlyDataArray<int>*>(raw[0]);

			if(-1 == faceSize)
				faceSize = idxs->length;
			else if(faceSize != idxs->length)
				throw new std::exception("Expected each face to have the same number of indexes");
		}

		if(withAdjacency)
		{
			MeshPtr->SetPrimitiveType((D3D11_PRIMITIVE_TOPOLOGY)(D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST + ((2*faceSize) - 1)));

			// Grab all of the faces so we can search for adjacency
			int* pRaw = new int[d.elementCount * faceSize];

			int pRawIdx = 0;

			for(int f = 0; f < d.elementCount; ++f)
			{
				void** raw = d.data.at(f);
				PlyDataArray<int>* idxs = reinterpret_cast<PlyDataArray<int>*>(raw[0]);

				for(unsigned int fi = 0; fi < idxs->length; ++fi)
					pRaw[pRawIdx++] = idxs->data[fi];
			}

			// We can now go and add the actual indices
			for(int f = 0; f < (d.elementCount * faceSize); f+=3)
			{
				MeshPtr->AddIndex(pRaw[f + 0]);
				MeshPtr->AddIndex(pRaw[f + 1]);
				MeshPtr->AddIndex(pRaw[f + 2]);

				// We now need to find an adjacency for each
				// edge where possible
				int a0 = FindAdjacentIndex(pRaw[f + 0],pRaw[f + 1],pRaw[f + 2],pRaw,d.elementCount * faceSize);
				int a1 = FindAdjacentIndex(pRaw[f + 1],pRaw[f + 2],pRaw[f + 0],pRaw,d.elementCount * faceSize);
				int a2 = FindAdjacentIndex(pRaw[f + 2],pRaw[f + 0],pRaw[f + 1],pRaw,d.elementCount * faceSize);

				std::wstringstream out;
				out << "Actual indices <" << pRaw[f+0] << ", " << pRaw[f+1] << ", " << pRaw[f+2] << "> have adjacency <" << a0 << ", " << a1 << ", " << a2 << ">.";
				OutputDebugString(out.str().c_str());
				OutputDebugString(L"\n");

				MeshPtr->AddIndex(a0);
				MeshPtr->AddIndex(a1);
				MeshPtr->AddIndex(a2);
			}

			delete[] pRaw;
		}
		else
		{
			// Thirdly, can now set the appropriate topology
			MeshPtr->SetPrimitiveType((D3D11_PRIMITIVE_TOPOLOGY)(D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST + (faceSize - 1)));

			// Finally, extract this data
			for(int f = 0; f < d.elementCount; ++f)
			{
				void** raw = d.data.at(f);
				PlyDataArray<int>* idxs = reinterpret_cast<PlyDataArray<int>*>(raw[0]);

				for(unsigned int fi = 0; fi < idxs->length; ++fi)
					MeshPtr->AddIndex(idxs->data[fi]);
			}
		}
	}
	else
	{
		throw new std::exception("Expected a 'face' element, but not found");
	}

	// Push into renderable resource
	MeshPtr->LoadToBuffers();

	// Release all intermediary memory
	for(std::vector< PlyElementDesc >::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		PlyElementDesc d = *it;
		for(int e = 0; e < d.elementCount; ++e)
		{
			void** raw = d.data.at(e);

			if(d.dataFormat.at(0).isList)
			{
				PlyDataArray<void*>* rawArray = reinterpret_cast<PlyDataArray<void*>*>(raw[0]);
				SAFE_DELETE_ARRAY(rawArray->data);
				Safe_Delete(raw[0]);
			}
			else
			{
				Safe_Delete(raw[0]);
			}
		}
	}

	// Return to caller
	return MeshPtr;
}

int ArkGeometryLoader11::FindAdjacentIndex(int edgeStart,int edgeEnd,int triV,int* pRaw,int rawLen)
{
	for(int f = 0; f < rawLen; f+=3)
	{
		int esIdx = -1;
		for(int i = 0; i < 3; ++i) { if(pRaw[f+i]==edgeStart) esIdx = i; }

		int eeIdx = -1;
		for(int i = 0; i < 3; ++i) { if(pRaw[f+i]==edgeEnd) eeIdx = i; }

		if((-1!=esIdx)&&(-1!=eeIdx))
		{
			int oIdx = -1;
			for(int i = 0; i < 3; ++i) { if((i!=esIdx) && (i!=eeIdx)) oIdx = i; }

			if(pRaw[f+oIdx]!=triV)
				return pRaw[f + oIdx];
		}
	}

	return edgeStart;
}

ArkGeometryLoader11::PlyElementDesc ArkGeometryLoader11::ParsePLYElementHeader(std::string headerLine,std::ifstream& input)
{
	ArkGeometryLoader11::PlyElementDesc desc;
	std::string txt;

	// Parse the header line
	// "element <name> <count>"
	int split = headerLine.find_first_of(' ',8);
	desc.name = headerLine.substr(8,split - 8);

	split = headerLine.rfind(' ');
	std::istringstream elemCount(headerLine.substr(split,headerLine.length() - split));
	elemCount >> desc.elementCount;

	// Parse any attached properties
	while(input.is_open() && !input.eof())
	{
		std::getline(input,txt);

		if(0 == txt.compare(0,13,"property list"))
		{
			// Parse this property list declaration
			desc.dataFormat.push_back(ParsePLYElementPropertyList(txt.substr(14,txt.length() - 14)));
		}
		else if(0 == txt.compare(0,8,"property"))
		{
			// Parse this property declaration
			desc.dataFormat.push_back(ParsePLYElementProperty(txt.substr(9,txt.length() - 9)));
		}
		else
		{
			// At this point we'll also have read a line too far so
			// need to "unread" it to avoid breaking remaining parsing.
			input.putback('\n');
			for(int i = -1 + txt.length(); i >= 0; i--)
				input.putback(txt.at(i));
			// (there must be a better way, no?!?)
			break;
		}
	}

	return desc;
}

ArkGeometryLoader11::PlyElementPropertyDeclaration ArkGeometryLoader11::ParsePLYElementProperty(std::string desc)
{
	// <type> <name>
	ArkGeometryLoader11::PlyElementPropertyDeclaration decl;
	decl.isList = false;

	int split = desc.find(' ');

	decl.type = desc.substr(0,split);
	decl.name = desc.substr(split + 1,desc.length() - (split + 1));

	return decl;
}

ArkGeometryLoader11::PlyElementPropertyDeclaration ArkGeometryLoader11::ParsePLYElementPropertyList(std::string desc)
{
	// <length_type> <element_type> <name>
	ArkGeometryLoader11::PlyElementPropertyDeclaration decl;
	decl.isList = true;

	int split = desc.find(' ');
	decl.listLengthType = desc.substr(0,split);

	decl.type = desc.substr(split + 1,desc.rfind(' ') - (split + 1));

	split = desc.rfind(' ') + 1;
	decl.name = desc.substr(split,desc.length() - split);

	return decl;
}

std::vector<void**> ArkGeometryLoader11::ReadPLYElementData(std::ifstream& input,const ArkGeometryLoader11::PlyElementDesc& desc)
{
	std::vector<void**> raw;

	for(int i = 0; i < desc.elementCount; ++i)
	{
		std::string txt;
		std::getline(input,txt);

		raw.push_back(ParsePLYElementData(txt,desc.dataFormat));
	}

	return raw;
}

void** ArkGeometryLoader11::ParsePLYElementData(std::string text,const std::vector<PlyElementPropertyDeclaration>& desc)
{
	void** parsed;

	parsed = new void*[desc.size()];

	std::vector<std::string> tokens;
	std::istringstream is(text);
	std::string tmps;
	while(is.good()) {
		is >> tmps;
		tokens.push_back(tmps);
	}


	unsigned int e = 0;
	for(std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		if(e >= desc.size())
			break;

		if(desc[e].isList)
		{
			// Read this as length
			int arrLen = 0;

			arrLen = ExtractDataVal<int>(*it);

			// Size up the type
			if(0 == desc[e].type.compare("char"))
			{
				parsed[e] = ExtractDataPtrArray<char>(arrLen,it);
			}
			else if(0 == desc[e].type.compare("uchar"))
			{
				parsed[e] = ExtractDataPtrArray<unsigned char>(arrLen,it);
			}
			else if(0 == desc[e].type.compare("short"))
			{
				parsed[e] = ExtractDataPtrArray<short>(arrLen,it);
			}
			else if(0 == desc[e].type.compare("ushort"))
			{
				parsed[e] = ExtractDataPtrArray<unsigned short>(arrLen,it);
			}
			else if(0 == desc[e].type.compare("int"))
			{
				parsed[e] = ExtractDataPtrArray<int>(arrLen,it);
			}
			else if(0 == desc[e].type.compare("uint"))
			{
				parsed[e] = ExtractDataPtrArray<unsigned int>(arrLen,it);
			}
			else if(0 == desc[e].type.compare("float"))
			{
				parsed[e] = ExtractDataPtrArray<float>(arrLen,it);
			}
			else if(0 == desc[e].type.compare("double"))
			{
				parsed[e] = ExtractDataPtrArray<double>(arrLen,it);
			}
			else
			{
				// wtf?
			}
		}
		else
		{
			if(0 == desc[e].type.compare("char"))
			{
				parsed[e] = ExtractDataPtr<char>(*it);
			}
			else if(0 == desc[e].type.compare("uchar"))
			{
				parsed[e] = ExtractDataPtr<unsigned char>(*it);
			}
			else if(0 == desc[e].type.compare("short"))
			{
				parsed[e] = ExtractDataPtr<short>(*it);
			}
			else if(0 == desc[e].type.compare("ushort"))
			{
				parsed[e] = ExtractDataPtr<unsigned short>(*it);
			}
			else if(0 == desc[e].type.compare("int"))
			{
				parsed[e] = ExtractDataPtr<int>(*it);
			}
			else if(0 == desc[e].type.compare("uint"))
			{
				parsed[e] = ExtractDataPtr<unsigned int>(*it);
			}
			else if(0 == desc[e].type.compare("float"))
			{
				parsed[e] = ExtractDataPtr<float>(*it);
			}
			else if(0 == desc[e].type.compare("double"))
			{
				parsed[e] = ExtractDataPtr<double>(*it);
			}
			else
			{
				// wtf?
			}
		}

		++e;
	}

	return parsed;
}

template<typename T> T* ArkGeometryLoader11::ExtractDataPtr(std::string input)
{
	T* t = new T;

	std::istringstream iss(input);

	iss >> *t;

	return t;
}

template<typename T> T ArkGeometryLoader11::ExtractDataVal(std::string input)
{
	T t;

	std::istringstream iss(input);

	iss >> t;

	return t;
}

template<typename T> ArkGeometryLoader11::PlyDataArray<T>* ArkGeometryLoader11::ExtractDataPtrArray(int length,std::vector<std::string>::iterator iterator)
{
	PlyDataArray<T>* t = new PlyDataArray<T>;
	t->length = length;
	t->data = new T[length];

	for(int i = 0; i < length; ++i)
	{
		std::istringstream iss(*(++iterator));
		iss >> t->data[i];
	}

	return t;
}



int ArkGeometryLoader11::FindPlyElementIndex(std::vector<PlyElementDesc> elems,std::string name)
{
	for(unsigned int idx = 0; idx < elems.size(); ++idx)
		if(0 == elems.at(idx).name.compare(name))
			return idx;

	return -1;
}

int ArkGeometryLoader11::FindPlyElementPropertyIndex(std::vector<PlyElementPropertyDeclaration> elems,std::string name)
{
	for(unsigned int idx = 0; idx < elems.size(); ++idx)
		if(0 == elems.at(idx).name.compare(name))
			return idx;

	return -1;
}
