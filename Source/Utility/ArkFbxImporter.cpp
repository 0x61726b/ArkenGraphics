//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//FbxImporter.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkFbxImporter.h"
#include "ArkFileSystem.h"
#include "ArkSurfaceMaterial.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
using namespace DirectX;
using namespace fbxsdk_2015_1;
//--------------------------------------------------------------------------------
ArkFbxImporter::ArkFbxImporter(FbxManager* Manager)
{
	if(Manager != NULL)
		m_pManager = Manager;
}
//--------------------------------------------------------------------------------
ArkFbxImporter::~ArkFbxImporter()
{
	if(m_pScene)
	{
		m_pScene->Destroy();
		m_pScene = NULL;
	}
}
//--------------------------------------------------------------------------------
bool ArkFbxImporter::LoadScene(const char* fileName)
{
	int sdkMajor,sdkMinor,sdkRevision;

	FbxManager::GetFileFormatVersion(sdkMajor,sdkMinor,sdkRevision);

	FbxImporter* Importer = FbxImporter::Create(m_pManager,"");

	FbxIOSettings *ioSetting = m_pManager->GetIOSettings();
	ioSetting->SetBoolProp(IMP_FBX_MATERIAL,true);
	ioSetting->SetBoolProp(IMP_FBX_TEXTURE,true);
	ioSetting->SetBoolProp(IMP_FBX_LINK,true);
	ioSetting->SetBoolProp(IMP_FBX_SHAPE,true);
	ioSetting->SetBoolProp(IMP_FBX_GOBO,true);
	ioSetting->SetBoolProp(IMP_FBX_ANIMATION,true);
	ioSetting->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS,true);

	ArkFileSystem Fs;
	std::wstring WModelsPath = Fs.GetModelsDirectory();
	std::string FilePath = fileName;
	std::string ModelPath(WModelsPath.begin(),WModelsPath.end());
	FilePath = ModelPath + FilePath;

	bool ImporterStatus = Importer->Initialize(FilePath.c_str(),-1,m_pManager->GetIOSettings());

	if(!ImporterStatus)
	{
		FbxString error = Importer->GetStatus().GetErrorString();
	}

	int fileMajor,fileMinor,fileRevision;
	Importer->GetFileVersion(fileMajor,fileMinor,fileRevision);

	bool Status = Importer->Import(m_pScene);

	if(!Status)
	{

	}

	Importer->Destroy();

	FbxAxisSystem axis = FbxAxisSystem::DirectX;
	axis.ConvertScene(m_pScene);

	return Status;
}
//--------------------------------------------------------------------------------
bool ArkFbxImporter::GetMaterial(FbxMesh* Mesh,MaterialElements* OutSMaterial,MaterialTextures* OutTextureNames)
{
	FbxNode* Node = Mesh->GetNode();

	int MaterialCount = Node->GetMaterialCount();
	for(int MaterialIdx = 0; MaterialIdx < MaterialCount; MaterialIdx++)
	{
		FbxSurfaceMaterial* FbxMaterial = Node->GetMaterial(MaterialIdx);

		if(FbxMaterial == nullptr)
			return false;

		FbxProperty FbxProperty;

		FbxProperty = FbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
		GetTexture(FbxProperty,&OutTextureNames->Diffuse);

		FbxProperty = FbxMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);
		GetTexture(FbxProperty,&OutTextureNames->Emissive);

		FbxProperty = FbxMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
		GetTexture(FbxProperty,&OutTextureNames->Specular);

		FbxProperty = FbxMaterial->FindProperty(FbxSurfaceMaterial::sSpecularFactor);
		GetTexture(FbxProperty,&OutTextureNames->SpecularFactor);

		FbxProperty = FbxMaterial->FindProperty(FbxSurfaceMaterial::sBump);
		GetTexture(FbxProperty,&OutTextureNames->Bump);

		FbxProperty = FbxMaterial->FindProperty(FbxSurfaceMaterial::sBumpFactor);
		GetTexture(FbxProperty,&OutTextureNames->BumpFactor);

		FbxProperty = FbxMaterial->FindProperty(FbxSurfaceMaterial::sTransparentColor);
		GetTexture(FbxProperty,&OutTextureNames->Transparency);
	}
}
bool ArkFbxImporter::GetNormals(FbxLayer* Layer,int CtrlIndexDefault,int VertCount,std::vector<XMFLOAT4>& OutPoints)
{
	int index = -1;
	bool res = ParseElements(Layer->GetBinormals(),CtrlIndexDefault,VertCount,&index);

	if(res)
	{
		FbxLayerElementNormal* FbxNormals = Layer->GetNormals();
		FbxVector4 v = FbxNormals->GetDirectArray().GetAt(index);
		v.Normalize();
		(OutPoints).push_back(XMFLOAT4((float)v[0],(float)v[1],(float)v[2],(float)v[3]));
	}
	return res;
}
//--------------------------------------------------------------------------------
bool ArkFbxImporter::GetUV(FbxLayer* Layer,FbxMesh* Mesh,int CtrlIndex,int PolygonIndex,int VertId,int PointId,std::vector<XMFLOAT2>& OutPoints)
{
	FbxLayerElementUV *fbxUV = Layer->GetUVs();

	if(fbxUV == nullptr)
		return false;

	int index = -1;

	{
		FbxLayerElement::EMappingMode mappingMode = fbxUV->GetMappingMode();
		FbxLayerElement::EReferenceMode refMode = fbxUV->GetReferenceMode();

		if(mappingMode == FbxLayerElement::eByControlPoint)
		{
			if(refMode == FbxLayerElement::eDirect)
				index = CtrlIndex;
			else if(refMode == FbxLayerElement::eIndexToDirect)
				index = fbxUV->GetIndexArray().GetAt(CtrlIndex);
		}
		else if(mappingMode == FbxLayerElement::eByPolygonVertex)
		{
			if(refMode == FbxLayerElement::eDirect)
				index = VertId;
			else if(refMode == FbxLayerElement::eIndexToDirect)
				index = Mesh->GetTextureUVIndex(PolygonIndex,PointId);
		}
	}

	if(index != -1)
	{
		FbxVector2 v = fbxUV->GetDirectArray().GetAt(index);
		OutPoints.push_back(XMFLOAT2((float)v[0],1.0f - (float)v[1]));
	}

	return index != -1;
}
bool ArkFbxImporter::GetVertexColors(FbxLayer* Layer,int CtrlIndexDefault,int VertCount,std::vector<XMFLOAT4>& OutPoints)
{
	int index = -1;
	bool res = ParseElements(Layer->GetBinormals(),CtrlIndexDefault,VertCount,&index);

	if(res)
	{
		FbxLayerElementVertexColor* FbxVertexColor = Layer->GetVertexColors();
		FbxColor v = FbxVertexColor->GetDirectArray().GetAt(index);
		(OutPoints).push_back(XMFLOAT4(v.mRed,v.mGreen,v.mBlue,v.mAlpha));
	}
	return res;
}
//--------------------------------------------------------------------------------
bool ArkFbxImporter::GetTangents(FbxLayer* Layer,int CtrlIndexDefault,int VertCount,std::vector<XMFLOAT4>& OutPoints)
{
	int index = -1;
	bool res = ParseElements(Layer->GetBinormals(),CtrlIndexDefault,VertCount,&index);

	if(res)
	{
		FbxLayerElementTangent* FbxTangent = Layer->GetTangents();
		FbxVector4 v = FbxTangent->GetDirectArray().GetAt(index);
		(OutPoints).push_back(XMFLOAT4((float)v[0],(float)v[1],(float)v[2],(float)v[3]));
	}
	return res;
}
//--------------------------------------------------------------------------------
bool ArkFbxImporter::GetBinormals(FbxLayer* Layer,int CtrlIndexDefault,int VertCount,std::vector<XMFLOAT4>& OutPoints)
{
	int index = -1;
	bool res = ParseElements(Layer->GetBinormals(),CtrlIndexDefault,VertCount,&index);

	if(res)
	{
		FbxLayerElementBinormal* FbxBinormal = Layer->GetBinormals();
		FbxVector4 v = FbxBinormal->GetDirectArray().GetAt(index);
		(OutPoints).push_back(XMFLOAT4((float)v[0],(float)v[1],(float)v[2],(float)v[3]));
	}
	return res;
}
//--------------------------------------------------------------------------------
bool ArkFbxImporter::GetTexture(FbxProperty& Property,std::string* outFileName)
{
	if(!Property.IsValid())
		return false;
	if(Property.GetSrcObjectCount<FbxSurfaceMaterial>() == 0)
		return false;

	int TextureCount = Property.GetSrcObjectCount<FbxSurfaceMaterial>();

	if(TextureCount == 1)
	{
		FbxFileTexture *texture = FbxCast<FbxFileTexture>(Property.GetSrcObject<FbxSurfaceMaterial>());
		const char *textureFileName = texture->GetFileName();
		*outFileName = textureFileName;
	}
	return true;
}
//--------------------------------------------------------------------------------
bool ArkFbxImporter::GetMaterialElements(FbxSurfaceMaterial* Material,MaterialElements* OutSMaterial)
{
	FbxClassId Id = Material->GetClassId();

	std::pair<FbxDouble3,double> Ambient,Diffuse,Emissive;
	double Transparency = 1.0f;
	double Shininess = 0.0f;


	if(Id.Is(FbxSurfacePhong::ClassId))
	{
		FbxSurfacePhong *phong = static_cast<FbxSurfacePhong*>(Material);

		FbxDouble3 specular;
		double specularFactor;

		Ambient.first = phong->Ambient.Get();
		Ambient.second = phong->AmbientFactor;

		Diffuse.first = phong->Diffuse.Get();
		Diffuse.second = phong->DiffuseFactor;

		specular = phong->Specular.Get();
		specularFactor = phong->SpecularFactor;

		Emissive.first = phong->Emissive.Get();
		Emissive.second = phong->EmissiveFactor;

		Transparency = phong->TransparencyFactor;

		OutSMaterial->Shininess = phong->Shininess.Get();
		OutSMaterial->Specular = XMFLOAT4(specular[0] * specularFactor,specular[1] * specularFactor,specular[2] * specularFactor,0.0f);
	}
	else if(Id.Is(FbxSurfaceLambert::ClassId))
	{
		FbxSurfaceLambert *lambert = static_cast<FbxSurfaceLambert*>(Material);

		Ambient.first = lambert->Ambient.Get();
		Ambient.second = lambert->AmbientFactor;

		Diffuse.first = lambert->Diffuse.Get();
		Diffuse.second = lambert->DiffuseFactor;

		Emissive.first = lambert->Emissive.Get();
		Emissive.second = lambert->EmissiveFactor;

		Transparency = lambert->TransparencyFactor;
	}
	else return false;

	OutSMaterial->Ambient = XMFLOAT4(Ambient.first[0] * Ambient.second,Ambient.first[1] * Ambient.second,Ambient.first[2] * Ambient.second,0.0f);
	OutSMaterial->Diffuse = XMFLOAT4(Diffuse.first[0] * Diffuse.second,Diffuse.first[1] * Diffuse.second,Diffuse.first[2] * Diffuse.second,0.0f);
	OutSMaterial->Emissive = XMFLOAT4(Emissive.first[0] * Emissive.second,Emissive.first[1] * Emissive.second,Emissive.first[2] * Emissive.second,0.0f);
	OutSMaterial->Transparency = Transparency;
}
//--------------------------------------------------------------------------------

