//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//Pch.h
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
#ifndef __Pch_h__
#define __Pch_h__
//--------------------------------------------------------------------------------

//Standart C++ Libraries
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <sstream>
#include <algorithm>
#include <vector>
#include <exception>
//

//Standard C Library includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory>
#include <assert.h>
#include <ctime>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//

//Direct3D Includes
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

//Effects Library
#include "d3dx11effect.h"
#include <wrl.h>


#define COM_Release( x ) {if(x){(x)->Release();(x)=NULL;}}
#define Safe_Delete( x ) {if(x){delete (x);(x)=NULL;}}
#define HR_CHECK( x ) { if( FAILED(x) ) return false;}

#define NUM_THREADS 1


//--------------------------------------------------------------------------------
#endif //__Pch_h__



