//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkParameterManager11.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "ArkParameterManager11.h"
#include "ArkVectorParameter11.h"
#include "ArkMatrixParameter11.h"
#include "ArkMatrixArrayParameter11.h"
#include "ArkParameterManager11.h"
#include "ArkLog.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
using namespace DirectX;
//--------------------------------------------------------------------------------
std::map<std::wstring,ArkRenderParameter11*> ArkParameterManager11::m_Parameters;
//--------------------------------------------------------------------------------
ArkParameterManager11::ArkParameterManager11(unsigned int ID)
{
	m_ID = ID;
	m_pWorldMatrix = GetMatrixParameterRef(std::wstring(L"WorldMatrix"));
	m_pViewMatrix  = GetMatrixParameterRef(std::wstring(L"ViewMatrix"));
	m_pProjMatrix  = GetMatrixParameterRef(std::wstring(L"ProjMatrix"));

	m_pWorldViewMatrix     = GetMatrixParameterRef(std::wstring(L"WorldViewMatrix"));
	m_pWorldProjMatrix     = GetMatrixParameterRef(std::wstring(L"WorldProjMatrix"));
	m_pViewProjMatrix      = GetMatrixParameterRef(std::wstring(L"ViewProjMatrix"));
	m_pWorldViewProjMatrix = GetMatrixParameterRef(std::wstring(L"WorldViewProjMatrix"));
}
//--------------------------------------------------------------------------------
ArkParameterManager11::~ArkParameterManager11()
{
	std::map< std::wstring,ArkRenderParameter11* >::iterator iter = m_Parameters.begin();
	while(iter != m_Parameters.end()) {
		Safe_Delete(iter->second);
		iter++;
	}

	m_Parameters.clear();
}
//--------------------------------------------------------------------------------
void ArkParameterManager11::SetConstantBufferParameter(const std::wstring& name,ResourcePtr resource)
{
	ArkRenderParameter11* pParameter = m_Parameters[name];

	// Only create the new parameter if it hasn't already been registered
	if(pParameter == 0)
	{
		pParameter = new ArkConstantBufferParameter11();
		pParameter->SetName(name);
		m_Parameters[name] = reinterpret_cast<ArkRenderParameter11*>(pParameter);

		// Initialize the parameter with the current data in all slots
		pParameter->InitializeParameterData(reinterpret_cast<void*>(&resource->m_iResource));
	}
	else
	{
		if(pParameter->GetParameterType() == CBUFFER)
			pParameter->SetParameterData(reinterpret_cast<void*>(&resource->m_iResource),GetID());
		else
			ArkLog::Get(LogType::Renderer).Output(L"Constant buffer parameter name collision!");
	}
}
//--------------------------------------------------------------------------------
void ArkParameterManager11::SetVectorParameter(const std::wstring& name,DirectX::XMVECTOR* pV)
{
	ArkRenderParameter11* pParameter = m_Parameters[name];

	if(pParameter == 0)
	{
		pParameter = new ArkVectorParameter11();
		pParameter->SetName(name);
		m_Parameters[name] = reinterpret_cast<ArkRenderParameter11*>(pParameter);

		pParameter->InitializeParameterData(reinterpret_cast<void*>(pV));
	}
	else
	{
		if(pParameter->GetParameterType() == ArkParamType::VECTOR)
		{
			pParameter->SetParameterData(reinterpret_cast<void*>(pV),GetID());
		}
		else
		{
			ArkLog::Get(LogType::Renderer).Output(L"VECTOR parameter name error!");
		}
	}

}
//--------------------------------------------------------------------------------
void ArkParameterManager11::SetMatrixParameter(const std::wstring& name,DirectX::XMMATRIX* pM)
{
	ArkRenderParameter11* pParameter = m_Parameters[name];

	if(pParameter == 0)
	{
		pParameter = new ArkMatrixParameter11();
		pParameter->SetName(name);
		m_Parameters[name] = reinterpret_cast<ArkRenderParameter11*>(pParameter);

		pParameter->InitializeParameterData(reinterpret_cast<void*>(pM));
	}
	else
	{
		if(pParameter->GetParameterType() == ArkParamType::MATRIX)
		{
			pParameter->SetParameterData(reinterpret_cast<void*>(pM),GetID());
		}
		else
		{
			ArkLog::Get(LogType::Renderer).Output(L"MATRIX parameter name error!");
		}
	}
}
//--------------------------------------------------------------------------------
void ArkParameterManager11::SetMatrixArrayParameter(const std::wstring& name,DirectX::XMFLOAT4X4* pmA)
{
	ArkRenderParameter11* pParameter = m_Parameters[name];

	if(pParameter == 0)
	{
		pParameter = new ArkMatrixArrayParameter11();
		pParameter->SetName(name);
		m_Parameters[name] = reinterpret_cast<ArkRenderParameter11*>(pParameter);

		pParameter->InitializeParameterData(reinterpret_cast<void*>(pmA));
	}
	else
	{
		if(pParameter->GetParameterType() == ArkParamType::MATRIX_ARRAY)
		{
			pParameter->SetParameterData(reinterpret_cast<void*>(pmA),GetID());
		}
		else
		{
			ArkLog::Get(LogType::Renderer).Output(L"MATRIX_ARRAY parameter name error!");
		}
	}
}
//--------------------------------------------------------------------------------
void ArkParameterManager11::SetConstantBufferParameter(ArkRenderParameter11* pParameter,ResourcePtr resource)
{
	if(pParameter->GetParameterType() == CBUFFER)
		pParameter->SetParameterData(reinterpret_cast<void*>(&resource->m_iResource),GetID());
	else
		ArkLog::Get(LogType::Renderer).Write(L"Constant buffer parameter name collision!");
}
//--------------------------------------------------------------------------------
void ArkParameterManager11::SetVectorParameter(ArkRenderParameter11* pP,DirectX::XMVECTOR* pV)
{
	if(pP->GetParameterType() == ArkParamType::VECTOR)
		pP->SetParameterData(reinterpret_cast<void*>(pV),GetID());
	else
		ArkLog::Get(LogType::Renderer).Output(L"VECTOR parameter name error!");
}
//--------------------------------------------------------------------------------
void ArkParameterManager11::SetMatrixParameter(ArkRenderParameter11* pP,DirectX::XMMATRIX* pV)
{
	if(pP->GetParameterType() == ArkParamType::MATRIX)
		pP->SetParameterData(reinterpret_cast<void*>(pV),GetID());
	else
		ArkLog::Get(LogType::Renderer).Output(L"MATRIX parameter name error!");
}
//--------------------------------------------------------------------------------

void ArkParameterManager11::SetMatrixArrayParameter(ArkRenderParameter11* pP,DirectX::XMFLOAT4X4* pV)
{
	if(pP->GetParameterType() == ArkParamType::MATRIX_ARRAY)
		pP->SetParameterData(reinterpret_cast<void*>(pV),GetID());
	else
		ArkLog::Get(LogType::Renderer).Output(L"MATRIX_ARRAY parameter name error!");
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR ArkParameterManager11::GetVectorParameter(const std::wstring& name)
{
	XMVECTOR pV;
	pV = XMVectorZero();

	ArkRenderParameter11* pParam = GetParameterRef(name);

	if(pParam != 0)
	{
		if(pParam->GetParameterType() == ArkParamType::VECTOR)
			pV = reinterpret_cast<ArkVectorParameter11*>(pParam)->GetVector();
	}
	else
	{
		pParam = new ArkVectorParameter11();
		pParam->SetName(name);
		m_Parameters[name] = pParam;
	}
	return pV;
}
//--------------------------------------------------------------------------------
ArkConstantBufferParameter11* ArkParameterManager11::GetConstantBufferParameterRef(const std::wstring& name)
{
	// Check for the existence of this parameter.  This search includes any
	// parent parameter managers if the parameter doesn't exist in this one.

	ArkRenderParameter11* pParam = GetParameterRef(name);

	// If the parameter is not found, create a new default one.  This goes 
	// into the bottom level manager.

	if(pParam == 0)
	{
		pParam = new ArkConstantBufferParameter11();
		pParam->SetName(name);
		m_Parameters[name] = pParam;
	}

	return(reinterpret_cast<ArkConstantBufferParameter11*>(pParam));
}
//--------------------------------------------------------------------------------
int ArkParameterManager11::GetConstantBufferParameter(const std::wstring& name)
{
	int result;
	result = -1;

	// Check for the existence of this parameter.  This search includes any
	// parent parameter managers if the parameter doesn't exist in this one.

	ArkRenderParameter11* pParam = GetParameterRef(name);

	// If the parameter is not found, create a new default one.  This goes 
	// into the bottom level manager.

	if(pParam != 0)
	{
		if(pParam->GetParameterType() == CBUFFER)
			result = reinterpret_cast<ArkConstantBufferParameter11*>(pParam)->GetIndex(1);
	}
	else
	{
		pParam = new ArkConstantBufferParameter11();
		pParam->SetName(name);
		m_Parameters[name] = pParam;
	}

	return(result);
}
DirectX::XMMATRIX ArkParameterManager11::GetMatrixParameter(const std::wstring& name)
{
	XMMATRIX pM;
	pM = XMMatrixIdentity();

	ArkRenderParameter11* pParam = GetParameterRef(name);

	if(pParam != 0)
	{
		if(pParam->GetParameterType() == ArkParamType::MATRIX)
			pM = reinterpret_cast<ArkMatrixParameter11*>(pParam)->GetMatrix();
	}
	else
	{
		pParam = new ArkVectorParameter11();
		pParam->SetName(name);
		m_Parameters[name] = pParam;
	}
	return pM;
}
//--------------------------------------------------------------------------------
DirectX::XMFLOAT4X4 ArkParameterManager11::GetMatrixArrayParameter(const std::wstring& name)
{
	XMFLOAT4X4 pmA;
	XMStoreFloat4x4(&pmA,XMMatrixIdentity());

	ArkRenderParameter11* pParam = GetParameterRef(name);

	if(pParam != 0)
	{
		if(pParam->GetParameterType() == ArkParamType::MATRIX_ARRAY)
			pmA = reinterpret_cast<ArkMatrixArrayParameter11*>(pParam)->GetMatrixArray();
	}
	else
	{
		pParam = new ArkVectorParameter11();
		pParam->SetName(name);
		m_Parameters[name] = pParam;
	}
	return pmA;
}
//--------------------------------------------------------------------------------
DirectX::XMVECTOR ArkParameterManager11::GetVectorParameter(ArkRenderParameter11* pP)
{
	assert(pP != 0);

	XMVECTOR result;
	result = XMVectorZero();

	if(pP->GetParameterType() == ArkParamType::VECTOR)
	{
		result = reinterpret_cast<ArkVectorParameter11*>(pP)->GetVector();
	}
	return result;
}
//--------------------------------------------------------------------------------
DirectX::XMMATRIX ArkParameterManager11::GetMatrixParameter(ArkRenderParameter11* pP)
{
	XMMATRIX pM;
	pM = XMMatrixIdentity();

	if(pP->GetParameterType() == ArkParamType::MATRIX)
	{
		pM = reinterpret_cast<ArkMatrixParameter11*>(pP)->GetMatrix();
	}
	return pM;
}
//--------------------------------------------------------------------------------
DirectX::XMFLOAT4X4 ArkParameterManager11::GetMatrixArrayParameter(ArkRenderParameter11* pP)
{
	XMFLOAT4X4 pmA;
	XMStoreFloat4x4(&pmA,XMMatrixIdentity());

	if(pP->GetParameterType() == ArkParamType::MATRIX_ARRAY)
		pmA = reinterpret_cast<ArkMatrixArrayParameter11*>(pP)->GetMatrixArray();
	return pmA;
}
int ArkParameterManager11::GetConstantBufferParameter(ArkRenderParameter11* pParameter)
{
	assert(pParameter != 0);

	int result = -1;

	// If the parameter is not found, create a new default one.  This goes 
	// into the bottom level manager.

	if(pParameter->GetParameterType() == CBUFFER)
		result = reinterpret_cast<ArkConstantBufferParameter11*>(pParameter)->GetIndex(GetID());

	return(result);
}
//--------------------------------------------------------------------------------
ArkVectorParameter11* ArkParameterManager11::GetVectorParameterRef(const std::wstring& name)
{
	ArkRenderParameter11* pParam = GetParameterRef(name);

	if(pParam == 0)
	{
		pParam = new ArkVectorParameter11();
		pParam->SetName(name);
		m_Parameters[name] = pParam;
	}

	return (reinterpret_cast<ArkVectorParameter11*>(pParam));
}
//--------------------------------------------------------------------------------
ArkMatrixParameter11* ArkParameterManager11::GetMatrixParameterRef(const std::wstring& name)
{
	ArkRenderParameter11* pParam = GetParameterRef(name);

	if(pParam == 0)
	{
		pParam = new ArkMatrixParameter11();
		pParam->SetName(name);
		m_Parameters[name] = pParam;
	}

	return (reinterpret_cast<ArkMatrixParameter11*>(pParam));
}
//--------------------------------------------------------------------------------
ArkMatrixArrayParameter11* ArkParameterManager11::GetMatrixArrayParameterRef(const std::wstring& name)
{
	ArkRenderParameter11* pParam = GetParameterRef(name);

	if(pParam == 0)
	{
		pParam = new ArkMatrixArrayParameter11();
		pParam->SetName(name);
		m_Parameters[name] = pParam;
	}

	return (reinterpret_cast<ArkMatrixArrayParameter11*>(pParam));
}
//--------------------------------------------------------------------------------
ArkRenderParameter11* ArkParameterManager11::GetParameterRef(const std::wstring& name)
{
	return m_Parameters[name];
}
//--------------------------------------------------------------------------------
void ArkParameterManager11::SetWorldMatrix(DirectX::XMMATRIX* w)
{
	SetMatrixParameter(m_pWorldMatrix,w);

	XMMATRIX WorldMatrix = GetMatrixParameter(m_pWorldMatrix);
	XMMATRIX ViewMatrix  = GetMatrixParameter(m_pViewMatrix);
	XMMATRIX ProjMatrix  = GetMatrixParameter(m_pProjMatrix);

	XMMATRIX WorldViewMatrix = WorldMatrix * ViewMatrix;
	XMMATRIX ViewProjMatrix = ViewMatrix * ProjMatrix;
	XMMATRIX WorldProjMatrix = WorldMatrix * ProjMatrix;
	XMMATRIX WorldViewProjMatrix = WorldMatrix * ViewProjMatrix;

	SetMatrixParameter(m_pWorldViewMatrix,&WorldViewMatrix);
	SetMatrixParameter(m_pViewProjMatrix,&ViewProjMatrix);
	SetMatrixParameter(m_pWorldProjMatrix,&WorldProjMatrix);
	SetMatrixParameter(m_pWorldViewProjMatrix,&WorldViewProjMatrix);
}
//--------------------------------------------------------------------------------
void ArkParameterManager11::SetViewMatrix(DirectX::XMMATRIX* w)
{
	SetMatrixParameter(m_pViewMatrix,w);

	XMMATRIX WorldMatrix = GetMatrixParameter(m_pWorldMatrix);
	XMMATRIX ViewMatrix  = GetMatrixParameter(m_pViewMatrix);
	XMMATRIX ProjMatrix  = GetMatrixParameter(m_pProjMatrix);

	XMMATRIX WorldViewMatrix = WorldMatrix * ViewMatrix;
	XMMATRIX ViewProjMatrix = ViewMatrix * ProjMatrix;
	XMMATRIX WorldProjMatrix = WorldMatrix * ProjMatrix;
	XMMATRIX WorldViewProjMatrix = WorldMatrix * ViewProjMatrix;

	SetMatrixParameter(m_pWorldViewMatrix,&WorldViewMatrix);
	SetMatrixParameter(m_pViewProjMatrix,&ViewProjMatrix);
	SetMatrixParameter(m_pWorldProjMatrix,&WorldProjMatrix);
	SetMatrixParameter(m_pWorldViewProjMatrix,&WorldViewProjMatrix);
}
//--------------------------------------------------------------------------------
void ArkParameterManager11::SetProjectionMatrix(DirectX::XMMATRIX* w)
{
	SetMatrixParameter(m_pProjMatrix,w);

	XMMATRIX WorldMatrix = GetMatrixParameter(m_pWorldMatrix);
	XMMATRIX ViewMatrix  = GetMatrixParameter(m_pViewMatrix);
	XMMATRIX ProjMatrix  = GetMatrixParameter(m_pProjMatrix);

	XMMATRIX WorldViewMatrix = WorldMatrix * ViewMatrix;
	XMMATRIX ViewProjMatrix = ViewMatrix * ProjMatrix;
	XMMATRIX WorldProjMatrix = WorldMatrix * ProjMatrix;
	XMMATRIX WorldViewProjMatrix = WorldMatrix * ViewProjMatrix;

	SetMatrixParameter(m_pWorldViewMatrix,&WorldViewMatrix);
	SetMatrixParameter(m_pViewProjMatrix,&ViewProjMatrix);
	SetMatrixParameter(m_pWorldProjMatrix,&WorldProjMatrix);
	SetMatrixParameter(m_pWorldViewProjMatrix,&WorldViewProjMatrix);
}
//--------------------------------------------------------------------------------
unsigned int ArkParameterManager11::GetID()
{
	return m_ID;
}