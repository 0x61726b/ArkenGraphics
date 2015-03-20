//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkShaderFactory11.h
//--------------------------------------------------------------------------------
#pragma warning( disable : 4244 )
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkShaderFactory11.h"
#include "ArkLog.h"
#include "ArkFileSystem.h"
#include "ArkFileLoader.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
ArkShaderFactory11::ArkShaderFactory11( )
{
}
//--------------------------------------------------------------------------------
ArkShaderFactory11::~ArkShaderFactory11()
{
}
//--------------------------------------------------------------------------------
ID3DBlob* ArkShaderFactory11::GenerateShader( ShaderType type, std::wstring& filename, std::wstring& function,
            std::wstring& model, const D3D_SHADER_MACRO* pDefines, bool enablelogging )
{
	HRESULT hr = S_OK;

	std::wstringstream message;

	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrorMessages = nullptr;

	char AsciiFunction[1024];
	char AsciiModel[1024];
	WideCharToMultiByte(CP_ACP, 0, function.c_str(), -1, AsciiFunction, 1024, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, model.c_str(), -1, AsciiModel, 1024, NULL, NULL);

	 UINT flags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#ifdef _DEBUG
    flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION; // | D3DCOMPILE_WARNINGS_ARE_ERRORS;
#endif


	ArkFileSystem fs;
	std::wstring shaderDir = fs.GetShadersDirectory() + filename;

	ArkFileLoader SourceFile;
	if( !SourceFile.Open(shaderDir) )
	{
		message << "Unable to load shader from file: " << shaderDir;
		return nullptr;
	}


	if ( FAILED( hr = D3DCompile( 
		SourceFile.GetDataPtr(),
		SourceFile.GetDataSize(),
		nullptr,
		pDefines,
		nullptr,
		AsciiFunction,
		AsciiModel,
		flags,
		0,
		&pCompiledShader,
		&pErrorMessages ) ) )
	{
		message << L"Error compiling shader program: " << shaderDir << std::endl << std::endl;
		message << L"The following error was reported:" << std::endl;

		if ( ( enablelogging ) && ( pErrorMessages != nullptr ) )
		{
			LPVOID pCompileErrors = pErrorMessages->GetBufferPointer();
			const char* pMessage = (const char*)pCompileErrors;
			message << std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes( std::string( pMessage ) );
			ArkLog::Get(LogType::FileSystem).Output( message.str() );
		}

		COM_Release( pCompiledShader );
		COM_Release( pErrorMessages );

		return nullptr;
	}

	COM_Release( pErrorMessages );

	return( pCompiledShader );
}
//--------------------------------------------------------------------------------
ID3DBlob* ArkShaderFactory11::GeneratePrecompiledShader( std::wstring& filename, std::wstring& function,
            std::wstring& model )
{
	std::wstringstream message;

	// Determine where to look for the shader file

	ArkFileSystem fs;
	std::wstring filepath = fs.GetShadersDirectory() + filename + L"_" + function + L"_" + model + L".cso";

	// Load the file into memory

	ArkFileLoader CompiledObjectFile;
	if ( !CompiledObjectFile.Open( filepath ) ) {
		message << "Unable to load shader from file: " << filepath;
		return( nullptr );
	}

	// Create a blob to store the object code in
	
	ID3DBlob* pBlob = nullptr;
	HRESULT hr = D3DCreateBlob( CompiledObjectFile.GetDataSize(), &pBlob );

	if ( FAILED( hr ) ) {
		message << "Unable to create a D3DBlob of size: " << CompiledObjectFile.GetDataSize() << L" while compiling shader: " << filepath;
		return( nullptr );
	}


	// Copy the file data into the blob
	memcpy( pBlob->GetBufferPointer(), CompiledObjectFile.GetDataPtr(), pBlob->GetBufferSize() );

	// The file object will automatically be released when it goes out of scope,
	// and hence will free its loaded contents automatically also.

	return( pBlob );
}
//--------------------------------------------------------------------------------
