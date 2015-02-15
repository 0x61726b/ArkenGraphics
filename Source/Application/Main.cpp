//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Main.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkApplication.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE h_Inst,HINSTANCE h_PrevInst,LPSTR lpcmdline,int ncmdshow)
{
	HANDLE hLogFile;
	hLogFile = CreateFile(L"C:\\Users\\alperen\\Desktop\\ArkenGraphics\\__CRTLOG__.txt",GENERIC_WRITE,
		FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,NULL);

	_CrtSetReportFile(_CRT_ERROR,hLogFile);
	_CrtSetReportMode(_CRT_ERROR,_CRTDBG_MODE_DEBUG);
	
	ArkApplication* m_pApp = ArkApplication::Get();
	if(!m_pApp)
	{
		return -1;
	}

	if(!m_pApp->ConfigureEngineComponents())
	{
		m_pApp->ShutdownEngineComponents();
		return(false);
	}


	// Call the application initialize function - this function
	// will be overloaded specifically for each of the applications.

	m_pApp->Initialize();


	// Call the application message loop function - this function
	// could be overloaded specifically for each the applications.

	m_pApp->MessageLoop();


	// Call the overloaded application shutdown method.

	m_pApp->Shutdown();
	m_pApp->ShutdownEngineComponents();
	return true;
}
