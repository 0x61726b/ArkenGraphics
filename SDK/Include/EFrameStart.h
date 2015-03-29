//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//EFrameStart.h
//--------------------------------------------------------------------------------
#ifndef __EFrameStart_h__
#define __EFrameStart_h__
//--------------------------------------------------------------------------------
#include "IEvent.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkTimer;

	class EFrameStart : public IEvent
	{
	public:
		EFrameStart( float dt );
		virtual ~EFrameStart();

		virtual std::wstring GetEventName();
		virtual eEvent GetEventType();

		float GetElapsed();

	protected:
		float m_fDelta;
	};
	typedef std::shared_ptr<EFrameStart> EFrameStartPtr;
};
#endif //__EKeyDown
