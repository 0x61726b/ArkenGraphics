//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//PointIndices.h
//--------------------------------------------------------------------------------
#ifndef __PointIndices_h__
#define __PointIndices_h__
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class PointIndices
	{
	public:
		PointIndices();
		PointIndices( unsigned int P1 );
		~PointIndices();

		PointIndices& operator= ( const PointIndices& Point );

		// member access
		unsigned int P1( ) const;
		unsigned int& P1( );

	protected:
		unsigned int m_uiIndices[1];
	};
};
//--------------------------------------------------------------------------------
#endif // PointIndices_h
