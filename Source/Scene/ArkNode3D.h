//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#ifndef __ArkNode3D_h__
#define __ArkNode3D_h__
//--------------------------------------------------------------------------------
#include "ArkTransform3D.h"
#include "ArkPickRecord.h"
#include "IController.h"
//#include "ParameterContainer.h"
#include "RenderTask.h"
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkEntity3D;

	class ArkNode3D
	{
	public:
		ArkNode3D();
		~ArkNode3D();


		void PreRender( ArkRenderer11* pRenderer, VIEWTYPE view );
		void Render( PipelineManager* pPipelineManager, IParameterManager* pParamManager, VIEWTYPE view );
		
		void Update( float time );
		void UpdateLocal( float time );
		void UpdateWorld( );

		void AttachChild( ArkEntity3D* Child );
		void AttachChild( ArkNode3D* Child );
		void DetachChild( ArkEntity3D* Child );
		void DetachChild( ArkNode3D* Child );

		void AttachParent( ArkNode3D* Parent );
		void DetachParent( );
		ArkNode3D* GetParent( );

		//Entity3D* GetChild( unsigned int index );

		std::wstring toString( );
		void SetName( const std::wstring& name );
		std::wstring GetName() const;

		const std::vector<ArkEntity3D*>& Leafs();
		const std::vector<ArkNode3D*>& Nodes();

		ArkTransform3D Transform;
		ControllerPack<ArkNode3D> Controllers;
	
	protected:
		std::wstring m_Name;

		std::vector< ArkEntity3D* > m_Leafs;
		std::vector< ArkNode3D* > m_Nodes;

		ArkNode3D* m_pParent;
	};
};
//--------------------------------------------------------------------------------
#endif // Node3D_h
//--------------------------------------------------------------------------------