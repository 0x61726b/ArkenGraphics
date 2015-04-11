//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Win32RenderWindow.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "Win32RenderWindow.h"
#include "ArkConfigFile.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
extern LRESULT CALLBACK WindowProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );
//--------------------------------------------------------------------------------
Win32RenderWindow::Win32RenderWindow( )
{
	m_hWnd = 0;

	// Specify the window style
	m_dStyle = (WS_OVERLAPPEDWINDOW | WS_VISIBLE);
}
//--------------------------------------------------------------------------------
Win32RenderWindow::~Win32RenderWindow()
{
	Shutdown();
}
//--------------------------------------------------------------------------------
LRESULT CALLBACK InternalWindowProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	LONG_PTR ObjPtr = GetWindowLongPtr(hwnd, 0);

	if (ObjPtr == 0) 
	{
        return( DefWindowProc( hwnd, msg, wparam, lparam ) );
	} 
	else
	{
	    return( ((IWndProc*)ObjPtr)->WindowProc(hwnd, msg, wparam, lparam) );
	}
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
void Win32RenderWindow::Initialize(IWndProc* WindowProcObj)
{
	WNDCLASSEX wc;
	
	// Setup the window class
	memset(&wc,0, sizeof(wc));
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= InternalWindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= sizeof(WindowProcObj);
	wc.hInstance		= 0;
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW); 
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= L"Arkeng32";
	wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);
	WindowProcObj->BeforeRegisterWindowClass(wc);
	
	// Register the window class
	RegisterClassEx(&wc);

	// Record the desired device size
	RECT rc;
	rc.top = rc.left = 0;
	rc.right = m_iWidth;
	rc.bottom = m_iHeight;
	
	// Adjust the window size for correct device size
	AdjustWindowRectEx(&rc, m_dStyle, false, 0);

	long lwidth = rc.right - rc.left;
	long lheight = rc.bottom - rc.top;

	long lleft = (long)m_iLeft;	
	long ltop = (long)m_iTop;

	std::wstring log = L"Attempting to create a Win32 window W = ";
	log.append(std::to_wstring(m_iWidth));
	log.append(L" H = ");
	log.append(std::to_wstring(m_iHeight));

	

	ArkLog::Get( Arkeng::LogType::Renderer ).Output(log);
	// Create an instance of the window
	m_hWnd = CreateWindowEx(
		NULL,							// extended style
		wc.lpszClassName, 				// class name
		ArkConfigFile::Get().GetConfig().BuildNumber.c_str(),				// instance title
		m_dStyle,						// window style
		lleft, ltop,					// initial x, y
		lwidth,							// initial width
		lheight,						// initial height
		NULL,							// handle to parent 
		NULL,							// handle to menu
		NULL,							// instance of this application
		NULL );							// extra creation parms
	ArkLog::Get(LogType::Renderer).Output(L"Success!");

	// Update the size of the window according to the client area that was 
	// created.  Due to limitations about the desktop size, this can cause some
	// situations where the created window is smaller than requested.

	RECT rect;
	GetClientRect( m_hWnd, &rect );	
	m_iWidth = rect.right - rect.left;
	m_iHeight = rect.bottom - rect.top;


	if (m_hWnd) 
	{
		// Set in the "extra" bytes the pointer to the IWindowProc object
		// which handles messages for the window
        SetWindowLongPtr(m_hWnd, 0, (LONG_PTR)WindowProcObj);
    	// Initially display the window
		ShowWindow( m_hWnd, SW_SHOWNORMAL );
		UpdateWindow( m_hWnd );
	}
	
}
//--------------------------------------------------------------------------------
void Win32RenderWindow::Shutdown()
{
	if ( m_hWnd )
		DestroyWindow( m_hWnd );

	m_hWnd = 0;
}
//--------------------------------------------------------------------------------
void Win32RenderWindow::Paint()
{
	
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR Win32RenderWindow::GetCursorPosition()
{
	POINT p;
	GetCursorPos( &p );
	ScreenToClient( m_hWnd,&p );

	return DirectX::XMVectorSet( static_cast<float>(p.x),static_cast<float>( p.y ),0,1 );
}
//--------------------------------------------------------------------------------