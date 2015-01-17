//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//IEventListener.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "IEventListener.h"
#include "EventManager.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
IEventListener::IEventListener()
	: m_pEventManager( nullptr )
{
}
//--------------------------------------------------------------------------------
IEventListener::~IEventListener()
{
	SetEventManager( nullptr );
}
void IEventListener::RequestEvent( eEvent e )
{
	bool bAlreadyRequested = false;

	for(auto requested:m_vRequestedEvents)
	{
		if(requested==e)
			bAlreadyRequested = true;
	}

	if(bAlreadyRequested)
	{
		//Todo
	}
	else
	{

		m_vRequestedEvents.push_back(e);

		if(m_pEventManager) {
			m_pEventManager->AddEventListener(e,this);
		}
	}
}
//--------------------------------------------------------------------------------
void IEventListener::UnRequestEvent(eEvent e)
{
	for(std::vector<eEvent>::iterator it = m_vRequestedEvents.begin(); it!=m_vRequestedEvents.end(); ++it)
	{
		if(*it==e)
		{
			it = m_vRequestedEvents.erase(it);
		}
	}

	if(m_pEventManager)
	{
		m_pEventManager->DelEventListener(e,this);
	}
}
//--------------------------------------------------------------------------------
void IEventListener::SetEventManager(EventManager* pEventManager)
{
	if(m_pEventManager)
	{
		for(auto e:m_vRequestedEvents)
		{
			m_pEventManager->DelEventListener(e,this);
		}
	}
	m_pEventManager = pEventManager;

	if(m_pEventManager)
	{
		for(auto e:m_vRequestedEvents)
		{
			m_pEventManager->AddEventListener(e,this);
		}
	}
}
//--------------------------------------------------------------------------------