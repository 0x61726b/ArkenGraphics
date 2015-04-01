//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) Alperen Gezer.All rights reserved.
//
//LineIndices.h
//--------------------------------------------------------------------------------
#ifndef __LineIndices_h__
#define __LineIndices_h__
//--------------------------------------------------------------------------------
namespace Arkeng
{
	class LineIndices
	{
	public:
		LineIndices();
		LineIndices( unsigned int P1, unsigned int P2 );
		~LineIndices();

		LineIndices& operator= ( const LineIndices& Line );

		// member access
		unsigned int P1( ) const;
		unsigned int& P1( );
		unsigned int P2( ) const;
		unsigned int& P2( );

		void swapP1P2( );

	protected:
		unsigned int m_uiIndices[2];
	};
};
//--------------------------------------------------------------------------------
#endif // LineIndices_h
