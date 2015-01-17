//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//IEventListener.h
//--------------------------------------------------------------------------------
#ifndef __IEventListener_h__
#define __IEventListener_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "IEvent.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class EventManager;

	class IEventListener
	{
	public:
		IEventListener();
		virtual ~IEventListener();

		virtual std::wstring GetName() = 0;
		virtual bool HandleEvent(EventPtr pEvent) = 0;

		void RequestEvent(eEvent e);
		void UnRequestEvent(eEvent e);
		void SetEventManager(EventManager* pEventManager);

	private:
		EventManager*			m_pEventManager;
		std::vector<eEvent>		m_vRequestedEvents;
	};
}



#endif //__IEventListener_h__