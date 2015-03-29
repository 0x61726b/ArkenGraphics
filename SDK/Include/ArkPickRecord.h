//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//ArkPickRecord.h
//--------------------------------------------------------------------------------
#ifndef __ArkPickRecord_h__
#define __ArkPickRecord_h__
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class ArkEntity3D;

	class ArkPickRecord
	{
	public:
		ArkPickRecord( );
		virtual ~ArkPickRecord( );
	
	public:
		ArkEntity3D*	pEntity;
		float		fDistance;
	};
};
//--------------------------------------------------------------------------------
#endif // PickRecord_h
