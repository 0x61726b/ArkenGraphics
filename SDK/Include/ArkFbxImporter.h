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
#include <fbxsdk.h>
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkFbxImporter
	{
		ArkFbxImporter(FbxManager* m);
		~ArkFbxImporter();

		bool LoadScene(const char* fileName);

	protected:
		FbxManager*				m_pManager;
		FbxScene*				m_pScene;
	};
};
//--------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------