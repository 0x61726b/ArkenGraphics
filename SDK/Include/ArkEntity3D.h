//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//RenderTask.h
//--------------------------------------------------------------------------------
#ifndef __ArkEntity3D_h__
#define __ArkEntity3D_h__
//--------------------------------------------------------------------------------
#include "PCH.h"
#include "ArkTransform3D.h"
#include "ArkRay3.h"
#include "ArkFrustum.h"
#include "ArkRenderer11.h"
#include "ArkPickRecord.h"
#include "ArkMaterial11.h"
#include "IController.h"
#include "ArkCompositeShape.h"
#include "ArkRenderable.h"
#include "ArkParameterContainer.h"
#include "RenderTask.h"

#include <string>
#include <vector>
//--------------------------------------------------------------------------------
namespace Arkeng
{

	class ArkNode3D;

	class ArkEntity3D
	{
	public:
		ArkEntity3D( );
		~ArkEntity3D( );

		// Spatial related data and functionality

		void Update( float time );
		void UpdateLocal( float time );
		void UpdateWorld( );


		// Rendering related data and functionality
		void SetRenderParams( IParameterManager* pParamManager );
		void PreRender( ArkRenderer11* pRenderer, VIEWTYPE view );
		void Render( PipelineManager* pPipelineManager, IParameterManager* pParamManager, VIEWTYPE view );

		std::wstring toString( );

		// Scene graph support is added by the following functions.  The graph is enforced
		// by allowing only a single parent, and only Entity3D will be leaf nodes.

		void AttachParent( ArkNode3D* Parent );
		void DetachParent( );
		ArkNode3D* GetParent( );

		

		void SetName( const std::wstring& name );
		std::wstring GetName() const;

		// Provide the ability for a user to add custom data to this entity.  There is no
		// usage of this pointer from within the entity - it is purely a convenience for
		// users.  This also means that the user has to ensure that the object pointed to
		// by pData must be properly managed (i.e. deleted or protected appropriately).

		void SetUserData( void* pData );
		void* GetUserData() const;

	protected:

		ArkNode3D* m_pParent;
		std::wstring m_Name;

	public:
		ArkTransform3D					Transform;
		ControllerPack<ArkEntity3D>		Controllers;
		ArkRenderable					Visual;
		ArkParameterContainer			Parameters;

		/*ArkSphere3 m_ModelBoundingSphere;*/

		// The composite shape consists of multiple shapes to represent the entity.
		// It is currently used for ray picking, but will eventually be added to visibility
		// tests, and possibly used to drive the physics shape generation.

		ArkCompositeShape				Shape;

		// Storage for the custom user data
		void* m_pUserData;

		// Entity flags
		//bool m_bPickable;
	};
};
//--------------------------------------------------------------------------------
#endif // Entity3D_h
//--------------------------------------------------------------------------------
