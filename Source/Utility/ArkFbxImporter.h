//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//FbxImporter.h
//--------------------------------------------------------------------------------
#ifndef __FbxImporter_h__
#define __FbxImporter_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkSurfaceMaterial.h"
#include <fbxsdk.h>
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkFbxImporter
	{
		ArkFbxImporter(FbxManager* m);
		~ArkFbxImporter();

		bool LoadScene(const char* fileName);
		bool BuildMesh(FbxMesh* Mesh);

		bool GetMaterialElements(FbxSurfaceMaterial* Material,MaterialElements* OutSMaterial);
		bool GetTexture(FbxProperty& Property,std::string* outFileName);

		bool GetMaterial(FbxMesh* Mesh,MaterialElements* OutSMaterial,MaterialTextures* OutTextureNames);

		bool GetNormals(FbxLayer* Layer,int CtrlIndexDefault,int VertCount,std::vector<XMFLOAT4>& OutPoints);
		bool GetBinormals(FbxLayer* Layer,int CtrlIndexDefault,int VertCount,std::vector<XMFLOAT4>& OutPoints);
		bool GetTangents(FbxLayer* Layer,int CtrlIndexDefault,int VertCount,std::vector<XMFLOAT4>& OutPoints);
		bool GetVertexColors(FbxLayer* Layer,int CtrlIndexDefault,int VertCount,std::vector<XMFLOAT4>& OutPoints);

		bool GetUV(FbxLayer* Layer,FbxMesh* Mesh,int CtrlIndex,int PolygonIndex,int VertId,int PointId,std::vector<XMFLOAT2>& OutPoints);

		template <typename ElementType>
		bool ParseElements(ElementType e,int CtrlIndex,int VertexId,int* outIndex)
		{
			if(e == nullptr)
				return false;

			int index = -1;
			FbxLayerElement::EMappingMode mappingMode = e->GetMappingMode();
			FbxLayerElement::EReferenceMode refMode = e->GetReferenceMode();

			if(mappingMode == FbxLayerElement::eByControlPoint)
			{
				if(refMode == FbxLayerElement::eDirect)
					index = CtrlIndex;
				else if(refMode == FbxLayerElement::eIndexToDirect)
					index = e->GetIndexArray().GetAt(ctrlPointIdx);
			}
			else if(mappingMode == FbxLayerElement::eByPolygonVertex)
			{
				if(refMode == FbxLayerElement::eDirect)
					index = VertexId;
				else if(refMode == FbxLayerElement::eIndexToDirect)
					index = e->GetIndexArray().GetAt(vertexIdx);
			}

			*outIndex = index;

			return index != -1;
		}

	protected:
		FbxManager*				m_pManager;
		FbxScene*				m_pScene;
	};
};
//--------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------