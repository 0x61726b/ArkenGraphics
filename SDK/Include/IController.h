//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//IController.h
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// IController
//
// The controller is the basic unit of animation.  The concept used in developing
// this class was taken from the work of Dave Eberly's Game Engine books.  The
// controller is intended to be attached to an entity, and used to vary some
// property with time.
//
// The animated property will be updated during the scene graph update phase.
//--------------------------------------------------------------------------------
#ifndef __IController_h__
#define __IController_h__
//--------------------------------------------------------------------------------
#include "ArkTimer.h"
#include <vector>
//#include <functional>
//--------------------------------------------------------------------------------
namespace Arkeng
{
	//class Entity3D;

	template <typename T>
	class IController
	{
	public:
		IController( );
		virtual ~IController( );
		virtual void Update( float fTime ) = 0;

		void SetEntity( T* pObject );
		T* GetEntity( );

	protected:
		T* m_pEntity;
	};

	template <typename T>
	class ControllerPack
	{
	public:
		ControllerPack( T* host ) : m_host( host ) {};

		~ControllerPack() {
			for ( auto pController : m_Controllers )
				delete pController;
		};

		void Attach( IController<T>* pController ) {
			if ( pController ) {
				m_Controllers.push_back( pController );
				pController->SetEntity( m_host );
			}
		};

		IController<T>* Get( unsigned int index ) {
			if ( m_Controllers.size() <= index )
				return( nullptr );

			return m_Controllers[index];
		};

		void Update( float time ) {
			for ( auto pController : m_Controllers ) {
				pController->Update( time );
			}
		}

	private:
		T* m_host;
		std::vector< IController<T>* > m_Controllers;
	};

	//template <typename T>
	//class ModifierPack
	//{
	//public:
	//	ModifierPack( ) {};
	//	~ModifierPack( ) {};

	//	void Update( T& client, float time ) {
	//		for ( const auto& modify : Modifiers ) {
	//			modify( client, time );
	//		}
	//	};


	//	typedef std::function<void(T&,float)> modifier;

	//	void AddModifier( const modifier& m ) {
	//		Modifiers.push_back( m );
	//	}

	//public:
	//	std::vector< modifier > Modifiers;
	//};

	#include "IController.inl"
};
//--------------------------------------------------------------------------------
#endif // IController_h
