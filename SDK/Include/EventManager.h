//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EventManager.h
//--------------------------------------------------------------------------------
#ifndef __EventManager_h__
#define __EventManager_h__
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "IEvent.h"
#include "IEventListener.h"
//--------------------------------------------------------------------------------

namespace Arkeng
{
	class EventManager
	{
	public:
		EventManager();
		virtual ~EventManager();


		bool AddEventListener(eEvent EventID,IEventListener* pListener);
		bool DelEventListener(eEvent EventID,IEventListener* pListener);

		bool ProcessEvent(EventPtr pEvent);
		bool QueueEvent(EventPtr pEvent);
		bool ProcessEventQueue();

		static EventManager* Get();

	protected:
		std::vector< IEventListener* > m_EventHandlers[NUM_EVENTS];
		std::vector< EventPtr > m_EventQueue;

		static EventManager* m_spEventManager;
	};
}
#endif