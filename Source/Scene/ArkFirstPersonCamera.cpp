//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkFirstPersonCamera.cpp
//--------------------------------------------------------------------------------
#include "Pch.h"
#include "EventManager.h"
#include "ArkFirstPersonCamera.h"
#include "EKeyDown.h"
#include "EKeyUp.h"
#include "EFrameStart.h"
#include "EMouseRButtonDown.h"
#include "EMouseRButtonUp.h"
#include "EMouseMove.h"
#include "EMouseLeave.h"
//--------------------------------------------------------------------------------
using namespace Arkeng;
//--------------------------------------------------------------------------------
const UINT ArkFirstPersonCamera::KeyBindings[NumControlKeys] = 
{
    'W', 'S', 'A', 'D', 'Q', 'E', VK_CONTROL
};

static const int InvalidMousePos = -99999;

static const float Pi = 3.14159f;
static const float Pi2 = Pi * 2.0f;
static const float PiOver2 = Pi / 2.0f;
//--------------------------------------------------------------------------------
// Modulo the range of the given angle such that -Pi <= Angle < Pi
float WrapAngle( float Angle )
{
    // Note: The modulo is performed with unsigned math only to work
    // around a precision error on numbers that are close to PI
    float fTemp;

    // Normalize the range from 0.0f to 2Pi
    Angle = Angle + Pi;

    // Perform the modulo, unsigned
    fTemp = fabsf( Angle );
    fTemp = fTemp - ( Pi2 * ( float )( ( int )( fTemp / Pi2 ) ) );

    // Restore the number to the range of -Pi to Pi - epsilon
    fTemp = fTemp - Pi;

    // If the modulo'd value was negative, restore negation
    if ( Angle < 0.0f )
        fTemp = -fTemp;
    
    return fTemp;
}
//--------------------------------------------------------------------------------
ArkFirstPersonCamera::ArkFirstPersonCamera() :    m_iLastMouseX( InvalidMousePos ),
                                            m_iLastMouseY( InvalidMousePos ),
                                            m_iMouseDeltaX( 0 ),
                                            m_iMouseDeltaY ( 0 ),
                                            m_fRotationX( 0.0f ),
                                            m_fRotationY( 0.0f )
{
    for( UINT i = 0; i < NumControlKeys; ++i )
        m_bPressedKeys[i] = false;

    RequestEvent( SYSTEM_KEYBOARD_KEYUP );
    RequestEvent( SYSTEM_KEYBOARD_KEYDOWN );
    RequestEvent( SYSTEM_RBUTTON_DOWN );
    RequestEvent( SYSTEM_RBUTTON_UP );
    RequestEvent( SYSTEM_MOUSE_MOVE );
    RequestEvent( SYSTEM_MOUSE_LEAVE );
    RequestEvent( RENDER_FRAME_START );
}
//--------------------------------------------------------------------------------
ArkFirstPersonCamera::~ArkFirstPersonCamera()
{
}
//--------------------------------------------------------------------------------
bool ArkFirstPersonCamera::HandleEvent( EventPtr pEvent )
{
    eEvent e = pEvent->GetEventType();

    if ( e == SYSTEM_KEYBOARD_KEYDOWN )
    {
        EKeyDownPtr pKeyDown = std::static_pointer_cast<EKeyDown>(pEvent);
        UINT key = pKeyDown->GetCharacterCode();

        for( UINT i = 0; i < NumControlKeys; ++i )
		{
            if( key == KeyBindings[i] )
			{
                m_bPressedKeys[i] = true;
		        return true;
			}
		}
    }
    else if ( e == SYSTEM_KEYBOARD_KEYUP )
    {
        EKeyUpPtr pKeyUp = std::static_pointer_cast<EKeyUp>(pEvent);

        UINT key = pKeyUp->GetCharacterCode();

        for( UINT i = 0; i < NumControlKeys; ++i )
		{
            if( key == KeyBindings[i] )
            {
				m_bPressedKeys[i] = false;
			    return true;
			}
		}
    }
    else if ( e == SYSTEM_MOUSE_MOVE )
    {
        EMouseMovePtr pMouseMove = std::static_pointer_cast<EMouseMove>(pEvent);
        
        m_iMouseDeltaX = 0;
        m_iMouseDeltaY = 0;

        const int mouseX = pMouseMove->GetX();
        const int mouseY = pMouseMove->GetY();

        // Only rotate the camera when dragging the right mouse button
        if( pMouseMove->RButtonDown() && m_iLastMouseX != InvalidMousePos && m_iLastMouseY != InvalidMousePos )
        {
            m_iMouseDeltaX = mouseX - m_iLastMouseX;
            m_iMouseDeltaY = mouseY - m_iLastMouseY;
        }

        m_iLastMouseX = mouseX;
        m_iLastMouseY = mouseY;

		

		return true;
    }
    else if ( e == SYSTEM_MOUSE_LEAVE )
    {
        m_iLastMouseX = InvalidMousePos;
        m_iLastMouseY = InvalidMousePos;
    }
    else if ( e == SYSTEM_RBUTTON_DOWN )
    {        
        m_iLastMouseX = InvalidMousePos;
        m_iLastMouseY = InvalidMousePos;

		return true;
    }
    else if ( e == SYSTEM_RBUTTON_UP )
    {
        m_iLastMouseX = InvalidMousePos;
        m_iLastMouseY = InvalidMousePos; 

		return true;
    }
    else if ( e == RENDER_FRAME_START )
    {
        Update();
    }

    return false;
}
//--------------------------------------------------------------------------------
std::wstring Arkeng::ArkFirstPersonCamera::GetName()
{
    return L"First Person Camera";
}
//--------------------------------------------------------------------------------
void ArkFirstPersonCamera::Update()
{
    timer.Update();
    float timeDelta = timer.Elapsed();    

    float CamMoveSpeed = 10.0f * timeDelta;
    const float CamRotSpeed = 0.24f * timeDelta;    

    // Move the camera with keyboard input
    if ( m_bPressedKeys[SpeedUpKey] )
        CamMoveSpeed *= 3.0f;

    // Move the camera with the keyboard
    if ( m_bPressedKeys[RightKey] )
	{
        Spatial().MoveRight( CamMoveSpeed );
	}
    else if ( m_bPressedKeys[LeftKey] )
        Spatial().MoveLeft( CamMoveSpeed );
    if ( m_bPressedKeys[UpKey] )
        Spatial().MoveUp( CamMoveSpeed );
    else if ( m_bPressedKeys[DownKey] )
        Spatial().MoveDown( CamMoveSpeed );
    if ( m_bPressedKeys[ForwardKey] )
        Spatial().MoveForward( CamMoveSpeed );
    else if ( m_bPressedKeys[BackKey] )
        Spatial().MoveBackward( CamMoveSpeed );

    // Rotate the camera with the mouse
    m_fRotationX += m_iMouseDeltaY * CamRotSpeed;
    m_fRotationY += m_iMouseDeltaX * CamRotSpeed;



    m_iMouseDeltaX = 0;
    m_iMouseDeltaY = 0;

	 if ( m_fRotationX < -PiOver2 )
        m_fRotationX = -PiOver2;
    else if ( m_fRotationX > PiOver2 )
        m_fRotationX = PiOver2;
    
    m_fRotationY = WrapAngle( m_fRotationY );

    // Make a rotation matrix from the X/Y rotation
	Spatial().RotateBy( DirectX::XMVectorSet( 0, m_fRotationY, 0.0f,0.0f ) );
	m_fRotationX = 0.0f;
	m_fRotationY = 0.0f;
}
//--------------------------------------------------------------------------------
