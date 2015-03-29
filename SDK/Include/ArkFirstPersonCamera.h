//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkFirstPersonCamera.h
//--------------------------------------------------------------------------------
#ifndef __ArkFirstPersonCamera_h__
#define __ArkFirstPersonCamera_h__
//--------------------------------------------------------------------------------
#include "Camera.h"
#include "ArkTimer.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkFirstPersonCamera : public Camera
    {
    public:
        ArkFirstPersonCamera();
		virtual ~ArkFirstPersonCamera();
        
        virtual bool HandleEvent( EventPtr pEvent );
        virtual std::wstring GetName();

    protected:

        void Update();        

        enum ControlKeys
        {
            ForwardKey = 0,
            BackKey,
            LeftKey,
            RightKey,
            UpKey,
            DownKey,
            SpeedUpKey,

            NumControlKeys
        };

        // Key bindings for camera movement
        static const UINT KeyBindings[NumControlKeys];

        bool m_bPressedKeys[NumControlKeys];        
        int m_iLastMouseX;
        int m_iLastMouseY;
        int m_iMouseDeltaX;
        int m_iMouseDeltaY;

        float m_fRotationX;
        float m_fRotationY;

        ArkTimer timer;
    };
};

//--------------------------------------------------------------------------------
#endif